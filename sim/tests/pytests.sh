#!/bin/bash

# Run functional tests on the header generation:
# - Generate different cases
# - Compare with expected output (save for whitespace differences)
# - Compile and run simple program to ensure you're producing valid C-code

set -e
rm -f ./hydro_part.h ./hydro_part_arrays_struct.h


function COMPILE_AND_RUN() {
  # Compile the test program.
  if [[ -z "${CC}" ]]; then
    CC=gcc
  fi
  "$CC" -DUSE_PART_STRUCT_ACCESSORS test_header_output.c -o test_header_output.o -Wall -Werror
  echo TODO MLADEN: THIS SHOULD ALSO WORK WITHOUT -DUSE_PART_STRUCT_ACCESSORS
  ./test_header_output.o
  rm -f ./test_header_output.o
}


function DIFF(){
  # Run a diff on files provided as args 1 and 2.
  echo DIFF "$1" "$2"
  diff --ignore-all-space --ignore-blank-lines --color "$1" "$2"
}



for flag in "--part-struct-accessor" "--global-var-accessor" "--explicit-var-accessor"; do
  for testcase in \
    "test_data_types" \
    "test_arrays" \
    "test_multidim_arrays" \
    "test_ifdefs" \
    "test_struct" \
    "test_union" \
    "test_split_struct" \
    "test_split_struct_nopart" \
    "test_split_struct_arrays" \
    "test_split_struct_structs_and_unions" \
  ; do

    echo "==============================================="
    echo "running $testcase $flag"
    echo "==============================================="

    python3 ../../py/generate_hydro_part.py --test "$flag" ./input/"$testcase".yml
    COMPILE_AND_RUN

    if [ "$flag" == "--part-struct-accessor" ]; then
      # I only keep the outputs for part struct accessors for comparison for now...
      DIFF hydro_part.h output/"$testcase".h
    fi

    rm -f ./hydro_part.h ./hydro_part_arrays_struct.h parts.h
  done
done

echo "Python tests passed."
