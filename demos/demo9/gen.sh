cp ../demo6/temperature.h .
cp ../demo6/temperature.c .
rm -f humidity_view_model.* temperature_view_model.*

node ../../../awtk/tools/idl_gen/index.js idl.json "humidity.h;temperature.h"
node ../../tools/view_model_gen/gen_vm.js idl.json

