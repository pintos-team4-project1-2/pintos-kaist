#!/bin/bash

make clean
source ./activate
cd ./threads
# make
# cd ./threads
make check
# pintos -- -q run alarm-multiple
# pintos -v -k -T 60 -m 20   -- -q   run alarm-priority
# cd build
# pintos -- -q   run priority-donate-one