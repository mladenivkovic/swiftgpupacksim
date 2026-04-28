#!/bin/bash

LOCALFLAG="--local-legion"
LOCALDIR="local-legion"

while [[ $# > 0 ]]; do
  arg="$1"
  case $arg in
    -h | --help)
      echo "script to run all plotting scripts on a local laptop."
      echo "Assumes all tests have been produced using './run_loop_variants_local.sh'"
      echo
      echo "use -hp for HP machine. Default: Lenovo Legion."
      exit
    ;;
    -hp)
      LOCALFLAG="--local-hp"
      LOCALDIR="local"
    ;;
    *)
      echo "Unknown argument: '""$arg""'"
      exit
    ;;
  esac;
  shift
done


# loop_splitting_compare_part_access_local-legion_EAGLE12_72threads.png
# loop_splitting_compare_part_access_local-legion_Gresho256_72threads.png
python3 ../../utils/loop_splitting_compare_particle_access.py $LOCALFLAG --png $LOCALDIR

# loop_splitting_compare_part_access_local-legion_Gresho256_72threads_normalised.png
# loop_splitting_compare_part_access_local-legion_EAGLE12_72threads_normalised.png
python3 ../../utils/loop_splitting_compare_particle_access.py $LOCALFLAG --png --normalise $LOCALDIR

# loop_splitting_compare_threads_local-legion_Gresho256_part-struct.png
# loop_splitting_compare_threads_local-legion_Gresho256_explicit-var.png
# loop_splitting_compare_threads_local-legion_Gresho256_global-var.png
# loop_splitting_compare_threads_local-legion_EAGLE12_part-struct.png
# loop_splitting_compare_threads_local-legion_EAGLE12_explicit-var.png
# loop_splitting_compare_threads_local-legion_EAGLE12_global-var.png
python3 ../../utils/loop_splitting_compare_nthreads.py $LOCALFLAG --png $LOCALDIR

#TODO: UPDATE THIS WITH NEW DATA
# loop_splitting_compare_packed_local-legion_Gresho256_part-struct.png
# loop_splitting_compare_packed_local-legion_EAGLE12_part-struct.png
python3  ../../utils/loop_splitting_compare_packed.py $LOCALFLAG --png $LOCALDIR


# loop_splitting_compare_vector_local-legion_Gresho256_part-struct.png
# loop_splitting_compare_vector_local-legion_Gresho256_explicit-var.png
# loop_splitting_compare_vector_local-legion_Gresho256_global-var.png
# loop_splitting_compare_vector_local-legion_EAGLE12_part-struct.png
# loop_splitting_compare_vector_local-legion_EAGLE12_explicit-var.png
# loop_splitting_compare_vector_local-legion_EAGLE12_global-var.png
python3 ../../utils/loop_splitting_compare_vector.py $LOCALFLAG --png $LOCALDIR

# loop_splitting_compare_noflush_local_IntelXeonGold5218_Gresho64_part-struct.png
# loop_splitting_compare_noflush_local_IntelXeonGold5218_Gresho64_explicit-var.png
# loop_splitting_compare_noflush_local_IntelXeonGold5218_Gresho64_global-var.png
python3 ../../utils/loop_splitting_compare_noflush.py $LOCALFLAG --png $LOCALDIR
