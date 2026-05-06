#!/bin/bash

# Generate executables first using `swiftgpupacksim/sim/make_flushing_variants.sh`

NODE=local

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

# for part_access in "part-struct" "global-var" "explicit-var"; do
#   for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do
for part_access in "explicit-var"; do
  for loop in "none"; do

    # Experiment reproduction directories
    # for dir in EAGLE25_64threads Gresho256_64threads EAGLE25_32threads Gresho256_32threads; do
    # for dir in EAGLE25_64threads Gresho256_64threads; do
    for dir in IntelCoffeeLake_Gresho128_6threads; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_flush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_bigflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_x86flush

      NTHREADS=1
      if [[ "$dir" == *"_6threads"* ]]; then NTHREADS=6; fi
      if [[ "$dir" == *"_4threads"* ]]; then NTHREADS=4; fi

      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_FLUSH_""$NODE"_"$layout"_"$part_access"_"$loop"" -s 2 --noflush"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/$dir
          $ex ../../data/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_FLUSH_""$NODE"_"$layout"_"$part_access"_"$loop"" -s 2"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_flush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/$dir
          $ex ../../data/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_BIGFLUSH_""$NODE"_"$layout"_"$part_access"_"$loop"" -s 2"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_bigflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" ../../data/$dir
          $ex ../../data/$dir
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"" ../swiftgpupack_FLUSHX86_""$NODE"_"$layout"_"$part_access"_"$loop"" -s 2"
        outfile=$NODE/"$dir"_"$part_access"_"$loop"_x86flush/results_"$layout".csv
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

