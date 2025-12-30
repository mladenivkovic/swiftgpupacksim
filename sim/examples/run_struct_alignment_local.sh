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
    mkdir -p $NODE/"$dir"_struct_align"$align"_novector
    mkdir -p $NODE/"$dir"_struct_align"$align"_noflush_novector

    NTHREADS=4

    for ex in ../swiftgpupack_aos ../swiftgpupack_soa ../swiftgpupack_soa-modified ../swiftgpupack_upstream ../swiftgpupack_pack-gradient ../swiftgpupack_pack-force ../swiftgpupack_pack-shared; do

      "$ex"_struct_align_"$align" ../../data/$dir
      mv results_*.csv $NODE/"$dir"_struct_align"$align"/

      "$ex"_struct_align_"$align"_novector ../../data/$dir
      mv results_*.csv $NODE/"$dir"_struct_align"$align"_novector/

      "$ex"_struct_align_"$align" ../../data/$dir --noflush
      mv results_*.csv $NODE/"$dir"_struct_align"$align"_noflush/

      "$ex"_struct_align_"$align"_novector ../../data/$dir --noflush
      mv results_*.csv $NODE/"$dir"_struct_align"$align"_noflush_novector/

    done

  done

done


python3 ../../utils/plot_struct_alignments.py -l local
