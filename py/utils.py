#!/usr/bin/env python3

import os
from typing import Union
import subprocess
import datetime


# Allow certain "field names" to be multiply defined for validation checks
# NB: You'll have to manually exception-handle them in the generation. This is
# not handled here.
_permitted_duplicate_field_names = [
    "doc", "documentation", "accessor_id",
    ]


def verify_file_exists(fname: str, message: str = ""):
    """
    Check if file exists, and if not, raise an error.
    Optionally, pass an additional message to be displayed upon error.
    """

    if not os.path.exists(fname):
        raise FileNotFoundError(f"File {fname} doesn't exist. {message}")
    return


def verify_dir_exists(dname: str, message: str = ""):
    """
    Check if file exists, and if not, raise an error.
    Optionally, pass an additional message to be displayed upon error.
    """

    if not os.path.isdir(dname):
        raise FileNotFoundError(f"Directory {dname} doesn't exist. {message}")
    return


def check_output_directory(outdir: Union[str, list], verbose: bool = True):
    """
    Extract the output directory from the parsed args.
    If it doesn't exist, make it.
    """

    if isinstance(outdir, list):
        if len(outdir) > 1:
            raise ValueError(
                "outdir cmdline arg should only contain 1 element?", outdir
            )
        outdir = outdir[0]
    elif isinstance(outdir, str):
        # we're good
        pass
    else:
        raise ValueError("Unknown type for output_dir cmdline arg:", type(outdir))

    if not os.path.isdir(outdir):
        if verbose:
            print(f"output directory '{outdir}' doesn't exist, creating it.")
        os.makedirs(outdir)

    return outdir


def get_git_hash() -> str:
    """
    Get the git hash of the latest commit.
    If git is unavailable or command fails for any reason, return current time instead.
    """

    git_hash_cmd = "git rev-parse --verify HEAD"
    try:
        githash = subprocess.run(
            git_hash_cmd,
            shell=True,
            check=True,
            capture_output=True,
        )
        gitstdout = githash.stdout
        if isinstance(gitstdout, bytes):
            gitstdout = gitstdout.decode("utf8")
        hashstr = gitstdout.strip()
    except subprocess.CalledProcessError:
        print("Something went wrong when trying to obtain hash of latest git commit.")
        print(f"Command that failed was '{git_hash_cmd}'")
        print("Using current timestamp instead.")
        hashstr = datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S_%f")

    return hashstr


def print_separator(msg: str = ""):
    print("----------------------------------------------------------------", msg)
    return


def validate_yml_contents(contents_d: dict) -> None:
    """
    Run through the read-in data from the yml file, passed as the dict
    `contents_d`, and validate that there is no issues:

    (- no top level struct defined twice)
    - no fields with same name defined twice
    """

    # This doesn't really work, since the yaml reader just overwrites them.
    #  struct_names = []
    #  for key in list(contents_d.keys()):
    #
    #      print(key)
    #      if key in struct_names:
    #          raise ValueError(f"particle data struct {key} defined more than once in your input yml file.")
    #      struct_names.append(key)

    field_names = []
    for key in list(contents_d.keys()):

        struct_fields = contents_d[key]
        for field in list(struct_fields.keys()):
            if field in _permitted_duplicate_field_names:
                # These are allowed to exist multiple times.
                continue
            if field in field_names:
                raise ValueError(
                    f"data field '{field}' defined more than once in your input yml file."+
                    "This will create multiply defined getters and setters."
                )
            field_names.append(field)

    return


def check_part_struct_first_in_list(contents_d: dict) -> bool:
    """
    Ensure that the particle struct is first in line.

    contents_d: dict
        dict containing yml read-in data of particle structs and fields to
        generate headers from

    Returns
    -------

    has_part_struct: bool
        True if contents_d has a struct 'part' to be generated
    """

    part_keylist = list(contents_d.keys())
    have_part_struct = False

    if len(part_keylist) > 1:
        # if 'part' is defined, it must be first.
        for i, key in enumerate(part_keylist):
            if key == "part":
                have_part_struct = True
                if i != 0:
                    raise ValueError(
                        "You're defining a particle data struct 'part', but it isn't in first position.\n"
                        + f"Put it at the top of your .yml file. Current position: {i+1}"
                    )

    return have_part_struct


def add_auxiliary_fields(contents_d: dict, id_checks: bool=True, verbose: bool=False) -> dict:
    """
    Run through the read in contents (passed as contents_d) and add auxiliary
    fields, if necessary:
    - If a `struct part` does not exist, create one and put it in first position.
    - If we're doing debug_id_checks for each struct, add that field to each substruct

    Parameters
    ----------

    contents_d: dict
        dict containing contents of read-in yml file to generate structs and fields from

    id_checks: bool
        if True, add a field for IDs for each struct for debugging checks

    verbose: bool
        if True, be talkative


    Returns:
    --------

    updated_contents_d: dict
        Updated dict of particle fields
    """

    have_part_struct = check_part_struct_first_in_list(contents_d)

    # fake having field props for the auxiliary fields
    main_part_struct_aux_fields_props = {
            "cell_offset":
            {
                "type": "size_t",
                "doc": "offset/index of particle in cell particle data array",
            },
            "cell_part_arrays":
            {
                "type": "struct hydro_part_arrays*",
                "doc": "pointer to particle data array struct of the cell this particle is located in",
            }
        }

    debug_id_props_name = "accessor_id"
    debug_id_props = {
            debug_id_props_name: {
                "type": "long long",
                "doc": "This particle's accessor ID, identical for all structs associated with this particle.",
                "ifdef": "SWIFT_DEBUG_CHECKS",
                }
        }

    updated_contents_d = {}

    if not have_part_struct:
        # Add 'struct part' manually. Contents will be filled below.

        if (verbose):
            print("-- Didn't find struct `part`, adding it.")

        updated_contents_d["part"] = main_part_struct_aux_fields_props

        # Copy data over
        for key in contents_d.keys():
            updated_contents_d[key] = contents_d[key]

    else:
        # Check that we don't have these field names already
        for key in main_part_struct_aux_fields_props:
            if key in contents_d["part"].keys():
                raise ValueError(f"key '{key} is already a field name in part struct!")
            updated_contents_d["part"][key] = main_part_struct_aux_fields_props[key]

    if id_checks:
        if len(list(updated_contents_d.keys()))== 1:
            if (verbose):
                print("-- Found only one struct for particle data, not adding accessor_id checks")
        else:
            # Add struct ID for debugging checks to each struct
            for key in updated_contents_d.keys():
                updated_contents_d[key][debug_id_props_name] = debug_id_props[debug_id_props_name]

                if (verbose):
                    print(f"-- Adding debug check field {debug_id_props_name} to struct {key}")

    return updated_contents_d



