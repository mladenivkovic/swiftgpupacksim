#!/usr/bin/bash

module restore likwid_intel2024

# Generate executables first using `swiftgpupacksim/sim/make_all_loop_splitting_variants.sh`

# NODE=local
# NODE=gn001
# NODE=gn002
# NODE=gn003
# NODE=dine2
NODE=mad06

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

for part_access in "part-struct" "global-var" "explicit-var"; do
  for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do

    # Experiment reproduction directories
    for dir in EAGLE25_64threads Gresho256_64threads; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_packed_noflush

      NTHREADS=128
      if [[ "$dir" == *"_64thread"* ]]; then NTHREADS=64; fi

      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        # construct executable and args
        ex="likwid-pin --quiet -c N:0-""$((NTHREADS - 1))"
        ex="$ex"" ../swiftgpupack_""$NODE"_"$layout"_"$part_access"_"$loop"

        EXFLAGS="-s 2 --noflush"
        EXFLAGS="$EXFLAGS"" ../../data/mad06/$dir"

        outfile=$NODE/"$dir"_"$part_access"_"$loop"_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex" "$EXFLAGS"
          $ex $EXFLAGS
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        outfile=$NODE/"$dir"_"$part_access"_"$loop"_packed_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex"_packed "$EXFLAGS"
          $ex"_packed" $EXFLAGS
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        outfile=$NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex"_vector "$EXFLAGS"
          $ex"_vector" $EXFLAGS
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

        outfile=$NODE/"$dir"_"$part_access"_"$loop"_vector_packed_noflush/results_"$layout".csv
        if [[ ! -f "$outfile" ]]; then
          echo running "$ex"_vector_packed "$EXFLAGS"
          $ex"_vector_packed" $EXFLAGS
          mv results_"$layout".csv "$outfile"
          echo written $outfile
        else
          echo found $outfile, not rerunning
        fi

      done
    done
  done
done

