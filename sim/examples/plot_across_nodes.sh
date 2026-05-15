#!/bin/bash

# compare_part_access_total_across_nodes_noflush_normalised.png
# python3 ../../utils/compare_particle_access_total_time_across_nodes.py --png --no-flush --normalise
python3 ../../utils/compare_particle_access_total_time_across_nodes.py --no-flush --normalise

python3 ../../utils/compare_cellsize_tests_across_nodes.py --no-flush
