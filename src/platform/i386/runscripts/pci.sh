#!/bin/sh

cp wheels.o llboot.o
./cos_linker "llboot.o, :" ./gen_client_stub
