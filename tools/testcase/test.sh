#!/bin/bash

for f in *.json;do node ../gen_vm.js $f;done

gcc -c -Wall -I../../src/ -I../../../awtk/src *.c

rm -fv *.h *.c *.o

