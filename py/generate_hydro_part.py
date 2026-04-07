#!/usr/bin/env python3

import argparse
import os
import yaml

from utils import (
    verify_file_exists,
    check_output_directory,
    print_separator,
    validate_yml_contents,
    add_auxiliary_fields,
)
from headers import (
    generate_hydro_part_header,
    generate_parts_header,
    generate_hydro_part_arrays_struct_header,
)


parser = argparse.ArgumentParser(description="Generate the hydro_part.h file.")
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
    verify_file_exists(input_file)

    input_fp = open(input_file, "r")
    particle_fields_d = yaml.safe_load(input_fp)
    input_fp.close()
    particle_fields_d = add_auxiliary_fields(
        particle_fields_d, id_checks=id_checks, verbose=verbose
    )
    validate_yml_contents(particle_fields_d)

    if (len(particle_fields_d.keys()) == 1) and id_checks:
        print("Have only a single particle struct. Skipping accessor ID checks.")
        id_checks = False

    hydro_part_header = generate_hydro_part_header(
        particle_fields_d,
        swift_header=swift_header,
        id_checks=id_checks,
        manual_align=manual_align,
        explicit_var_accessors=explicit_var_accessors,
        global_var_accessors=global_var_accessors,
        testing=testing,
        verbose=verbose,
    )

    parts_header = generate_parts_header(
        particle_fields_d,
        swift_header=swift_header,
        id_checks=id_checks,
        verbose=verbose,
    )

    hydro_part_arrays_struct_header = generate_hydro_part_arrays_struct_header(
        particle_fields_d, swift_header=swift_header, verbose=verbose
    )

    if dry_run:
        print_separator("hydro_part_header")
        print(hydro_part_header)
        print_separator("parts_header")
        print(parts_header)
        print_separator("hydro_part_arrays_struct_header")
        print(hydro_part_arrays_struct_header)
    else:
        outfile = os.path.join(outdir, "hydro_part.h")
        fp = open(outfile, "w")
        fp.write(hydro_part_header)
        fp.close()
        print("Written", outfile)

        outfile = os.path.join(outdir, "parts.h")
        fp = open(outfile, "w")
        fp.write(parts_header)
        fp.close()
        print("Written", outfile)

        outfile = os.path.join(outdir, "hydro_part_arrays_struct.h")
        fp = open(outfile, "w")
        fp.write(hydro_part_arrays_struct_header)
        fp.close()
        print("Written", outfile)
