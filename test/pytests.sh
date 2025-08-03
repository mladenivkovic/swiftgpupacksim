#!/bin/bash

set -e
rm -f hydro_part.h

echo "running test_data_types"
python3 ../py/generate_hydro_part.py --test ./input/test_data_types.yml
diff --ignore-all-space --ignore-blank-lines hydro_part.h output/test_data_types.h
rm -f hydro_part.h


echo "running test_arrays"
python3 ../py/generate_hydro_part.py --test ./input/test_arrays.yml
diff --ignore-all-space --ignore-blank-lines hydro_part.h output/test_arrays.h
rm -f hydro_part.h


