#!/bin/bash

export OMP_NUM_THREADS=4
likwid-pin -c M1:0-4 ../swiftgpupack ../../data/IntelXeonGold5218_Gresho64_4threads


