#!/bin/bash

# Generate executables first using `swiftgpupacksim/sim/make_all_loop_splitting_variants.sh`

NODE=local

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

# for part_access in "part-struct" "global-var" "explicit-var"; do
#   for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do
for part_access in "part-struct"; do
  for loop in "none"; do

    # Experiment reproduction directories
    # for dir in EAGLE25_64threads Gresho256_64threads EAGLE25_32threads Gresho256_32threads; do
    # for dir in EAGLE25_64threads Gresho256_64threads; do
    # for dir in IntelCoffeeLake_Gresho128_6threads; do
    for dir in TestCellSize_64 TestCellSize_128 TestCellSize_512 TestCellSize_2048; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush

      NTHREADS=1
      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-gradient-type pack-force pack-force-type pack-shared pack-shared-type random-order; do

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_""$NODE"_"$layout"_"$part_access"_"$loop"" --noflush"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/$dir
          $ex ../../data/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

      done
    done
  done
done

