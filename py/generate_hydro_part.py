#!/usr/bin/env python3

import argparse
import os
import yaml

from utils import _verify_file_exists
from hydro_part_header import generate_single_part_struct, generate_split_part_structs


parser = argparse.ArgumentParser(description="Generate the hydro_part.h file.")
parser.add_argument("input_file")
parser.add_argument("-v", "--verbose", dest="verbose", action="store_true")



if __name__ == "__main__":
    print("Hello world!")

    args = parser.parse_args()
    verbose = args.verbose
    input_file = args.input_file
    _verify_file_exists(input_file)

    input_fp = open(input_file, "r")
    #  particle_fields_d = yaml.load(input_fp, Loader=yaml.Loader)
    particle_fields_d = yaml.safe_load(input_fp)
    input_fp.close()

    if len(particle_fields_d.keys()) == 1:
        generate_single_part_struct(particle_fields_d)
    else:
        generate_split_part_structs(particle_fields_d)


