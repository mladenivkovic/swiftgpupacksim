#!/bin/bash

# pretend we have a SWIFT repository here. Later, when everything's done, we'll
# just replace the SWIFT_PATH variable with an actual path, and won't have to
# manually move around files to their right places.
# mkdir -p swift
# mkdir -p swift/src
# mkdir -p swift/src/hydro
# SWIFT_SRC_HYDRO_PATH=./swift/src/hydro
#
# for SPH_FLAVOUR in AnarchyPU Gadget2 Gasoline Gizmo \
#                    Minimal Phantom Planetary PressureEnergy \
#                    PressureEnergyMorrisMonaghanAV PressureEntropy REMIX SPHENIX;
# do
#   mkdir -p "$SWIFT_SRC_HYDRO_PATH"/"$SPH_FLAVOUR"
# done
#
# -----------------------------------------

SWIFT_SRC_HYDRO_PATH=~/Durham/GPUSWIFT/src/hydro/

# Now, to the actual work

./generate_hydro_part.py --global-var-access --swift input/SPHENIX.yml -o $SWIFT_SRC_HYDRO_PATH/SPHENIX
./generate_hydro_part.py --global-var-access --swift input/AnarchyPU.yml -o $SWIFT_SRC_HYDRO_PATH/AnarchyPU
./generate_hydro_part.py --global-var-access --swift input/Gasoline.yml -o $SWIFT_SRC_HYDRO_PATH/Gasoline
