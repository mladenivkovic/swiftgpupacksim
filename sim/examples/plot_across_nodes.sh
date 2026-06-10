#!/bin/bash

set -e

PNGFLAw=""
# PNGFLAG="--png"

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



python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --dine-grace
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --intel
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --dine-grace-gn001
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --dine-grace       --eagle
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --intel            --eagle
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --dine-grace-gn001 --eagle
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --dine-grace       --all
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --intel            --all
python3 ../../utils/plot_GPU_timers.py ./data/timers --by-operation $PNGFLAG --dine-grace-gn001 --all

# compare_part_access_total_across_nodes_noflush_normalised.png
# python3 ../../utils/compare_particle_access_total_time_across_nodes.py --no-flush --normalise $PNGFLAG
#
# python3 ../../utils/compare_cellsize_tests_across_nodes.py --no-flush
