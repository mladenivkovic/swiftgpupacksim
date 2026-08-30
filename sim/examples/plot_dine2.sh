#!/bin/bash

set -e

# run script with -p flag to create png images instead of default pdfs
PNGFLAG=""

if [ $# -gt 0 ]; then
  case "$1" in
  -p | -png | --png )
    PNGFLAG="--png"
    ;;
  *)
    echo unknown cmdline param "'""$1""'"
    ;;
  esac
fi


# ---------------------------------------------
# PARTICLE ACCESS
# ---------------------------------------------

# compare_part_access_dine2_5steps_64threads_noflush_normalised.pdf
# python3 ../../utils/compare_particle_access.py --no-flush -t 64 data/dine2_5steps --normalise $PNGFLAG



# ---------------------------------------------
# PARTICLE ACCESS + LOOP SPLITTING
# ---------------------------------------------

# loop_splitting_compare_part_access_to_part-struct_no-split_dine2_5steps_64threads_noflush.png
# python3 ../../utils/loop_splitting_compare_total_times.py --no-flush -t 64 data/dine2_5steps $PNGFLAG

# loop_splitting_compare_part_access_to_no-split_dine2_5steps_64threads_noflush.png
# python3 ../../utils/loop_splitting_compare_total_times_compared_to_no_split.py --no-flush -t 64 data/dine2_5steps $PNGFLAG

# loop_splitting_compare_part_access_dine2_5steps_Gresho256_64threads_noflush.png
# loop_splitting_compare_part_access_dine2_5steps_EAGLE25_64threads_noflush.png
# python3 ../../utils/loop_splitting_compare_particle_access.py data/dine2_5steps --no-flush -t 64 $PNGFLAG


# ---------------------------------------------
# CACHE FLUSHING
# ---------------------------------------------

# compare_flush_variants_dine2_explicit-var_none.png
# ../../utils/compare_flush_variants.py -t 64 data/dine2 $PNGFLAG

# compare_flush_variants_dine2_explicit-var_none_normalised.png
# ../../utils/compare_flush_variants.py -t 64 data/dine2 --normalise $PNGFLAG


# ---------------------------------------------
# FIRST INIT / FIRST TOUCH
# ---------------------------------------------

# compare_firstinit_dine2_explicit-var_none.png
# python3 ../../utils/compare_firstinit.py -t 64 data/dine2 --no-flush $PNGFLAG

# compare_nthreads_dine2_explicit-var_none.png
# python3 ../../utils/compare_nthreads.py data/dine2 --no-flush $PNGFLAG



# ---------------------------------------------------------------------
# PACKING
# ---------------------------------------------------------------------

# loop_splitting_compare_packed_dine2_Gresho256_part-struct_noflush.png
# loop_splitting_compare_packed_dine2_Gresho256_explicit-var_noflush.png
# loop_splitting_compare_packed_dine2_Gresho256_global-var_noflush.png
# loop_splitting_compare_packed_dine2_EAGLE25_part-struct_noflush.png
# loop_splitting_compare_packed_dine2_EAGLE25_explicit-var_noflush.png
# loop_splitting_compare_packed_dine2_EAGLE25_global-var_noflush.png
# python3 ../../utils/loop_splitting_compare_packed.py data/dine2 --no-flush -t 64 $PNGFLAG

# loop_splitting_compare_packed_dine2_global-var_noflush_relative.png
# python3 ../../utils/loop_splitting_compare_packed_relative.py data/dine2 --no-flush -t 64 $PNGFLAG



