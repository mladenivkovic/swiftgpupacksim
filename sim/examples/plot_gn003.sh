#!/bin/bash

# compare_part_access_gn003_72threads_noflush_normalised.png
# python3 ../../utils/compare_particle_access.py --png --no-flush gn003 --normalise
python3 ../../utils/compare_particle_access.py --no-flush gn003_5steps --normalise

# loop_splitting_compare_total_time_gn003_72threads_noflush.png
# python3 ../../utils/loop_splitting_compare_total_times.py --png --no-flush gn003
python3 ../../utils/loop_splitting_compare_total_times.py --no-flush gn003_5steps

python3 ../../utils/loop_splitting_compare_total_times_compared_to_no_split.py --no-flush gn003_5steps

# # compare_flush_variants_gn003_explicit-var_none.png
# python3 ../../utils/compare_flush_variants.py --png gn003
#
# # compare_flush_variants_gn003_explicit-var_none_normalised.png
# python3 ../../utils/compare_flush_variants.py --png gn003 --normalise
#
# # loop_splitting_compare_packed_gn003_Gresho256_part-struct_noflush.png
# # loop_splitting_compare_packed_gn003_Gresho256_explicit-var_noflush.png
# # loop_splitting_compare_packed_gn003_Gresho256_global-var_noflush.png
# # loop_splitting_compare_packed_gn003_EAGLE25_part-struct_noflush.png
# # loop_splitting_compare_packed_gn003_EAGLE25_explicit-var_noflush.png
# # loop_splitting_compare_packed_gn003_EAGLE25_global-var_noflush.png
# python3 ../../utils/loop_splitting_compare_packed.py --png gn003 --no-flush
#
# # loop_splitting_compare_packed_gn003_global-var_noflush_relative.png
# python3 ../../utils/loop_splitting_compare_packed_relative.py --png gn003 --no-flush
#
# # TODO
# # compare_firstinit_gn003_explicit-var_none.png
# # ../../utils/compare_firstinit.py --png gn003 --no-flush
#
# # TODO
# # compare_nthreads_gn003_explicit-var_none.png
# # ../../utils/compare_nthreads.py --png gn003 --no-flush

# -------------------------------------------------------------------------------

# # loop_splitting_compare_part_access_gn003_EAGLE12_72threads.png
# # loop_splitting_compare_part_access_gn003_Gresho256_72threads.png
# python3 ../../utils/loop_splitting_compare_particle_access.py gn003 --png
#
# # loop_splitting_compare_part_access_gn003_Gresho256_72threads_normalised.png
# # loop_splitting_compare_part_access_gn003_EAGLE12_72threads_normalised.png
# python3 ../../utils/loop_splitting_compare_particle_access.py gn003 --png --normalise
#
# # compare_experiments_loop_splitting_gn003_part-struct_72threads.png
# # compare_experiments_loop_splitting_gn003_explicit-var_72threads.png
# # compare_experiments_loop_splitting_gn003_global-var_72threads.png
# python3 ../../utils/loop_splitting_compare_experiments.py --png gn003
#
# # loop_splitting_compare_threads_gn003_Gresho256_part-struct.png
# # loop_splitting_compare_threads_gn003_Gresho256_explicit-var.png
# # loop_splitting_compare_threads_gn003_Gresho256_global-var.png
# # loop_splitting_compare_threads_gn003_EAGLE12_part-struct.png
# # loop_splitting_compare_threads_gn003_EAGLE12_explicit-var.png
# # loop_splitting_compare_threads_gn003_EAGLE12_global-var.png
# python3 ../../utils/loop_splitting_compare_nthreads.py --png gn003
#
# #TODO: UPDATE THIS WITH NEW DATA
# # loop_splitting_compare_packed_gn003_Gresho256_part-struct.png
# # loop_splitting_compare_packed_gn003_EAGLE12_part-struct.png
# python3  ../../utils/loop_splitting_compare_packed.py --png gn003
#
#
# # loop_splitting_compare_vector_gn003_Gresho256_part-struct.png
# # loop_splitting_compare_vector_gn003_Gresho256_explicit-var.png
# # loop_splitting_compare_vector_gn003_Gresho256_global-var.png
# # loop_splitting_compare_vector_gn003_EAGLE12_part-struct.png
# # loop_splitting_compare_vector_gn003_EAGLE12_explicit-var.png
# # loop_splitting_compare_vector_gn003_EAGLE12_global-var.png
# python3 ../../utils/loop_splitting_compare_vector.py --png gn003
#
# # loop_splitting_compare_noflush_gn003_Gresho256_part-struct.png
# # loop_splitting_compare_noflush_gn003_Gresho256_explicit-var.png
# # loop_splitting_compare_noflush_gn003_Gresho256_global-var.png
# # loop_splitting_compare_noflush_gn003_EAGLE12_part-struct.png
# # loop_splitting_compare_noflush_gn003_EAGLE12_explicit-var.png
# # loop_splitting_compare_noflush_gn003_EAGLE12_global-var.png
# python3 ../../utils/loop_splitting_compare_noflush.py --png gn003
#
