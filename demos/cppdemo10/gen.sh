cp -rf ../cppdemo9/temperature.h .
cp -rf ../cppdemo9/temperature.cpp .

rm -rf *view_model.*
node ../../../awtk/tools/idl_gen/index.js idl.json .
node ../../tools/gen_vm.js idl.json

