#!/bin/bash

set -e
make clean && make
cmd="./swiftgpupacksim -v ../data/IntelXeonGold5218_64_1thread"
if [ $# -gt 0 ]; then
    case "$1" in
    -g | g | gdb)
        cmd='gdb --args '"$cmd"
        ;;
    *)
        echo unknown cmdline param, running without gdb
        ;;
    esac
fi

$cmd
