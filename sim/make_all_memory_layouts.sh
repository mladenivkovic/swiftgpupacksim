#!/bin/bash

if [[ ! -f ./configure ]]; then
  ./autogen.sh
fi

EXTRA_CONFFLAGS=""

for layout in aos soa; do
  ./configure --with-particle-memory-layout="$layout" $EXTRA_CONFFLAGS
  make clean
  make -j
  mv ./swiftgpupack ./swiftgpupack_$layout
done
