#!/bin/bash

# Generate executables first using `swiftgpupacksim/sim/make_all_loop_splitting_variants.sh`

set -e
rm -f results_*.csv

NODE=local
EXPERIMENT="IntelCoffeeLake_Gresho128_6threads"
NTHREADS=6
RUN_ALL="false"

while [[ $# > 0 ]]; do
  arg="$1"
  case $arg in
    -h | --help)
      echo "script to run all loop variant tests on a local laptop."
      echo "use -hp for HP machine. Default: Lenovo Legion."
      echo "use -a to run *all* tests (include vector/novector, flush/noflush variants)"
      exit
    ;;
    -hp)
      EXPERIMENT="IntelXeonGold5218_Gresho64_4threads"
      NTHREADS=4
    ;;
    -a | --all)
      RUN_ALL="true"
    ;;
    *)
      echo "Unknown argument: '""$arg""'"
      exit
    ;;
  esac;
  shift
done

export OMP_PROC_BIND=true
export OMP_NUM_THREADS=$NTHREADS

for part_access in "part-struct" "global-var" "explicit-var"; do
  for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do

    # Experiment reproduction directories
    for dir in "$EXPERIMENT"; do

      mkdir -p $NODE
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"
      mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed

      if [[ "$RUN_ALL" == "true" ]]; then
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_noflush
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_vector
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_vector_noflush
      fi

      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        ex="likwid-pin -c N:0-""$NTHREADS"" ../swiftgpupack_""$layout"_"$part_access"_"$loop"

        echo running "$ex" ../../data/$dir -s 1
        $ex ../../data/$dir -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"/

        echo running "$ex"_packed ../../data/$dir -s 1
        $ex"_packed" ../../data/$dir -s 1
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed/

        if [[ "$RUN_ALL" == "true" ]]; then

          echo running "$ex" ../../data/$dir --noflush -s 1
          $ex ../../data/$dir --noflush -s 1
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_noflush/

          echo running "$ex"_vector ../../data/$dir -s 1
          $ex"_vector" ../../data/$dir -s 1
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector/

          echo running "$ex"_vector ../../data/$dir --noflush -s 1
          $ex"_vector" ../../data/$dir --noflush -s 1
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/

          echo running "$ex"_packed ../../data/$dir --noflush -s 1
          $ex"_packed" ../../data/$dir --noflush -s 1
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush/

          echo running "$ex"_vector_packed ../../data/$dir -s 1
          $ex"_vector_packed" ../../data/$dir -s 1
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_vector/

          echo running "$ex"_vector_packed ../../data/$dir --noflush -s 1
          $ex"_vector_packed" ../../data/$dir --noflush -s 1
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_vector_noflush/
        fi

      done
    done
  done
done


# python3 ../../utils/plot_loop_splitting_all_in_single_plot.py -l local --png --flush-vector
# python3 ../../utils/plot_loop_splitting_all_in_single_plot.py -l local --png --packed-vector
# python3 ../../utils/plot_loop_splitting_all_in_single_plot.py -l local --png --packed-flush
