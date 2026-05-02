#!/bin/bash

# Generate executables first using `swiftgpupacksim/sim/make_all_loop_splitting_variants.sh`

set -e
rm -f results_*.csv

NODE=local-legion
# EXPERIMENT="IntelCoffeeLake_Gresho128_1thread"
# NTHREADS=1
EXPERIMENT="IntelCoffeeLake_Gresho128_6thread"
NTHREADS=6
RUN_ALL="false"
NSTEPS=5

while [[ $# > 0 ]]; do
  arg="$1"
  case $arg in
    -h | --help)
      echo "script to run all loop variant tests on a local laptop."
      echo "use -hp for HP machine. Default: Lenovo Legion."
      echo "use -a to run *all* tests (include vector/novector, flush/noflush variants)"
      echo "use -s <nsteps> to set number of simulation steps to run"
      exit
    ;;
    -hp)
      EXPERIMENT="IntelXeonGold5218_Gresho64_4threads"
      NTHREADS=4
      # EXPERIMENT="IntelXeonGold5218_Gresho64_1thread"
      # NTHREADS=1
    ;;
    -a | --all)
      RUN_ALL="true"
    ;;
    -s | --steps)
      NSTEPS=$2
      shift
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
        mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_vector
        # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush
        # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush
        # mkdir -p $NODE/"$dir"_"$part_access"_"$loop"_packed_vector_noflush
      fi

      for layout in aos soa upstream pack-gradient pack-force pack-shared; do

        ex="likwid-pin -c N:0-""$NTHREADS"" ../swiftgpupack_""$layout"_"$part_access"_"$loop"

        echo running "$ex" ../../data/$dir -s $NSTEPS
        $ex ../../data/$dir -s $NSTEPS
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"/

        echo running "$ex"_packed ../../data/$dir -s $NSTEPS
        $ex"_packed" ../../data/$dir -s $NSTEPS
        mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed/

        if [[ "$RUN_ALL" == "true" ]]; then

          echo running "$ex" ../../data/$dir --noflush -s $NSTEPS
          $ex ../../data/$dir --noflush -s $NSTEPS
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_noflush/

          echo running "$ex"_vector ../../data/$dir -s $NSTEPS
          $ex"_vector" ../../data/$dir -s $NSTEPS
          mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector/

          # echo running "$ex"_vector ../../data/$dir --noflush -s $NSTEPS
          # $ex"_vector" ../../data/$dir --noflush -s $NSTEPS
          # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_vector_noflush/
          #
          # echo running "$ex"_packed ../../data/$dir --noflush -s $NSTEPS
          # $ex"_packed" ../../data/$dir --noflush -s $NSTEPS
          # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_noflush/
          #
          # echo running "$ex"_vector_packed ../../data/$dir -s $NSTEPS
          # $ex"_vector_packed" ../../data/$dir -s $NSTEPS
          # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_vector/
          #
          # echo running "$ex"_vector_packed ../../data/$dir --noflush -s $NSTEPS
          # $ex"_vector_packed" ../../data/$dir --noflush -s $NSTEPS
          # mv results_*.csv $NODE/"$dir"_"$part_access"_"$loop"_packed_vector_noflush/
        fi

      done
    done
  done
done

