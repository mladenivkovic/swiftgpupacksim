#!/bin/bash

export OMP_NUM_THREADS=4
export OMP_PROC_BIND=true
../swiftgpupack ../../data/IntelXeonGold5218_Gresho64_4threads --noflush -s 1


