#!/bin/bash

source ./activate
make clean
cd ./userprog
make
cd ./build
# make
# cd ./threads
# make check
# pintos -- -q run alarm-multiple
# pintos -v -k -T 60 -m 20   -- -q   run alarm-priority
# cd build
# pintos -- -q   run priority-donate-one
# pintos --fs-disk=10 -p tests/userprog/args-single:args-single -- -q -f run 'args-single onearg'
# pintos --fs-disk=10 -p tests/userprog/echo -- -q -f run 'echo x'

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/open-missing:open-missing -- -q   -f run open-missing
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/open-empty:open-empty -- -q   -f run open-empty
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/open-twice:open-twice -p ../../tests/userprog/sample.txt:sample.txt -- -q   -f run open-twice