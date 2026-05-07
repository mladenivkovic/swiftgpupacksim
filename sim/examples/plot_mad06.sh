#!/bin/bash

# compare_flush_variants_mad06_explicit-var_none.png
../../utils/compare_flush_variants.py --png -t 128 mad06

# compare_flush_variants_mad06_explicit-var_none_normalised.png
../../utils/compare_flush_variants.py --png -t 128 mad06 --normalise

# loop_splitting_compare_packed_mad06_Gresho256_part-struct_noflush.png
# loop_splitting_compare_packed_mad06_Gresho256_explicit-var_noflush.png
# loop_splitting_compare_packed_mad06_Gresho256_global-var_noflush.png
# loop_splitting_compare_packed_mad06_EAGLE25_part-struct_noflush.png
# loop_splitting_compare_packed_mad06_EAGLE25_explicit-var_noflush.png
# loop_splitting_compare_packed_mad06_EAGLE25_global-var_noflush.png
python3 ../../utils/compare_packed.py --png mad06 --no-flush -t 128


