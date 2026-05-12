#!/bin/bash

python3 ../utils/generate_cell_size_experiment.py -n 64
python3 ../utils/generate_cell_size_experiment.py -n 128
python3 ../utils/generate_cell_size_experiment.py -n 512
python3 ../utils/generate_cell_size_experiment.py -n 1024
python3 ../utils/generate_cell_size_experiment.py -n 2048
