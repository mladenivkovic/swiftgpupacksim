#!/bin/bash

set -e

if [[ ! -f ./configure ]]; then
  ./autogen.sh
fi

# NODE=local
# NODE=dine2
NODE=gn003
# NODE=mad06

MYCFLAGS="-march=native"

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

# for part_access in "part-struct" "global-var" "explicit-var"; do
  # for loop in "none" "by-struct" "by-element" "by-type" "by-struct-and-type"; do
for part_access in "explicit-var"; do
  for loop in "none"; do
    for layout in "aos" "soa" "upstream" "pack-gradient" "pack-force" "pack-shared"; do

      SUFFIX="$NODE"_"$layout"_"$part_access"_"$loop"

      CFLAGS="$MYCFLAGS" ./configure --with-particle-memory-layout="$layout" --with-loop-split=$loop --with-particle-access=$part_access $EXTRA_CONFFLAGS
      make clean
      echo
      echo $loop $layout
      echo
      make -j
      mv ./swiftgpupack ./swiftgpupack_NOFIRSTINIT_"$SUFFIX"
      echo
      echo COMPILED ./swiftgpupack_NOFIRSTINIT_"$SUFFIX"
      echo

      CFLAGS="$MYCFLAGS" ./configure --with-particle-memory-layout="$layout" --with-loop-split=$loop --with-particle-access=$part_access $EXTRA_CONFFLAGS --enable-parallel-init
      make clean
      echo
      echo $loop $layout
      echo
      make -j
      mv ./swiftgpupack ./swiftgpupack_PARALLELINIT_"$SUFFIX"
      echo
      echo COMPILED ./swiftgpupack_PARALLELINIT_"$SUFFIX"
      echo

    done
  done
done
