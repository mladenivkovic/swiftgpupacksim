#!/usr/bin/bash

module restore likwid_intel2024

# Generate executables first using `swiftgpupacksim/sim/make_all_loop_splitting_variants.sh`

NODE=mad06

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

for part_access in "part-struct" "global-var" "explicit-var"; do
  for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do

    # Experiment reproduction directories
    for dir in EAGLE25_128threads Gresho256_128threads; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush

      NTHREADS=128
      if [[ "$dir" == *"_64thread"* ]]; then NTHREADS=64; fi

      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-force pack-shared pack-force-type random-order; do

        # construct executable and args
        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"
        ex="$ex"" ../swiftgpupack_""$NODE"_"$layout"_"$part_access"_"$loop"
        ex="$ex"" -s 2"
        ex="$ex"" --noflush"

        outfile=$NODE/"$dir"_"$part_access"_"$loop"_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/dine2/$dir
          $ex ../../data/mad06/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        outfile=$NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex"_vector ../../data/dine2/$dir
          $ex"_vector" ../../data/mad06/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi


      done
    done
  done
done

