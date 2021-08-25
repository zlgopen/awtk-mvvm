cp -f ../cppdemo1/temperature.h .
cp -f ../cppdemo1/temperature.cpp .

rm -rf *view_model.*
node ../../../awtk/tools/idl_gen/index.js idl.json .
node ../../tools/view_model_gen/gen_vm.js idl.json

