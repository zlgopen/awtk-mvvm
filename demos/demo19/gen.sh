cp ../demo6/temperature.h .
cp ../demo6/temperature.c .
cp ../demo10/temperature_timer.h .
cp ../demo10/temperature_timer.c .
rm -f *view_model.*

node ../../../awtk/tools/idl_gen/index.js idl.json .
node ../../tools/gen_vm.js idl.json

