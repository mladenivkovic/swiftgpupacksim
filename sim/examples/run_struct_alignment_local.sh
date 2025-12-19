#!/bin/bash

NODE=local
# NODE=gn001
# NODE=gn002

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

for align in 1 2 4 8 16 32 64; do

  for dir in IntelXeonGold5218_Gresho64_4threads; do

    mkdir -p $NODE
    mkdir -p $NODE/"$dir"_struct_align"$align"
    mkdir -p $NODE/"$dir"_struct_align"$align"_noflush

    NTHREADS=4

    for ex in ../swiftgpupack_aos ../swiftgpupack_soa ../swiftgpupack_soa-modified ../swiftgpupack_upstream; do

      "$ex"_struct_align_"$align" ../../data/$dir
      mv results_*.csv $NODE/"$dir"_struct_align"$align"/

      "$ex"_struct_align_"$align" ../../data/$dir --noflush
      mv results_*.csv $NODE/"$dir"_struct_align"$align"_noflush/

    done

  done

done
