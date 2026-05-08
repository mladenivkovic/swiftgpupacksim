#!/bin/bash

./generate_hydro_part.py -e -g input/SPHENIX.yml -o ../sim/src/realisations/aos
./generate_hydro_part.py -e -g input/SPHENIX_pack_force.yml -o ../sim/src/realisations/pack_force
./generate_hydro_part.py -e -g input/SPHENIX_pack_gradient.yml -o ../sim/src/realisations/pack_gradient
./generate_hydro_part.py -e -g input/SPHENIX_pack_shared.yml -o ../sim/src/realisations/pack_shared
./generate_hydro_part.py -e -g input/SPHENIX_pack_force_type.yml -o ../sim/src/realisations/pack_force_type
./generate_hydro_part.py -e -g input/SPHENIX_SOA.yml -o ../sim/src/realisations/soa
./generate_hydro_part.py -e -g input/SPHENIX_upstream.yml -o ../sim/src/realisations/upstream
./generate_hydro_part.py -e -g input/SPHENIX_random_order.yml -o ../sim/src/realisations/random_order
