rm *view_model.*
node ../../../awtk/tools/idl_gen/index.js idl.json .
node ../../tools/gen_vm.js idl.json
node ../../tools/gen_vm_array.js idl.json

