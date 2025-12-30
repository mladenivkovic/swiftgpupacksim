#!/bin/bash

set -e

if [[ ! -f ./configure ]]; then
  ./autogen.sh
fi

EXTRA_CONFFLAGS=""

if [ $# -gt 0 ]; then
  case "$1" in
  -d | d | debug | DEBUG )
    EXTRA_CONFFLAGS="--enable-debug --enable-debugging-checks"
    ;;
  *)
    echo unknown cmdline param "'""$1""'"
    ;;
  esac
fi

for layout in aos soa soa-modified upstream pack-gradient pack-force pack-shared; do
  for align in 1 2 4 8 16 32 64; do
    ./configure --config-cache --with-particle-memory-layout="$layout" $EXTRA_CONFFLAGS
    make clean
    make -j
    mv ./swiftgpupack ./swiftgpupack_"$layout"_struct_align_"$align"

    ./configure --config-cache --with-particle-memory-layout="$layout" $EXTRA_CONFFLAGS --disable-vectorization
    make clean
    make -j
    mv ./swiftgpupack ./swiftgpupack_"$layout"_struct_align_"$align"_novector
  done
done
