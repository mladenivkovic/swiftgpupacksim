#!/usr/bin/env python3

import os
from typing import Union
import subprocess
import datetime
import yaml


# Allow certain "field names" to be multiply defined for validation checks
# NB: You'll have to manually exception-handle them in the generation. This is
# not handled here.
_permitted_duplicate_field_names = [
    "doc",
    "documentation",
    "accessor_id",
]

_required_file_metadata_swift = [
    "authors",
    "flavour",
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


def read_input_file(filename: str):
    """
    Read the yaml input file.

    Returns
    -------

    part_data_dict: Dict containing all particle fields specified in yaml file

    metadata_dict: Dict containing (SPH flavour) metadata specified in yaml file
    """

    verify_file_exists(filename)
    input_fp = open(filename, "r")
    yaml_data_d = yaml.safe_load(input_fp)
    input_fp.close()

    part_data_dict = {}
    # Set some default values
    metadata_dict = {
        "authors": "NO AUTHORS SPECIFIED",
        "flavour": "NO_FLAVOUR_SPECIFIED",
    }

    for key in yaml_data_d.keys():
        if key == "metadata":
            # copy contents of group "metadata", not entire group
            for key2 in yaml_data_d[key].keys():
                metadata_dict[key2] = yaml_data_d[key][key2]
        else:
            part_data_dict[key] = yaml_data_d[key]

    return part_data_dict, metadata_dict


def sanitize_include_headers(headerfiles: list):
    """
    Sanitize header file names, passed as a list of strings

    Returns: list of sanitized file names
    """
    sanitized = []

    for inc in headerfiles:
        inc = inc.strip()
        if len(inc) == 0:
            continue
        if (not inc.startswith('"')) and (not inc.startswith("<")):
            inc = '"' + inc
        if (not inc.endswith('"')) and (not inc.endswith(">")):
            inc = inc + '"'
        sanitized.append(inc)
    return sanitized


def read_default_includes(swift_header: bool = False, verbose: bool=False):
    """
    Read default headers to be included.

    Returns: List of headers as strings
    """

    fname = "default_headers_swiftgpupacksim.h"
    if swift_header:
        fname = "default_headers_swift.h"

    fname_full = os.path.join("input", fname)

    if verbose:
        print(f"-- checking metadata: Using defaults from {fname_full} with additions from yml file")

    f = open(fname_full, "r")
    lines=f.readlines()
    headers = [l.strip() for l in lines]
    f.close()

    sanitized = sanitize_include_headers(headers)
    return sanitized



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
                    f"data field '{field}' defined more than once in your input yml file."
                    + "This will create multiply defined getters and setters."
                )
            field_names.append(field)

    return

def process_yml_metadata(metadata_d: dict, swift_header: bool = False, verbose: bool = False) -> None:
    """
    Run through the read-in metadata from the yml file, passed as the dict
    `metadata_d`, validate that there are no issues, and set up defaults if
    necessary.

    Parameters
    ----------

    metadata_d: dict
        dict containing read-in metadata items from the yml file

    swift_header: bool
        if True, we're creating header files for SWIFT. Otherwise, for
        SWIFTGPUPACKSIM.

    verbose: bool
        are we talkative?
    """

    passed_fields = list(metadata_d.keys())

    if swift_header:
        # we only require metadata to be present if we're compiling swift headers.

        for field in _required_file_metadata_swift:
            if field not in passed_fields:
                raise ValueError(f"Required field {field} not found in read-in metadata")

    # Now go through fields one-by-one

    if "authors" not in passed_fields:
        metadata_d["author"] = "NO AUTHORS SPECIFIED"
        if verbose:
            print("-- checking metadata: No authors found")

    if "flavour" not in passed_fields:
        metadata_d["flavour"] = get_git_hash()
        if verbose:
            print("-- checking metadata: No SPH flavour found")

    if ("doc" not in passed_fields) and ("documentation" not in passed_fields):
        metadata_d["has_doc"] = False
        if verbose:
            print("-- checking metadata: No file documentation found")

        # place documentation contents with a key you'll know to search for later
        if "doc" not in passed_fields:
            doc = ""
            if "documentation" in passed_fields:
                doc = metadata_d["documentation"]
            metadata_d["doc"] = doc
    else:
        metadata_d["has_doc"] = True

    if ("includes") not in passed_fields:
        if verbose:
            print(f"-- checking metadata: No header includes provided.")

        # use defaults.
        incs = read_default_includes(swift_header=swift_header, verbose=verbose)
        metadata_d["includes"] = incs

    else:
        # sanitize input
        sanitized = sanitize_include_headers(metadata_d["includes"])
        metadata_d["includes"] = sanitized


    if ("includes_add") in passed_fields:

        if ("includes") in passed_fields:
            raise ValueError("Got both 'includes' and 'includes_add' parameters, pick one!")

        # sanitize input
        sanitized = sanitize_include_headers(metadata_d["includes_add"])
        metadata_d["includes_add"] = sanitized
        metadata_d["has_extra_includes"] = True

        # now use defaults for 'includes' field, then add these later.
        incs = read_default_includes(swift_header=swift_header, verbose=verbose)
        metadata_d["includes"] = incs


    else:
        metadata_d["includes_add"] = [""]
        metadata_d["has_extra_includes"] = False



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


def add_auxiliary_fields(
    contents_d: dict,
    id_checks: bool = True,
    part_struct_accessors: bool = False,
    verbose: bool = False,
) -> dict:
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

    part_struct_accessors: bool
        if True, add auxiliary fields needed for part struct accessors (offset/index in cell's particle arrays for this particle, and pointer to cell_part_arrays)

    verbose: bool
        if True, be talkative


    Returns:
    --------

    updated_contents_d: dict
        Updated dict of particle fields
    """

    have_part_struct = check_part_struct_first_in_list(contents_d)
    if len(list(contents_d.keys())) == 1:
        # we only have a single part struct, no need to add accessors
        return contents_d

    # fake having field props for the auxiliary fields
    main_part_struct_aux_fields_props = {}

    if part_struct_accessors:
        # without part struct accessors, we don't need any further aux fields
        main_part_struct_aux_fields_props = {
            "cell_offset": {
                "type": "size_t",
                "doc": "offset/index of particle in cell particle data array",
                "ifdef": "USE_PART_STRUCT_ACCESSORS",
            },
            "cell_part_arrays": {
                "type": "struct part_arrays*",
                "doc": "pointer to particle data array struct of the cell this particle is located in",
                "ifdef": "USE_PART_STRUCT_ACCESSORS",
            },
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

        if verbose:
            print("-- Didn't find struct `part`, adding it.")

        updated_contents_d["part"] = main_part_struct_aux_fields_props

        # Copy data over
        for key in contents_d.keys():
            updated_contents_d[key] = contents_d[key]

    else:
        # start empty
        updated_contents_d["part"] = {}

        # Add auxiliary fields to the updated contents
        for key in main_part_struct_aux_fields_props:
            # Check that we don't have these field names already
            if key in contents_d["part"].keys():
                raise ValueError(f"key '{key} is already a field name in part struct!")
            updated_contents_d["part"][key] = main_part_struct_aux_fields_props[key]

        # Now copy data over
        for key in contents_d.keys():
            if key == "part":
                for partkey in contents_d["part"].keys():
                    updated_contents_d["part"][partkey] = contents_d["part"][partkey]
            else:
                updated_contents_d[key] = contents_d[key]

    if id_checks:
        if len(list(updated_contents_d.keys())) == 1:
            if verbose:
                print(
                    "-- Found only one struct for particle data, not adding accessor_id checks"
                )
        else:
            # Add struct ID for debugging checks to each struct
            for key in updated_contents_d.keys():
                updated_contents_d[key][debug_id_props_name] = debug_id_props[
                    debug_id_props_name
                ]

                if verbose:
                    print(
                        f"-- Adding debug check field {debug_id_props_name} to struct {key}"
                    )

    return updated_contents_d
