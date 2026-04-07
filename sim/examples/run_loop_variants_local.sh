#!/bin/bash

# Generate executables first using `swiftgpupacksim/sim/make_loop_splitting_variants.sh`

NODE=local
# NODE=gn001
# NODE=gn002

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

# for part_access in "part-struct" "global-var" "explicit-var"; do
# for part_access in "part-struct"; do
# for part_access in "explicit-var"; do
for part_access in "global-var"; do
  for loop in "none" "by-struct" "by-element"; do

    # Experiment reproduction directories
    for dir in IntelXeonGold5218_Gresho64_4threads; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_packed
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_packed_noflush

      NTHREADS=4

      # for ex in ../swiftgpupack_aos ../swiftgpupack_soa ../swiftgpupack_soa-modified ../swiftgpupack_upstream ../swiftgpupack_pack-gradient ../swiftgpupack_pack-force ../swiftgpupack_pack-shared; do
      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        ex=../swiftgpupack_"$layout"_"$part_access"_"$loop"

        echo "$ex" ../../data/$dir
        "$ex" ../../data/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"/

        "$ex" ../../data/$dir --noflush
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_noflush/

        "$ex"_vector ../../data/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector/

        "$ex"_vector ../../data/$dir --noflush
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/

        "$ex"_packed ../../data/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed/

        "$ex"_packed ../../data/$dir --noflush
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush/

        "$ex"_vector_packed ../../data/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_packed/

        "$ex"_vector_packed ../../data/$dir --noflush
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_packed_noflush/

      done
    done
  done
done


python3 ../../utils/plot_struct_alignments.py -l local
