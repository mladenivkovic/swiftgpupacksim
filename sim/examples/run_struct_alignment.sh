#!/bin/bash

NODE=gn001
# NODE=gn002

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

for align in 1 2 4 8 16 32 64; do

  for dir in EAGLE12_16threads  EAGLE12_32threads  Gresho256_16threads  Gresho256_32threads; do

    mkdir -p $NODE
    mkdir -p $NODE/"$dir"_structalign"$align"
    mkdir -p $NODE/"$dir"_structalign"$align"_noflush

    NTHREADS=16
    if [[ "$dir" == "*_32thread*" ]]; then NTHREADS=32; fi

    for ex in ../swiftgpupack_aos ../swiftgpupack_soa ../swiftgpupack_soa-modified ../swiftgpupack-upstream; do

      likwid-pin -c N:0-$NTHREADS $ex ~/runs_GPUSWIFT/$NODE/measurements_gpupacksim/$dir
      mv results_*.csv $NODE/"$dir"_structalign"$align"

      likwid-pin -c N:0-$NTHREADS $ex ~/runs_GPUSWIFT/$NODE/measurements_gpupacksim/$dir --noflush
      mv results_*.csv $NODE/"$dir"_structalign"$align"_noflush

    done

  done

done
