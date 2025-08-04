#!/usr/bin/env python3

from parse_yaml import FieldEntry
from templates import jinja_generate_hydro_part_h


def generate_hydro_part_header(
    part_d: dict,
    swift_header: bool = True,
    testing: bool = False,
    verbose: bool = False,
) -> str:
    """
    Generate a hydro_part.h.

    Parameters
    ----------

    part_d: dict
        dict containing the read-in particle struct fields

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    testing: bool
        if True, use minimalistic header template for unit tests to generate
        header file

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_file: str
        The contents of the hydro_part.h file.
    """

    if verbose:
        print("Generating particle struct header")

    part_keylist = list(part_d.keys())
    have_part_struct = False
    if len(part_keylist) > 1:
        # if 'part' is defined, it must be first.
        for i, key in enumerate(part_keylist):
            if key == "part":
                have_part_struct = True
                if i != 0:
                    raise ValueError("You're defining a particle data struct 'part', but it isn't in first position.\n"
                                     +f"Put it at the top of your .yml file. Current position: {i+1}")

    # Create an empty dict we'll fill out with C-code for the API and
    # declaration C-code snippets for each struct containing particle data
    part_struct_d = {}

    # TODO: add cell and offset fields
    # TODO: document them as prohibited/reserved

    if not have_part_struct:
        raise NotImplementedError("Implement addition of part struct as first")

    for struct_name in part_d.keys():

        declarations = []
        apis = []

        #  if struct_name == "part" and len(part_keylist) > 1:
            # Generate getters for other structs


        for field in list(part_d[struct_name].keys()):
            if FieldEntry.prohibited_name(field):
                if verbose:
                    print(f"-- Found prohibited field name {field}, skipping it.")
                continue
            # parse the field specification
            field_props = part_d[struct_name][field]
            field_entry = FieldEntry(field, field_props, verbose)
            # get the declaration C-code
            decl = field_entry.generate_declaration(verbose=verbose)
            declarations.append(decl)
            # get the API C-code
            api = field_entry.generate_API(verbose=verbose)
            apis.append(api)

        # store the parsed data
        part_struct_d[struct_name] = {"API": apis, "DECLARATIONS": declarations}

    # Now generate the file from template
    header_file = jinja_generate_hydro_part_h(
        part_struct_d, swift_header=swift_header, testing=testing, verbose=verbose
    )

    return header_file
