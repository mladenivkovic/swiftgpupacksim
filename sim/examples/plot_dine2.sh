#!/bin/bash

# compare_flush_variants_dine2_explicit-var_none.png
../../utils/compare_flush_variants.py --png -t 64 dine2

# compare_flush_variants_dine2_explicit-var_none_normalised.png
../../utils/compare_flush_variants.py --png -t 64 dine2 --normalise

# loop_splitting_compare_packed_dine2_Gresho256_part-struct_noflush.png
# loop_splitting_compare_packed_dine2_Gresho256_explicit-var_noflush.png
# loop_splitting_compare_packed_dine2_Gresho256_global-var_noflush.png
# loop_splitting_compare_packed_dine2_EAGLE25_part-struct_noflush.png
# loop_splitting_compare_packed_dine2_EAGLE25_explicit-var_noflush.png
# loop_splitting_compare_packed_dine2_EAGLE25_global-var_noflush.png
python3 ../../utils/compare_packed.py --png dine2 --no-flush -t 64



