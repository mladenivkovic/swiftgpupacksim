#!/usr/bin/env python3

from parse_yaml import FieldEntry
from templates import jinja_generate_hydro_part_h


def generate_hydro_part_header(
    part_d: dict, swift_header: bool = True, verbose: bool = False
)-> str:
    """
    Generate a hydro_part.h with a single struct part.

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
        print("Generating single particle struct")

    if len(part_d.keys()) > 1:
        raise NotImplementedError("Split structs not implemented yet")

    part_struct_d = {}

    for struct_name in part_d.keys():

        declarations = []
        apis = []

        for field in list(part_d[struct_name].keys()):
            # parse the field specification
            field_props = part_d[struct_name][field]
            field_entry = FieldEntry(field, field_props, verbose)
            # get the declaration C-code
            decl = field_entry.generate_declaration()
            declarations.append(decl)
            # get the API C-code
            api = field_entry.generate_API()
            apis.append(api)

        # store the parsed data
        part_struct_d[struct_name] = {"API": apis, "DECLARATIONS": declarations}

    header_file = jinja_generate_hydro_part_h(
        part_struct_d, swift_header=swift_header, verbose=verbose
    )

    return header_file
