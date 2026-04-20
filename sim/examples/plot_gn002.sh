#!/bin/bash

# loop_splitting_compare_part_access_gn002_EAGLE12_72threads.png
python3 ../../utils/loop_splitting_compare_particle_access.py --png gn002 --eagle
# loop_splitting_compare_part_access_gn002_EAGLE12_72threads_normalised.png
python3 ../../utils/loop_splitting_compare_particle_access.py --png gn002 --eagle --normalise

# loop_splitting_compare_part_access_gn002_Gresho256_72threads.png
python3 ../../utils/loop_splitting_compare_particle_access.py gn002 --png
# loop_splitting_compare_part_access_gn002_Gresho256_72threads_normalised.png
python3 ../../utils/loop_splitting_compare_particle_access.py gn002 --png --normalise


