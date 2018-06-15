#!/bin/bash

#hello stack source files
scp -r -P 10022 hello_stack.c root@localhost:/usr/src/minix/drivers/examples/hello_stack/hello_stack.c
scp -r -P 10022 hello_stack.h root@localhost:/usr/src/minix/drivers/examples/hello_stack/hello_stack.h
scp -r -P 10022 Makefile root@localhost:/usr/src/minix/drivers/examples/hello_stack/Makefile

#config file
scp -r -P 10022 system.conf root@localhost:/etc/system.conf

#tests
#scp -r -P 10022 tests/root/* root@localhost:/root
#scp -r -P 10022 tests/bash_test_so/* root@localhost:~