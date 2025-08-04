#!/bin/bash

# Run functional tests on the header generation:
# - Generate different cases
# - Compare with expected output (save for whitespace differences)
# - Compile and run simple program to ensure you're producing valid C-code

set -e
rm -f ./hydro_part.h ./hydro_part_data_struct.h


function COMPILE_AND_RUN() {
  # Compile the test program.
  if [[ -z "${CC}" ]]; then
    CC=gcc
  fi
  "$CC" test_header_output.c -o test_header_output.o -Wall -Werror
  ./test_header_output.o
  rm -f ./test_header_output.o
}


function DIFF(){
  # Run a diff on files provided as args 1 and 2.
  diff --ignore-all-space --ignore-blank-lines "$1" "$2"
}


echo "running test_data_types"
python3 ../py/generate_hydro_part.py --test ./input/test_data_types.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_data_types.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h


echo "running test_arrays"
python3 ../py/generate_hydro_part.py --test ./input/test_arrays.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_arrays.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h


echo "running test_ifdefs"
python3 ../py/generate_hydro_part.py --test ./input/test_ifdefs.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_ifdefs.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h

echo "running test_struct"
python3 ../py/generate_hydro_part.py --test ./input/test_struct.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_struct.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h

echo "running test_union"
python3 ../py/generate_hydro_part.py --test ./input/test_union.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_union.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h

echo "running test_split_struct"
python3 ../py/generate_hydro_part.py --test ./input/test_split_struct.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_split_struct.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h

echo "running test_split_struct_nopart"
python3 ../py/generate_hydro_part.py --test ./input/test_split_struct_nopart.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_split_struct_nopart.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h

echo "running test_split_struct_arrays"
python3 ../py/generate_hydro_part.py --test ./input/test_split_struct_arrays.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_split_struct_arrays.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h

echo "running test_split_struct_structs_and_unions"
python3 ../py/generate_hydro_part.py --test ./input/test_split_struct_structs_and_unions.yml
COMPILE_AND_RUN
DIFF hydro_part.h output/test_split_struct_structs_and_unions.h
rm -f ./hydro_part.h ./hydro_part_data_struct.h
