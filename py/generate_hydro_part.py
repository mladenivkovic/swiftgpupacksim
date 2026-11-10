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
    generate_timestep_limiter_struct_dot_h,
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
    help="Generate output compatible for swift, not for swiftgpupacksim",
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
    help="Directory to write output into. If --full or --swift options are selected, it is assumed that this is the top level directory which contains `/src`.",
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
    default=True,
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
parser.add_argument(
    "-f",
    "--full",
    dest="full",
    action="store_true",
    default=False,
    help="Generate full output, i.e. not just files for src/hydro/<flavour>, and corresponding subdirectories",
)


if __name__ == "__main__":

    args = parser.parse_args()
    verbose = args.verbose
    swift_header = args.swift
    input_file = args.input_file
    explicit_var_accessors = args.explicit_var_accessors
    global_var_accessors = args.global_var_accessors
    part_struct_accessors = args.part_struct_accessors
    dry_run = args.dry_run
    full_output = args.full
    testing = args.testing
    id_checks = not args.no_id_checks
    manual_align = not args.no_manual_align

    outdir = check_output_directory(args.output_dir, swift_header=swift_header, full_output=full_output, verbose=verbose)

    # Check our setup is sensible.
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

    # advertise what we're doing.
    accessor_type = "global-var"
    if explicit_var_accessors:
        accessor_type = "explicit-var"
    elif part_struct_accessors:
        accessor_type = "part-struct"
    print("Starting file generation")
    print(f"Output directory:                 {outdir}")
    print(f"Particle input file:              {input_file}")
    print(f"Accessor type:                    {accessor_type}")
    print(f"Dry run?                          {dry_run}")
    print(f"Generate files besides src/hydro? {full_output}")
    print(f"Outputs for unit tests?           {testing}")
    print(f"Add part ID checks in accessor?   {id_checks}")
    print(f"Manual struct alignment?          {manual_align}")

    # read in data
    particle_fields_d, metadata_d = read_input_file(input_file)

    timestep_limiter_input_file = os.path.join("input", "timestep_limiter_struct.yml")
    timestep_limiter_d, timestep_limiter_metadata_d = read_input_file(timestep_limiter_input_file)

    # add required auxiliary fields to particle structs
    particle_fields_d = add_auxiliary_fields(
        particle_fields_d,
        id_checks=id_checks,
        part_struct_accessors=part_struct_accessors,
        verbose=verbose,
    )

    # check again that everything is sensible
    validate_yml_contents(particle_fields_d)
    process_yml_metadata(metadata_d, swift_header=swift_header, verbose=verbose)

    validate_yml_contents(timestep_limiter_d)
    process_yml_metadata(timestep_limiter_metadata_d, swift_header=swift_header, verbose=verbose)

    if (len(particle_fields_d.keys()) == 1) and id_checks:
        print("Have only a single particle struct. Skipping accessor ID checks.")
        id_checks = False

    # generate src/hydro/<flavour>/hydro_part.h
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


    # generate src/hydro_part.h
    #  top_level_hydro_part_header = generate_top_level_hydro_part_dot_h(
    #      particle_fields_d,
    #      metadata_d,
    #      swift_header=swift_header,
    #      id_checks=id_checks,
    #      manual_align=manual_align,
    #      explicit_var_accessors=explicit_var_accessors,
    #      global_var_accessors=global_var_accessors,
    #      part_struct_accessors=part_struct_accessors,
    #      testing=testing,
    #      verbose=verbose,
    #  )


    # generate src/timestep_limiter_struct.h
    timestep_limiter_struct_header = generate_timestep_limiter_struct_dot_h(
        timestep_limiter_d,
        timestep_limiter_metadata_d,
        swift_header=swift_header,
        explicit_var_accessors=explicit_var_accessors,
        global_var_accessors=global_var_accessors,
        part_struct_accessors=part_struct_accessors,
        verbose=verbose,
    )


    # generate src/hydro/<flavour>/parts.h
    parts_header = generate_parts_dot_h(
        particle_fields_d,
        metadata_d,
        swift_header=swift_header,
        id_checks=id_checks,
        verbose=verbose,
    )

    # generate src/hydro/<flavour>/hydro_part_arrays.h
    hydro_part_arrays_struct_header = generate_hydro_part_arrays_struct_dot_h(
        particle_fields_d,
        metadata_d,
        swift_header=swift_header,
        verbose=verbose
    )

    # generate src/hydro/<flavour>/hydro_part_arrays_flush.h
    # (Only really used in swiftgpupacksim)
    hydro_part_arrays_flush_header = generate_hydro_part_arrays_flush_dot_h(
        particle_fields_d,
        metadata_d,
        swift_header=swift_header,
        verbose=verbose
    )

    # TODO: Replace with hydro_data.h
    #  hydro_space_base = generate_hydro_space_dot_c(
    #      particle_fields_d, metadata_d, swift_header=swift_header, verbose=verbose
    #  )

    if dry_run:
        # ToDo: add new files here
        print_separator("hydro_part_header")
        print(hydro_part_header)
        print_separator("parts_header")
        print(parts_header)
        print_separator("hydro_part_arrays_struct_header")
        print(hydro_part_arrays_struct_header)
        print_separator("hydro_part_arrays_flush_header")
        print(hydro_part_arrays_struct_header)
        #  print_separator("hydro_space_base")
        #  print(hydro_space_base)
    else:

        outdir_particle = outdir

        if swift_header or full_output:
            outdir_particle = os.path.join(outdir, "hydro", metadata_d["flavour"])

        if not os.path.exists(outdir_particle):
            os.makedirs(outdir_particle)

        # First, files that go into hydro/<flavour>/
        outfile = os.path.join(outdir_particle, "hydro_part.h")
        fp = open(outfile, "w")
        fp.write(hydro_part_header)
        fp.close()
        print("Written", outfile)

        outfile = os.path.join(outdir_particle, "hydro_part_arrays_struct.h")
        fp = open(outfile, "w")
        fp.write(hydro_part_arrays_struct_header)
        fp.close()
        print("Written", outfile)

        outfile = os.path.join(outdir_particle, "parts.h")
        fp = open(outfile, "w")
        fp.write(parts_header)
        fp.close()
        print("Written", outfile)

        if not swift_header:
            # only needed in swiftgpupacksim
            outfile = os.path.join(outdir_particle, "hydro_part_arrays_flush.h")
            fp = open(outfile, "w")
            fp.write(hydro_part_arrays_flush_header)
            fp.close()
            print("Written", outfile)


        # are we writing full output?
        if full_output:

            # TODO: does any of this need to be moved outside of --full?

            outfile = os.path.join(outdir, "timestep_limiter_struct.h")
            fp = open(outfile, "w")
            fp.write(timestep_limiter_struct_header)
            fp.close()
            print("Written", outfile)

            #  outfile = os.path.join(outdir, "hydro_part.h")
            #  fp = open(outfile, "w")
            #  fp.write(top_level_hydro_part_header)
            #  fp.close()
            #  print("Written", outfile)

            #  outfile = os.path.join(outdir, "hydro_space.c")
            #  fp = open(outfile, "w")
            #  fp.write(hydro_space_base)
            #  fp.close()
            #  print("Written", outfile)
