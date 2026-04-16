#!/bin/bash

# Generate executables first using `swiftgpupacksim/sim/make_loop_splitting_variants.sh`

NODE=local
# NODE=gn001
# NODE=gn002

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

for part_access in "part-struct" "global-var" "explicit-var"; do
# for part_access in "part-struct"; do
# for part_access in "explicit-var"; do
# for part_access in "global-var"; do
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
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_vector
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_vector_noflush

      NTHREADS=4
      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        ex=../swiftgpupack_"$layout"_"$part_access"_"$loop"

        echo running "$ex" ../../data/$dir -s 1
        "$ex" ../../data/$dir -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"/

        echo running "$ex" ../../data/$dir --noflush -s 1
        "$ex" ../../data/$dir --noflush -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_noflush/

        echo running "$ex"_vector ../../data/$dir -s 1
        "$ex"_vector ../../data/$dir -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector/

        echo running "$ex"_vector ../../data/$dir --noflush -s 1
        "$ex"_vector ../../data/$dir --noflush -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/

        echo running "$ex"_packed ../../data/$dir -s 1
        "$ex"_packed ../../data/$dir -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed/

        echo running "$ex"_packed ../../data/$dir --noflush -s 1
        "$ex"_packed ../../data/$dir --noflush -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush/

        echo running "$ex"_vector_packed ../../data/$dir -s 1
        "$ex"_vector_packed ../../data/$dir -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_vector/

        echo running "$ex"_vector_packed ../../data/$dir --noflush -s 1
        "$ex"_vector_packed ../../data/$dir --noflush -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_vector_noflush/

      done
    done
  done
done


# python3 ../../utils/plot_loop_splitting_all_in_single_plot.py -l local --png --flush-vector
# python3 ../../utils/plot_loop_splitting_all_in_single_plot.py -l local --png --packed-vector
# python3 ../../utils/plot_loop_splitting_all_in_single_plot.py -l local --png --packed-flush
