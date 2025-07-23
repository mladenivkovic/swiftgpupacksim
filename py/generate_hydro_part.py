#!/usr/bin/env python3

import argparse
import os
import yaml

from utils import verify_file_exists, check_output_directory
from hydro_part_header import generate_hydro_part_header


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


if __name__ == "__main__":

    args = parser.parse_args()
    verbose = args.verbose
    swift_headers = args.swift
    outdir = check_output_directory(args.output_dir)
    dry_run = args.dry_run
    input_file = args.input_file
    verify_file_exists(input_file)

    input_fp = open(input_file, "r")
    particle_fields_d = yaml.safe_load(input_fp)
    input_fp.close()

    hydro_part_header = generate_hydro_part_header(particle_fields_d, swift_headers, verbose)

    if (dry_run):
        print(hydro_part_header)
    else:
        # TODO: write output to file.
        raise NotImplementedError()

