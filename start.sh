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



# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-once:fork-once -- -q   -f run fork-once
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-multiple:fork-multiple -- -q   -f run fork-multiple
#  pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-recursive:fork-recursive -- -q   -f run fork-recursive
#  pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-read:fork-read -p ../../tests/userprog/sample.txt:sample.txt -- -q   -f run fork-read
#  pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-close:fork-close -p ../../tests/userprog/sample.txt:sample.txt -- -q   -f run fork-close
#  pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/fork-boundary:fork-boundary -- -q   -f run fork-boundary
pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-once:exec-once -p tests/userprog/child-simple:child-simple -- -q   -f run exec-once
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-arg:exec-arg -p tests/userprog/child-args:child-args -- -q   -f run exec-arg
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-boundary:exec-boundary -p tests/userprog/child-simple:child-simple -- -q   -f run exec-boundary
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-missing:exec-missing -- -q   -f run exec-missing
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-bad-ptr:exec-bad-ptr -- -q   -f run exec-bad-ptr
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/exec-read:exec-read -p ../../tests/userprog/sample.txt:sample.txt -p tests/userprog/child-read:child-read -- -q   -f run exec-read
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/wait-simple:wait-simple -p tests/userprog/child-simple:child-simple -- -q   -f run wait-simple
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/wait-twice:wait-twice -p tests/userprog/child-simple:child-simple -- -q   -f run wait-twice
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/wait-killed:wait-killed -p tests/userprog/child-bad:child-bad -- -q   -f run wait-killed
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/wait-bad-pid:wait-bad-pid -- -q   -f run wait-bad-pid
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/multi-recurse:multi-recurse -- -q   -f run 'multi-recurse 15'
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/multi-child-fd:multi-child-fd -p ../../tests/userprog/sample.txt:sample.txt -p tests/userprog/child-close:child-close -- -q   -f run multi-child-fd
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/rox-simple:rox-simple -- -q   -f run rox-simple
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/rox-child:rox-child -p tests/userprog/child-rox:child-rox -- -q   -f run rox-child
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/userprog/rox-multichild:rox-multichild -p tests/userprog/child-rox:child-rox -- -q   -f run rox-multichild
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/filesys/base/lg-random:lg-random -- -q   -f run lg-random
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/filesys/base/sm-random:sm-random -- -q   -f run sm-random
# pintos -v -k -T 300 -m 20   --fs-disk=10 -p tests/filesys/base/syn-read:syn-read -p tests/filesys/base/child-syn-read:child-syn-read -- -q   -f run syn-read
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/filesys/base/syn-remove:syn-remove -- -q   -f run syn-remove
# pintos -v -k -T 60 -m 20   --fs-disk=10 -p tests/filesys/base/syn-write:syn-write -p tests/filesys/base/child-syn-wrt:child-syn-wrt -- -q   -f run syn-write
# pintos -v -k -T 600 -m 20 -m 20   --fs-disk=10 -p tests/userprog/no-vm/multi-oom:multi-oom -- -q   -f run multi-oom