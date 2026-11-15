#!/bin/bash

# Run functional tests on the header generation:
# - Generate different cases
# - Compare with expected output for part-struct accessor (save for whitespace differences)
# - Compile and run simple program for all accessors to ensure you're producing valid C-code
#
# These tests require a C compiler. Specify which one to use using the ${CC} environment variable.

set -eo pipefail

errmsg="
Run tests for SWIFTPARTGEN.
Note: A part of these tests is to check whether full particle header outputs are
valid C code. To do so, we need a path to a SWIFT or SWIFTGPUPACKSIM directory
in order to include other needed headers.
This script will try to find them in default locations, i.e. assuming it's being
used as part of a git submodule within SWIFT or SWIFTGPUPACKSIM. If that is not
the case, you need to provide a path to one of those using the '-i' flag.
Alternatively, specify the SWIFTSRCDIR environment variable before running this
script, e.g.

  SWIFTSRCDIR=/path/to/swiftsim ./pytests.sh

Options:

-s, --swift :                 When testing whether particle file outputs are
                              valid C code, write SWIFT headers.
-g, --swiftgpupacksim :       When testing whether particle file outputs are
                              valid C code, write SWIFTGPUPACKSIM headers.
-i <path>, --include <path> : path to a SWIFT or SWIFTGPUPACKSIM directory.
-h, --help :                  show this message and exit.
"

USE_SWIFT_HEADERS="false"
USE_SWIFTGPUPACKSIM_HEADERS="false"
INCDIR=""

# Get cmdlineargs.
while [[ $# > 0 ]]; do
  arg="$1"

  case $arg in
    -s | --swift)
      USE_SWIFT_HEADERS="true"
    ;;

    -g | --swiftgpupacksim)
      USE_SWIFTGPUPACKSIM_HEADERS="true"
    ;;

    -h | --help)
      echo -e "$errmsg"
      exit
    ;;

    -i | --include)
      INCDIR=$2
      shift
    ;;

    *)
    echo "Unknown argument:" $arg
    echo "Use -h or --help for help."
    exit
    ;;
  esac

  shift
done


# Check and process inputs

if [ "$USE_SWIFT_HEADERS" == "true" -a "$USE_SWIFTGPUPACKSIM_HEADERS" == "true" ]; then
  echo "Error: Can't run with -s and -g simultaneously. Pick only one."
  echo "Use -h or --help for help."
  exit
fi

if [ "$USE_SWIFT_HEADERS" == "false" -a "$USE_SWIFTGPUPACKSIM_HEADERS" == "false" ]; then
  echo "Error: Can't run without either -s and -g. You need to pick one."
  echo "Use -h or --help for help."
  exit
fi


if [ "$USE_SWIFT_HEADERS" == "true" ]; then

  HEADER_TYPE_FLAG="--swift"
  OUTPUT_PREFIX="src/hydro/NO_FLAVOUR_SPECIFIED"
  DIFF_PREFIX="output/swiftsim"

  if [ "$INCDIR" == "" ]; then
    # is variable defined?
    if [[ -z ${SWIFTSRCDIR} ]]; then
      # try to find relative path assuming we're in the git submodule
      echo here1
      if [ -d ../../../swiftsim/src ]; then
        INCDIR=../../../swiftsim
        echo here2
      else
        echo "Error: Didn't find a swiftsim dir while running with -s."
        echo "Specify a path using '-i' flag."
        exit 1
      fi
    else
      INCDIR=${SWIFTSRCDIR}
    fi
  fi

  echo $INCDIR

  if [ ! -d "${INCDIR}" ]; then
    echo "Error: Didn't find directory ${INCDIR}"
  fi
  PARTINCLUDES="-I${INCDIR} -I${INCDIR}/src"
fi

if [ "$USE_SWIFTGPUPACKSIM_HEADERS" == "true" ]; then

  HEADER_TYPE_FLAG=""
  OUTPUT_PREFIX="."
  DIFF_PREFIX="output/swiftgpupacksim"

  if [ "$INCDIR" == "" ]; then
    # is variable defined?
    if [[ -z ${SWIFTSRCDIR} ]]; then
      # try to find relative path assuming we're in the git submodule
      if [ -d ../../../swiftgpupacksim/sim/src ]; then
        INCDIR=../../../swiftgpupacksim/sim
      else
        echo "Error: Didn't find a swiftgpupacksim dir while running with -g."
        echo "Specify a path using '-i' flag."
        exit 1
      fi
    else
      INCDIR=${SWIFTSRCDIR}
    fi
  fi

  if [ ! -d "${INCDIR}" ]; then
    echo "Error: Didn't find directory ${INCDIR}"
  fi
  PARTINCLUDES="-I${INCDIR}/src -I${INCDIR}/src/swift_placeholders"
fi




# ---------------------------------------
# HELPER FUNCTIONS
# ---------------------------------------


function COMPILE_AND_RUN() {
  # Compile and run the test program.

  # try a default compiler
  if [[ -z "${CC}" ]]; then
    CC="gcc -fopenmp"
  fi

  echo COMPILE_AND_RUN: compiling
  # we need -DUSE_PART_STRUCT_ACCESSORS for certain APIs which access cell particle arrays directly to compile
  echo $CC -DUSE_PART_STRUCT_ACCESSORS test_header_output.c -o test_header_output.o -Wall -Werror $PARTINCLUDES
  $CC -DUSE_PART_STRUCT_ACCESSORS test_header_output.c -o test_header_output.o -Wall -Werror $PARTINCLUDES

  echo COMPILE_AND_RUN: running
  ./test_header_output.o

  rm -f ./test_header_output.o

  echo COMPILE_AND_RUN: done
}


function COMPILE_AND_RUN_PARTICLE() {
  # Compile and run the test program.
  # Usage:
  # COMPILE_AND_RUN_PARTICLE -D<flavour-name>
  # where -D<flavour-name> will be used to define the -D<FLAVOUR> macro

  FLAVOURFLAG=$1

  # try a default compiler
  if [[ -z "${CC}" ]]; then
    CC="gcc -fopenmp"
  fi

  echo COMPILE_AND_RUN_PARTICLE: compiling
  # we need -DUSE_PART_STRUCT_ACCESSORS for certain APIs which access cell particle arrays directly to compile
  # $CC -DUSE_PART_STRUCT_ACCESSORS ${FLAVOURFLAG} test_particle_flavour.c -o test_particle_flavour.o -Wall -Werror $PARTINCLUDES
  echo $CC -DUSE_PART_STRUCT_ACCESSORS -DRT_NONE ${FLAVOURFLAG} test_particle_flavour.c -o test_particle_flavour.o -Wall -Werror -I./src $PARTINCLUDES
  $CC -DUSE_PART_STRUCT_ACCESSORS -DRT_NONE ${FLAVOURFLAG} test_particle_flavour.c -o test_particle_flavour.o -Wall -Werror -I. -I./src $PARTINCLUDES

  echo COMPILE_AND_RUN_PARTICLE: running
  ./test_particle_flavour.o

  rm -f ./test_particle_flavour.o

  echo COMPILE_AND_RUN_PARTICLE: done
}



function DIFF(){
  # Run a diff on files provided as args 1 and 2.
  echo DIFF "$1" "$2"
  diff --ignore-all-space --ignore-blank-lines --color "$1" "$2"
}


# ---------------------------------------
# TEST API GENERATION
# ---------------------------------------

# cleanup before we start
rm -rf ./hydro_part.h ./hydro_part_arrays_struct.h ./test_header_output.o ./src

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

    python3 ../../py/generate_hydro_part.py --test "$flag" $HEADER_TYPE_FLAG ./input/"$testcase".yml
    COMPILE_AND_RUN

    if [ "$flag" == "--part-struct-accessor" ]; then
      # I only keep the outputs for part struct accessors for comparison for now...
      # cp "$OUTPUT_PREFIX"/hydro_part.h "$DIFF_PREFIX"/"$testcase".h
      DIFF "$DIFF_PREFIX"/"$testcase".h "$OUTPUT_PREFIX"/hydro_part.h
    fi

    rm -f ./hydro_part.h ./hydro_part_arrays_struct.h parts.h
  done

done # loop over accessor flags


# ---------------------------------------
# TEST HEADER INCLUSION
# ---------------------------------------

# testing header inclusion:
# we're not generating particle API here, so no point in testing all accessor methods.

# make temporary headers which will be included
touch a.h b.h c.h

for testcase in test_headerfile_includes test_headerfile_includes_add; do

  echo "==============================================="
  echo "running $testcase $flag"
  echo "==============================================="

  python3 ../../py/generate_hydro_part.py --part-struct-accessors --test $HEADER_TYPE_FLAG ./input/"$testcase".yml
  COMPILE_AND_RUN

  # cp "$OUTPUT_PREFIX"/hydro_part.h "$DIFF_PREFIX"/"$testcase".h
  DIFF "$DIFF_PREFIX"/"$testcase".h "$OUTPUT_PREFIX"/hydro_part.h

done

rm -f a.h b.h c.h



# ---------------------------------------
# TEST FULL PARTICLE OUTPUT COMPILES
# ---------------------------------------

declare -A SPH_FLAVOURS
SPH_FLAVOURS["SPHENIX"]="SPHENIX_SPH"
# SPH_FLAVOURS["None"]="NONE_SPH"
# SPH_FLAVOURS["MINIMAL"]="MINIMAL_SPH"
# SPH_FLAVOURS["GADGET2"]="GADGET2_SPH"
# SPH_FLAVOURS["PressureEntropy"]="HOPKINS_PE_SPH"
# SPH_FLAVOURS["PressureEnergy"]="HOPKINS_PU_SPH"
# SPH_FLAVOURS["PressureEnergyMonaghan"]="HOPKINS_PU_SPH_MONAGHAN"
# SPH_FLAVOURS["Phantom"]="PHANTOM_SPH"
# SPH_FLAVOURS["Gizmo_MFV"]="GIZMO_MFV_SPH"
# SPH_FLAVOURS["Gizmo_MFM"]="GIZMO_MFM_SPH"
# SPH_FLAVOURS["Shadowswift"]="SHADOWSWIFT"
# SPH_FLAVOURS["Planetary"]="PLANETARY_SPH"
# SPH_FLAVOURS["REMIX"]="REMIX_SPH"
# SPH_FLAVOURS["SPHENIX"]="SPHENIX_SPH"
# SPH_FLAVOURS["Gasoline"]="GASOLINE_SPH"
# SPH_FLAVOURS["AnarchyPU"]="ANARCHY_PU_SPH"



for FLAVOUR in ${!SPH_FLAVOURS[@]}; do
  for ACCESSOR_FLAG in \
    "--part-struct-accessors" \
    "--explicit-var-accessors" \
    "--global-var-accessors" "--part-struct-accessors --explicit-var-accessors --global-var-accessors" \
  ; do
    echo RUNNING FOR $FLAVOUR $ACCESSOR_FLAG
    rm -rf src/*
    echo python3 ../../py/generate_hydro_part.py $ACCESSOR_FLAG --full $HEADER_TYPE_FLAG ../input/"$FLAVOUR".yml
    python3 ../../py/generate_hydro_part.py $ACCESSOR_FLAG --full $HEADER_TYPE_FLAG ../input/"$FLAVOUR".yml
    COMPILE_AND_RUN_PARTICLE -D${SPH_FLAVOURS[$FLAVOUR]}
  done
done


echo "Python tests passed."
