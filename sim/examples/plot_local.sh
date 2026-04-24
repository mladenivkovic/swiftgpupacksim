#!/bin/bash

# loop_splitting_compare_part_access_local-legion_EAGLE12_72threads.png
# loop_splitting_compare_part_access_local-legion_Gresho256_72threads.png
python3 ../../utils/loop_splitting_compare_particle_access.py --local-legion --png local-legion

# loop_splitting_compare_part_access_local-legion_Gresho256_72threads_normalised.png
# loop_splitting_compare_part_access_local-legion_EAGLE12_72threads_normalised.png
python3 ../../utils/loop_splitting_compare_particle_access.py --local-legion --png --normalise local-legion

# loop_splitting_compare_threads_local-legion_Gresho256_part-struct.png
# loop_splitting_compare_threads_local-legion_Gresho256_explicit-var.png
# loop_splitting_compare_threads_local-legion_Gresho256_global-var.png
# loop_splitting_compare_threads_local-legion_EAGLE12_part-struct.png
# loop_splitting_compare_threads_local-legion_EAGLE12_explicit-var.png
# loop_splitting_compare_threads_local-legion_EAGLE12_global-var.png
python3 ../../utils/loop_splitting_compare_nthreads.py --local-legion --png local-legion

#TODO: UPDATE THIS WITH NEW DATA
# loop_splitting_compare_packed_local-legion_Gresho256_part-struct.png
# loop_splitting_compare_packed_local-legion_EAGLE12_part-struct.png
python3  ../../utils/loop_splitting_compare_packed.py --local-legion --png local-legion


# loop_splitting_compare_vector_local-legion_Gresho256_part-struct.png
# loop_splitting_compare_vector_local-legion_Gresho256_explicit-var.png
# loop_splitting_compare_vector_local-legion_Gresho256_global-var.png
# loop_splitting_compare_vector_local-legion_EAGLE12_part-struct.png
# loop_splitting_compare_vector_local-legion_EAGLE12_explicit-var.png
# loop_splitting_compare_vector_local-legion_EAGLE12_global-var.png
python3 ../../utils/loop_splitting_compare_vector.py --local-legion --png local-legion

