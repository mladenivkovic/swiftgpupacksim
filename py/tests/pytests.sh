#!/bin/bash

# Run functional tests on the header generation:
# - Generate different cases
# - Compare with expected output for part-struct accessor (save for whitespace differences)
# - Compile and run simple program for all accessors to ensure you're producing valid C-code
#
# These tests require a C compiler. Specify which one to use using the ${CC} environment variable.

set -eo pipefail
rm -f ./hydro_part.h ./hydro_part_arrays_struct.h ./test_header_output.o


function COMPILE_AND_RUN() {
  # Compile the test program.
  if [[ -z "${CC}" ]]; then
    CC="gcc -fopenmp"
  fi
  echo COMPILE_AND_RUN: compiling
  $CC -DUSE_PART_STRUCT_ACCESSORS test_header_output.c -o test_header_output.o -Wall -Werror -I../src/ -I../src/swift_placeholders

  echo COMPILE_AND_RUN: running
  ./test_header_output.o

  rm -f ./test_header_output.o

  echo COMPILE_AND_RUN: done
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
      # cp hydro_part.h output/"$testcase".h
      DIFF output/"$testcase".h hydro_part.h
    fi

    rm -f ./hydro_part.h ./hydro_part_arrays_struct.h parts.h
  done

done # loop over accessor flags


# testing header inclusion:
# we're not generating particle API here, so no point in testing all accessor methods.

# make temporary headers which will be included
touch a.h b.h c.h

for testcase in test_headerfile_includes test_headerfile_includes_add; do

  echo "==============================================="
  echo "running $testcase $flag"
  echo "==============================================="

  python3 ../../py/generate_hydro_part.py --part-struct-accessors --test ./input/"$testcase".yml
  COMPILE_AND_RUN

  # cp hydro_part.h output/"$testcase".h
  DIFF output/"$testcase".h hydro_part.h

done

rm -f a.h b.h c.h


echo "Python tests passed."
