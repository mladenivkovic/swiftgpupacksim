#!/bin/bash

export OMP_NUM_THREADS=6
export OMP_PROC_BIND=true
../swiftgpupack ../../data/IntelCoffeeLake_Gresho128_6threads --noflush


