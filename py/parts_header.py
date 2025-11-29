#!/usr/bin/env python3

from parse_yaml import FieldEntry
from templates import jinja_generate_parts_h, jinja_generate_hydro_part_arrays_struct_h
from utils import check_part_struct_first_in_list


def generate_parts_header(
    part_d: dict,
    swift_header: bool = True,
    id_checks: bool=True,
    verbose: bool = False,
) -> str:
    """
    Generate a parts.h, which contains:
    - `init_part_arrays()`, function to allocate memory for all
      arrays in `struct part_arrays`
    - `clear_parts()`, function to free all data allocated in `init_part_arrays()`

    Parameters
    ----------

    part_d: dict
        dict containing the read-in particle struct fields

    swift_header: bool
        if True, generate headers compatible with swift, not swiftgpupacksim

    id_checks: bool
        If True, add initialisation of particle accessor_id variables

    verbose: bool
        if True, be talkative

    Returns
    -------

    header_file: str
        The contents of the hydro_part.h file.
    """

    if verbose:
        print("Generating parts.h")

    have_part_struct = check_part_struct_first_in_list(part_d)
    if not have_part_struct:
        raise ValueError("Part struct should've been added beforehand. Call this only after generating hydro_part.h")

    # Prep the dict for jinja for each struct
    part_struct_d = {}

    # ... and fill out its contents
    for struct_name in part_d.keys():

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

        part_struct_d[struct_name] = {
            "DOC": documentation,
            "HAS_DOC": documentation is not None,
        }

    if verbose:
        print("--", part_struct_d)

    #  generate the file from template
    header_file = jinja_generate_parts_h(
        part_struct_d, swift_header=swift_header, verbose=verbose
    )

    return header_file


def generate_hydro_part_arrays_struct_header(
    part_d: dict,
    swift_header: bool = True,
    verbose: bool = False,
) -> str:
    """
    Generate a hydro_part_arrays_struct.h, which contains a `struct part_arrays`, containing
    pointers to all particle fields in a cell.

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
        print("Generating hydro_part_arrays_struct.h header")

    have_part_struct = check_part_struct_first_in_list(part_d)
    if not have_part_struct:
        raise ValueError("Part struct should've been added beforehand. Call this only after generating hydro_part.h")

    # Prep the dict for jinja for each struct
    part_struct_d = {}

    # ... and fill out its contents
    for struct_name in part_d.keys():

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

        part_struct_d[struct_name] = {
            "DOC": documentation,
            "HAS_DOC": documentation is not None,
        }

    if verbose:
        print("--", part_struct_d)

    #  generate the file from template
    header_file = jinja_generate_hydro_part_arrays_struct_h(
        part_struct_d, swift_header=swift_header, verbose=verbose
    )

    return header_file
