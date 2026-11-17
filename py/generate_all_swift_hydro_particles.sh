#!/bin/bash

set -eo pipefail

errmsg="
Generate header files for all SWIFT particles, intended to be placed into and
used in the SWIFT repository.

Usage: $0 </path/to/swiftsim/src>

If no </path/to/swiftsim/src> is provided, the files will be generated in the
current directory.

Options:

-h, --help :  show this message and exit.
"

OUTPUTDIR="."

# Get cmdlineargs.
while [[ $# > 0 ]]; do
  arg="$1"

  case $arg in
    -h | --help)
      echo -e "$errmsg"
      exit
    ;;

    *)
    if [ "$OUTPUTDIR" != "." ]; then
      # we've already read in a path
      echo "You provided a second positional cmdline argument, '"$arg"'."
      echo "Don't know what to do with that."
      echo "Use -h or --help for help."
      exit 1
    fi
    if [ ! -d "$arg" ]; then
      echo "Provided path to swiftsim/src dir, '"$arg"', not found."
      echo "Use -h or --help for help."
      exit 1
    else
      OUTPUTDIR="$arg"
    fi
    ;;
  esac

  shift
done


# Now, to the actual work
for FLAVOUR in \
  "None" \
  "Minimal" \
  "Gadget2" \
  "PressureEntropy" \
  "PressureEnergy" \
  "PressureEnergyMorrisMonaghanAV" \
  "Phantom" \
  "Gizmo_MFV" \
  "Gizmo_MFM" \
  "Shadowswift" \
  "Planetary" \
  "AnarchyPU" \
  "Gasoline" \
  "REMIX" \
  "SPHENIX" \
; do

  ./generate_hydro_part.py --global-var-access --swift --full input/hydro/"$FLAVOUR".yml -o "$OUTPUTDIR"

done
