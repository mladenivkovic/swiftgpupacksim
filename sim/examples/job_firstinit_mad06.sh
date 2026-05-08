#!/usr/bin/bash

module restore likwid_intel2024

# Generate executables first using `swiftgpupacksim/sim/make_first_init_variants.sh`

NODE=mad06

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

# for part_access in "part-struct" "global-var" "explicit-var"; do
#   for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do
for part_access in "explicit-var"; do
  for loop in "none"; do

    # Experiment reproduction directories
    # for dir in EAGLE25_64threads Gresho256_64threads EAGLE25_128threads Gresho256_128threads; do
    for dir in EAGLE25_128threads Gresho256_128threads; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush_firstinit

      NTHREADS=128
      if [[ "$dir" == *"_64thread"* ]]; then NTHREADS=64; fi

      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-force pack-shared pack-force-type random-order; do

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_NOFIRSTINIT_""$NODE"_"$layout"_"$part_access"_"$loop"" -s 2 --noflush"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/mad06/$dir
          $ex ../../data/mad06/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_PARALLELINIT_""$NODE"_"$layout"_"$part_access"_"$loop"" -s 2 --noflush"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_noflush_firstinit/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/mad06/$dir
          $ex ../../data/mad06/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

      done
    done
  done
done

