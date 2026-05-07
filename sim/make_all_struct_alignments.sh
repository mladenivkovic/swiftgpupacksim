#!/bin/bash

set -e

if [[ ! -f ./configure ]]; then
  ./autogen.sh
fi

# NODE=dine2
NODE=gn003
# NODE=mad06

MYCFLAGS="-march=native -Wno-pass-failed"

EXTRA_CONFFLAGS="--config-cache"

if [[ "$NODE" == "local" ]]; then
  EXTRA_CONFFLAGS="$EXTRA_CONFFLAGS --enable-no-cuda-mallochost"
fi

if [[ "$NODE" == "mad06" ]]; then
  MYCFLAGS="$MYCFLAGS -Wno-unused-command-line-argument"
fi


if [ $# -gt 0 ]; then
  case "$1" in
  -d | d | debug | DEBUG )
    EXTRA_CONFFLAGS+=" --enable-debug --enable-debugging-checks"
    ;;
  *)
    echo unknown cmdline param "'""$1""'"
    ;;
  esac
fi


rm -f config.cache

for layout in aos soa upstream pack-gradient pack-force pack-shared; do
  for align in 1 2 4 8 16 32 64; do
    CFLAGS="$MYCFLAGS" ./configure --config-cache --with-particle-memory-layout="$layout" --with-struct-align=$align $EXTRA_CONFFLAGS
    make clean
    make -j
    mv ./swiftgpupack ./swiftgpupack_"$layout"_struct_align_"$align"

    CFLAGS="$MYCFLAGS" ./configure --config-cache --with-particle-memory-layout="$layout" --with-struct-align=$align $EXTRA_CONFFLAGS --disable-vectorization
    make clean
    make -j
    mv ./swiftgpupack ./swiftgpupack_"$layout"_struct_align_"$align"_novector
  done
done
