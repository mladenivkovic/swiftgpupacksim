#!/bin/bash

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
  ./configure --with-particle-memory-layout="$layout" $EXTRA_CONFFLAGS
  make clean
  make -j
  mv ./swiftgpupack ./swiftgpupack_$layout
done
