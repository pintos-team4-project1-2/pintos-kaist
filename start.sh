#!/bin/bash

source ./activate
cd ./userprog/build

make clean
make
# make
# cd ./threads
# make check
# pintos -- -q run alarm-multiple
# pintos -v -k -T 60 -m 20   -- -q   run alarm-priority
# cd build
# pintos -- -q   run priority-donate-one
pintos --fs-disk=10 -p tests/userprog/args-single:args-single -- -q -f run 'args-single onearg'