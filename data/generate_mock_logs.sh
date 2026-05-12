#!/bin/bash

# First attempt with 1 thread
# python3 ../utils/generate_cell_size_experiment.py -n 64
# python3 ../utils/generate_cell_size_experiment.py -n 128
# python3 ../utils/generate_cell_size_experiment.py -n 512
# python3 ../utils/generate_cell_size_experiment.py -n 1024
# python3 ../utils/generate_cell_size_experiment.py -n 2048

# 72 threads for GH
python3 ../utils/generate_cell_size_experiment.py -n 64 -T 72 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 128 -T 72 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 512 -T 72 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 1024 -T 72 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 2048 -T 72 -l 40000

# 64 threads for dine2
python3 ../utils/generate_cell_size_experiment.py -n 64 -T 64 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 128 -T 64 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 512 -T 64 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 1024 -T 64 -l 40000
python3 ../utils/generate_cell_size_experiment.py -n 2048 -T 64 -l 40000
