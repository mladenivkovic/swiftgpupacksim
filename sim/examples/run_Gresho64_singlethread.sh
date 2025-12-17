#!/bin/bash

export OMP_NUM_THREADS=1
export OMP_PROC_BIND=true
likwid-pin -c M0:1 ../swiftgpupack ../../data/IntelXeonGold5218_Gresho64_1thread


