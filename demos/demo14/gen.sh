cp ../demo6/temperature.h .
cp ../demo6/temperature.c .

node ../../../awtk/tools/idl_gen/index.js idl.json temperature.h
node ../../tools/view_model_gen/gen_vm.js idl.json

