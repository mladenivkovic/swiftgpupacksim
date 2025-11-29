#!/bin/bash

./generate_hydro_part.py input/SPHENIX.yml -o ../sim/src/realisations/aos
./generate_hydro_part.py input/SPHENIX_SOA.yml -o ../sim/src/realisations/soa
