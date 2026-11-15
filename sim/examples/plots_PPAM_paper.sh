#!/bin/bash

# Plots used in the PPAM paper.
# git commit hash of plotting scripts: 1306492c9522057e4b4caa7a07862e43989d66c8

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



# # first version, changed in final submission
# python3 ../../utils/plot_GPU_timers.py ./timer_data --by-operation $PNGFLAG --dine-grace       --eagle

# gpu_timers_singlebar_all-experiments_dine-grace.pdf
python3 ../../utils/plot_GPU_timers_singlebar.py ./timer_data --dine-grace --all-experiments $PNGFLAG

# compare_part_access_total_across_nodes_noflush_normalised.png
python3 ../../utils/compare_particle_access_total_time_across_nodes.py --no-flush --normalise $PNGFLAG


# compare_part_access_dine2_5steps_64threads_noflush_normalised.pdf
python3 ../../utils/compare_particle_access.py --no-flush -t 64 data/dine2_5steps --normalise $PNGFLAG
# python3 ../../utils/compare_particle_access.py --no-flush  data/gn003_5steps --normalise $PNGFLAG

# loop_splitting_compare_part_access_to_no-split_dine2_5steps_64threads_noflush.png
python3 ../../utils/loop_splitting_compare_total_times_compared_to_no_split.py --no-flush -t 64 data/dine2_5steps $PNGFLAG

# loop_splitting_compare_part_access_to_no-split_dine2_5steps_64threads_noflush.png
python3 ../../utils/loop_splitting_compare_total_times_compared_to_no_split.py --no-flush data/gn003_5steps $PNGFLAG


