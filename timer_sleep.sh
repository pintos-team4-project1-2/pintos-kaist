#!/bin/bash

source ./activate
# cd ./threads/build
cd ./threads
make check
# pintos -- -q run alarm-multiple
# pintos -v -k -T 60 -m 20   -- -q   run alarm-priority
