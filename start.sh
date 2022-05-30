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

pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/create-null:create-null -- -q   -f run create-null
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/create-bad-ptr:create-bad-ptr -- -q   -f run create-bad-ptr




pintos -v -k -T 60 -m 20   --fs-disk=10  -- -q  -threads-tests -f run alarm-simultaneous
pintos -v -k -T 60 -m 20   --fs-disk=10  -- -q  -threads-tests -f run priority-condvar
pintos -v -k -T 60 -m 20   --fs-disk=10  -- -q  -threads-tests -f run priority-donate-chain
