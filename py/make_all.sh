#!/bin/bash

python3 ./generate_hydro_part.py -o ../sim/src/realisations/aos input/SPHENIX.yml
python3 ./generate_hydro_part.py -o ../sim/src/realisations/pack_gradient input/SPHENIX_pack_gradient.yml
python3 ./generate_hydro_part.py -o ../sim/src/realisations/pack_shared input/SPHENIX_pack_shared.yml
python3 ./generate_hydro_part.py -o ../sim/src/realisations/soa input/SPHENIX_SOA.yml
python3 ./generate_hydro_part.py -o ../sim/src/realisations/upstream/ input/SPHENIX_upstream.yml

