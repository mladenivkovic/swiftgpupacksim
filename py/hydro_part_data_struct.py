#!/usr/bin/env python3

from parse_yaml import FieldEntry
from templates import jinja_generate_hydro_part_h, jinja_generate_hydro_part_data_h


def generate_hydro_part_data_struct(
    part_d: dict,
    swift_header: bool = True,
    verbose: bool = False,
) -> str:
    """
    Generate a hydro_part_data.h, which contains a struct with pointers
    to all particle fields in a cell.

    Parameters
    ----------

    part_d: dict
        dict containing the read-in particle struct fields

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_file: str
        The contents of the hydro_part.h file.
    """

    if verbose:
        print("Generating particle_data struct header")

    part_keylist = list(part_d.keys())
    have_part_struct = False

    # if 'part' is defined, it must be first.
    for i, key in enumerate(part_keylist):
        if key == "part":
            have_part_struct = True
            if i != 0:
                raise ValueError("You're defining a particle data struct 'part', but it isn't in first position.\n"
                                 +f"Put it at the top of your .yml file. Current position: {i+1}")

    if not have_part_struct:
        # 'part' not provided in yml file. Add a 'struct part' in first place
        keylist = ["part"]
        keylist += part_keylist
        part_struct_d = {"part": {"DOC": "the main particle data struct", "HAS_DOC": True,}}
    else:
        keylist = part_keylist
        part_struct_d = {}

    # Now grab all the others
    for struct_name in part_keylist:

        props = part_d[struct_name]

        # Do we have documentation?
        documentation = None
        try:
            documentation = props["doc"]
            if verbose:
                print("  documentation:", documentation)
        except KeyError:
            pass
        try:
            documentation = props["documentation"]
            if verbose:
                print("  documentation:", documentation)
        except KeyError:
            pass

        part_struct_d[struct_name] = {"DOC": documentation, "HAS_DOC": documentation is not None}


    if verbose:
        print("--", part_struct_d)

    #  generate the file from template
    header_file = jinja_generate_hydro_part_data_h(
        part_struct_d, swift_header=swift_header, verbose=verbose
    )

    return header_file
