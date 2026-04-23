#!/bin/bash

# loop_splitting_compare_part_access_gn003_EAGLE12_72threads.png
python3 ../../utils/loop_splitting_compare_particle_access.py --png gn003 --eagle
# loop_splitting_compare_part_access_gn003_EAGLE12_72threads_normalised.png
python3 ../../utils/loop_splitting_compare_particle_access.py --png gn003 --eagle --normalise

# loop_splitting_compare_part_access_gn003_Gresho256_72threads.png
python3 ../../utils/loop_splitting_compare_particle_access.py gn003 --png
# loop_splitting_compare_part_access_gn003_Gresho256_72threads_normalised.png
python3 ../../utils/loop_splitting_compare_particle_access.py gn003 --png --normalise

# compare_experiments_loop_splitting_gn003_part-struct_72threads.png
# compare_experiments_loop_splitting_gn003_explicit-var_72threads.png
# compare_experiments_loop_splitting_gn003_global-var_72threads.png
python3 ../../utils/loop_splitting_compare_experiments.py --png gn003

# loop_splitting_compare_threads_gn003_Gresho256_part-struct.png
# loop_splitting_compare_threads_gn003_Gresho256_explicit-var.png
# loop_splitting_compare_threads_gn003_Gresho256_global-var.png
# loop_splitting_compare_threads_gn003_EAGLE12_part-struct.png
# loop_splitting_compare_threads_gn003_EAGLE12_explicit-var.png
# loop_splitting_compare_threads_gn003_EAGLE12_global-var.png
python3 ../../utils/loop_splitting_compare_nthreads.py --png gn003
