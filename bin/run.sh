#!/bin/bash

set -e
make
./swiftgpupacksim -v ../data/IntelXeonGold5218_64_1thread
