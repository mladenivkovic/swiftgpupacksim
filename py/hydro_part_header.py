#!/usr/bin/env python3


def generate_single_part_struct(part_d: dict, verbose: bool=False):
    """
    Generate a hydro_part.h with a single struct part
    """

    if verbose:
        print("Generating single particle struct")
    print(part_d)
    #  print(type(part_d))
    #  print(part_d.keys())
    #  print(len(part_d.keys()))

    struct_name = list(part_d.keys())[0]
    #  print(struct_name)

    #  print(part_d["part"])
    #  print(part_d["part"]["x"])

    print("loop")
    for field in list(part_d[struct_name].keys()):
        field_type = "float"
        field_size = 1
        #  "field" here is x, h, m...
        field_props = part_d[struct_name][field]
        if field_props is None:
            print("Found float:", field)
        else:
            try:
                field_type = field_props["type"]
                print("Found type:", field_type)
            except KeyError:
                pass

            try:
                field_size = field_props["size"]
                print("Found size:", field_size)
            except KeyError:
                pass


    #      print(field.type)
    #      print(field)
    #      print(type(field))
    #


    return


def generate_split_part_structs(part_d: dict, verbose: bool=False):
    """
    Generate a hydro_part.h with a single struct part
    """

    if verbose:
        print("Generating particle composed of several structs")


    return
