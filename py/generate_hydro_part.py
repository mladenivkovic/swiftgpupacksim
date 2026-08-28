#!/usr/bin/env python3

import argparse
import os

from utils import (
    verify_file_exists,
    check_output_directory,
    print_separator,
    validate_yml_contents,
    process_yml_metadata,
    add_auxiliary_fields,
    read_input_file,
)
from headers import (
    generate_hydro_part_dot_h,
    generate_parts_dot_h,
    generate_hydro_part_arrays_struct_dot_h,
    generate_hydro_part_arrays_flush_dot_h,
    generate_hydro_space_dot_c,
)


parser = argparse.ArgumentParser(description="Generate the particle header files.")
parser.add_argument("input_file")
parser.add_argument(
    "-v",
    "--verbose",
    dest="verbose",
    action="store_true",
    default=False,
    help="Toggle verbosity",
)
parser.add_argument(
    "-s",
    "--swift",
    dest="swift",
    action="store_true",
    default=False,
    help="Generate output compatible swift, not for swiftgpupacksim",
)
parser.add_argument(
    "-d",
    "--dry-run",
    dest="dry_run",
    action="store_true",
    default=False,
    help="Dry run: Print output files to screen instead of writing them to file",
)
parser.add_argument(
    "-o",
    "--outputdir",
    nargs=1,
    dest="output_dir",
    action="store",
    default=".",
    type=str,
    help="Directory to write output into",
)
parser.add_argument(
    "-t",
    "--test",
    dest="testing",
    action="store_true",
    default=False,
    help="Use minimalistic templates for unit tests",
)
parser.add_argument(
    "-n",
    "--no-id-checks",
    dest="no_id_checks",
    action="store_true",
    default=False,
    help="Do not generate particle struct ID debugging checks",
)
parser.add_argument(
    "-a",
    "--no-manual-align",
    dest="no_manual_align",
    action="store_true",
    default=False,
    help="Align each particle data struct equally using SWIFT_STRUCT_ALIGN",
)
parser.add_argument(
    "-e",
    "--explicit-var-accessors",
    dest="explicit_var_accessors",
    action="store_true",
    default=False,
    help="Generate getters/setters using explicitly passed argument pointer",
)
parser.add_argument(
    "-g",
    "--global-var-accessors",
    dest="global_var_accessors",
    action="store_true",
    default=False,
    help="Generate getters/setters using global variable pointer",
)
parser.add_argument(
    "-p",
    "--part-struct-accessors",
    dest="part_struct_accessors",
    action="store_true",
    default=False,
    help="Generate getters/setters using part struct accessors",
)

if __name__ == "__main__":

    args = parser.parse_args()
    verbose = args.verbose
    swift_header = args.swift
    outdir = check_output_directory(args.output_dir)
    dry_run = args.dry_run
    input_file = args.input_file
    testing = args.testing
    id_checks = not args.no_id_checks
    manual_align = not args.no_manual_align
    explicit_var_accessors = args.explicit_var_accessors
    global_var_accessors = args.global_var_accessors
    part_struct_accessors = args.part_struct_accessors
    verify_file_exists(input_file)

    if (
        (not part_struct_accessors)
        and (not global_var_accessors)
        and (not explicit_var_accessors)
    ):
        raise ValueError(
            "Neither '--part-struct-accessors' nor "
            + "'--global-var-accessors' nor "
            + "'--explicit-var-accessors' selected."
            + " You must select at least one."
        )

    # read in data
    particle_fields_d, metadata_d = read_input_file(input_file)

    # add required auxiliary fields
    particle_fields_d = add_auxiliary_fields(
        particle_fields_d,
        id_checks=id_checks,
        part_struct_accessors=part_struct_accessors,
        verbose=verbose,
    )

    # check that everything is sensible
    validate_yml_contents(particle_fields_d)
    process_yml_metadata(metadata_d, swift_header=swift_header, verbose=verbose)

    if (len(particle_fields_d.keys()) == 1) and id_checks:
        print("Have only a single particle struct. Skipping accessor ID checks.")
        id_checks = False

    hydro_part_header = generate_hydro_part_dot_h(
        particle_fields_d,
        metadata_d,
        swift_header=swift_header,
        id_checks=id_checks,
        manual_align=manual_align,
        explicit_var_accessors=explicit_var_accessors,
        global_var_accessors=global_var_accessors,
        part_struct_accessors=part_struct_accessors,
        testing=testing,
        verbose=verbose,
    )

    parts_header = generate_parts_dot_h(
        particle_fields_d,
        swift_header=swift_header,
        id_checks=id_checks,
        verbose=verbose,
    )

    hydro_part_arrays_struct_header = generate_hydro_part_arrays_struct_dot_h(
        particle_fields_d, swift_header=swift_header, verbose=verbose
    )

    hydro_part_arrays_flush_header = generate_hydro_part_arrays_flush_dot_h(
        particle_fields_d, swift_header=swift_header, verbose=verbose
    )

    hydro_space_base = generate_hydro_space_dot_c(
        particle_fields_d, metadata_d, swift_header=swift_header, verbose=verbose
    )


    if dry_run:
        print_separator("hydro_part_header")
        print(hydro_part_header)
        print_separator("parts_header")
        print(parts_header)
        print_separator("hydro_part_arrays_struct_header")
        print(hydro_part_arrays_struct_header)
        print_separator("hydro_part_arrays_flush_header")
        print(hydro_part_arrays_struct_header)
        print_separator("hydro_space_base")
        print(hydro_space_base)
    else:

        if swift_header:

            outfile = os.path.join(outdir, "hydro_part.h")
            fp = open(outfile, "w")
            fp.write(hydro_part_header)
            fp.close()
            print("Written", outfile)

            outfile = os.path.join(outdir, "hydro_part_arrays_struct.h")
            fp = open(outfile, "w")
            fp.write(hydro_part_arrays_struct_header)
            fp.close()
            print("Written", outfile)

            outfile = os.path.join(outdir, "hydro_space.c")
            fp = open(outfile, "w")
            fp.write(hydro_space_base)
            fp.close()
            print("Written", outfile)


        else:

            outfile = os.path.join(outdir, "hydro_part.h")
            fp = open(outfile, "w")
            fp.write(hydro_part_header)
            fp.close()
            print("Written", outfile)

            outfile = os.path.join(outdir, "hydro_part_arrays_struct.h")
            fp = open(outfile, "w")
            fp.write(hydro_part_arrays_struct_header)
            fp.close()
            print("Written", outfile)

            outfile = os.path.join(outdir, "parts.h")
            fp = open(outfile, "w")
            fp.write(parts_header)
            fp.close()
            print("Written", outfile)

            outfile = os.path.join(outdir, "hydro_part_arrays_flush.h")
            fp = open(outfile, "w")
            fp.write(hydro_part_arrays_flush_header)
            fp.close()
            print("Written", outfile)
