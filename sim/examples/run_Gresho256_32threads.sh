#!/bin/bash

export OMP_NUM_THREADS=32
likwid-pin -c N:0-32 ../swiftgpupack ../../data/IntelXeonGold5218_Gresho256_32threads


