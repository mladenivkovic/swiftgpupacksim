#!/bin/bash

export OMP_NUM_THREADS=6
export OMP_PROC_BIND=true

PREFIX=""
# PREFIX="likwid-pin -c N:0-5"
SUFFIX="--noflush"
# SUFFIX="--noflush -s 1"

while [ $# -gt 0 ]; do
  case "$1" in
  -d | d | debug | DEBUG | -g | g )
    PREFIX="gdb --args"
    ;;
  -v | v | --verbose | verbose )
    SUFFIX="$SUFFIX --verbose"
    ;;
  *)
    echo unknown cmdline param "'""$1""'"
    ;;
  esac
  shift
done


$PREFIX ../swiftgpupack ../../data/IntelCoffeeLake_Gresho128_6threads $SUFFIX

