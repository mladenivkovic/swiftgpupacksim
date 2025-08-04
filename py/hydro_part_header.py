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

    # fake having field props for the auxiliary fields
    main_part_struct_aux_fields_props = [
            ("cell_offset", {"type": "size_t", "doc": "offset of particle in cell particle data array"}),
            ("cell_part_data", {"type": "struct hydro_part_data*", "doc": "pointer to cell particle data array struct"})
    ]

    if not have_part_struct:
        # Add 'struct part' manually.

        declarations = []
        apis = []

        # First, generate required fields and getters for other particle data structs.
        for field_data in main_part_struct_aux_fields_props:
            field_name = field_data[0]
            field_props = field_data[1]
            # parse the field specification
            field_entry = FieldEntry(field_name, field_props, "part", verbose=verbose, allow_prohibited=True)
            # get the declaration C-code
            decl = field_entry.generate_declaration(verbose=verbose)
            declarations.append(decl)
            # get the API C-code
            #  api = field_entry.generate_API(verbose=verbose)
            #  apis.append(api)
        # store the parsed data
        part_struct_d["part"] = {"API": apis, "DECLARATIONS": declarations}



    # Now go through all the structs provided in the yml file
    for struct_name in part_d.keys():

        declarations = []
        apis = []

        # First, generate required fields and getters for other particle data structs.
        if struct_name == "part" and len(part_keylist) > 1:
            for field_data in main_part_struct_aux_fields_props:
                field_name = field_data[0]
                field_props = field_data[1]
                # parse the field specification
                field_entry = FieldEntry(field_name, field_props, "part", verbose=verbose, allow_prohibited=True)
                # get the declaration C-code
                decl = field_entry.generate_declaration(verbose=verbose)
                declarations.append(decl)
                # get the API C-code
                #  api = field_entry.generate_API(verbose=verbose)
                #  apis.append(api)


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
            api = field_entry.generate_API(verbose=verbose)
            apis.append(api)

        # store the parsed data
        part_struct_d[struct_name] = {"API": apis, "DECLARATIONS": declarations}

    # Now generate the file from template
    header_file = jinja_generate_hydro_part_h(
        part_struct_d, swift_header=swift_header, testing=testing, verbose=verbose
    )

    return header_file
