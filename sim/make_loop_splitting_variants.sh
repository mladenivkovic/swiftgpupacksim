#!/bin/bash

set -e

if [[ ! -f ./configure ]]; then
  ./autogen.sh
fi

EXTRA_CONFFLAGS="--config-cache"

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

for part_access in "part-struct" "global-var" "explicit-var"; do
  for loop in "none" "by-struct" "by-element"; do
    for layout in "aos" "soa" "upstream" "pack-gradient" "pack-force" "pack-shared"; do
      ./configure --with-particle-memory-layout="$layout" --with-loop-split=$loop --with-particle-access=$part_access $EXTRA_CONFFLAGS
      make clean
      echo
      echo $loop $layout
      echo
      make -j
      mv ./swiftgpupack ./swiftgpupack_"$layout"_"$part_access"_"$loop"

      ./configure --with-particle-memory-layout="$layout" --with-loop-split=$loop --with-particle-access=$part_access --enable-vectorization-pragmas $EXTRA_CONFFLAGS
      make clean
      echo
      echo $loop $layout
      echo
      make -j
      mv ./swiftgpupack ./swiftgpupack_"$layout"_"$part_access"_"$loop"_vector

      ./configure --with-particle-memory-layout="$layout" --with-loop-split=$loop --with-particle-access=$part_access --enable-packed-structs $EXTRA_CONFFLAGS
      make clean
      echo
      echo $loop $layout
      echo
      make -j
      mv ./swiftgpupack ./swiftgpupack_"$layout"_"$part_access"_"$loop"_packed

      ./configure --with-particle-memory-layout="$layout" --with-loop-split=$loop --with-particle-access=$part_access --enable-vectorization-pragmas --enable-packed-structs $EXTRA_CONFFLAGS
      make clean
      echo
      echo $loop $layout
      echo
      make -j
      mv ./swiftgpupack ./swiftgpupack_"$layout"_"$part_access"_"$loop"_vector_packed

    done
  done
done
