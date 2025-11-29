#!/usr/bin/env python3

from parse_yaml import FieldEntry
from templates import jinja_generate_hydro_part_h
from utils import check_part_struct_first_in_list


def generate_hydro_part_header(
    part_d: dict,
    swift_header: bool = True,
    id_checks: bool = True,
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

    id_checks: bool
        if True, add a struct_id field to every struct for debugging checks
        and a debugging check in each getter/setter

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

    have_part_struct = check_part_struct_first_in_list(part_d)

    # Create an empty dict we'll fill out with C-code for the API and
    # declaration C-code snippets for each struct containing particle data
    part_struct_d = {}

    # Now go through all the structs provided in the yml file
    for struct_name in part_d.keys():

        declarations = []
        apis = []

        for field in list(part_d[struct_name].keys()):
            if FieldEntry.prohibited_name(field):
                if verbose:
                    print(f"-- Found prohibited field name {field}, skipping it.")
                continue
            # parse the field specification
            field_props = part_d[struct_name][field]
            field_entry = FieldEntry(field, field_props, struct_name, verbose=verbose)
            # get the declaration C-code
            decl = field_entry.generate_declaration(verbose=verbose)
            declarations.append(decl)
            # get the API C-code
            api = field_entry.generate_API(verbose=verbose, id_checks=id_checks)
            apis.append(api)

        # store the parsed data
        part_struct_d[struct_name] = {"API": apis, "DECLARATIONS": declarations}

    # Now generate the file from template
    header_file = jinja_generate_hydro_part_h(
        part_struct_d, swift_header=swift_header, testing=testing, verbose=verbose
    )

    return header_file
