rm -f *view_model.*

node ../../../awtk/tools/idl_gen/index.js idl.json room_settings.h
node ../../tools/gen_vm.js idl.json

