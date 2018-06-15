#!/bin/bash

scp -r -P 10022 hello_stack.c root@localhost:/usr/src/minix/drivers/examples/hello_stack/hello_stack.c
scp -r -P 10022 hello_stack.h root@localhost:/usr/src/minix/drivers/examples/hello_stack/hello_stack.h
scp -r -P 10022 Makefile root@localhost:/usr/src/minix/drivers/examples/hello_stack/Makefile

scp -r -P 10022 system.conf root@localhost:/etc/system.conf