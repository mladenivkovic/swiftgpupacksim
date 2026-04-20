#!/usr/bin/bash

module restore gpu_swift_oneapi_2025

# Generate executables first using `swiftgpupacksim/sim/make_all_loop_splitting_variants.sh`

# NODE=local
# NODE=gn001
# NODE=gn002
NODE=gn004

set -e
rm -f results_*.csv

export OMP_PROC_BIND=true

for part_access in "part-struct" "global-var" "explicit-var"; do
  for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do

    # Experiment reproduction directories
    for dir in Gresho128_16threads; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector

      # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush
      # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush
      # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_packed
      # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_packed_noflush

      NTHREADS=16
      # if [[ "$dir" == *"_72thread"* ]]; then NTHREADS=72; fi

      export OMP_NUM_THREADS=$NTHREADS

      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        ex="likwid-pin -c N:16-""$((NTHREADS - 1))"" ../swiftgpupack_""$layout"_"$part_access"_"$loop"

        echo running "$ex" ../../data/gn004/$dir
        $ex ../../data/gn004/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"/

        echo running "$ex" ../../data/gn004/$dir --noflush
        $ex ../../data/gn004/$dir --noflush
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_noflush/

        echo running "$ex"_vector ../../data/gn004/$dir
        $ex"_vector" ../../data/gn004/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector/

        echo running "$ex"_packed ../../data/gn002/$dir
        $ex"_packed" ../../data/gn004/$dir
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed/

        # echo running "$ex"_vector ../../data/gn002/$dir --noflush
        # $ex"_vector" ../../data/gn004/$dir --noflush
        # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/

        # echo running "$ex"_packed ../../data/gn002/$dir --noflush
        # $ex"_packed" ../../data/gn004/$dir --noflush
        # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush/

        # echo running "$ex"_vector_packed ../../data/gn002/$dir
        # $ex"_vector_packed" ../../data/gn004/$dir
        # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_packed/

        # echo running "$ex"_vector_packed ../../data/gn002/$dir --noflush
        # $ex"_vector_packed" ../../data/gn004/$dir --noflush
        # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_packed_noflush/

      done
    done
  done
done


