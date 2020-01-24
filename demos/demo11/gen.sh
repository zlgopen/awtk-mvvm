cp ../demo9/temperature.h .
cp ../demo9/temperature.c .
cp ../demo9/humidity.h .
cp ../demo9/humidity.c .

rm -f *view_model.*

node ../../../awtk/tools/idl_gen/index.js idl.json .
node ../../tools/gen_vm.js idl.json

