import awtk_locator
import sys

COMPILE_CONFIG = {
  'AWTK_ROOT' : { 'value' : '', 'desc' : ['awtk root'], 'help_info' : 'set link awtk root, AWTK_ROOT=XXXXX'},
  'LINUX_FB' : { 'value' : False, 'desc' : ['use linux\'s building'], 'help_info' : 'use linux\'s compile tools prefix building, value is true or false'},
  'SHARED' : { 'value' : True, 'desc' : ['build share library'], 'help_info' : 'set build share library, value is true or false'},
  'WITH_IOTJS' : { 'value' : False, 'desc' : ['support iotjs language, but must WITH_JS is true'], 'help_info' : 'set enable support iotjs language, but must WITH_JS is true, value is true or false'},
  'WITH_JERRYSCRIPT' : { 'value' : True, 'desc' : ['support js language'], 'help_info' : 'set enable support iotjs language, value is true or false'},
  'DEBUG' : { 'value' : True, 'desc' : ['awtk\'s compile is debug'], 'help_info' : 'awtk\'s compile is debug, value is true or false' },
  'PDB' : { 'value' : True, 'desc' : ['export pdb file'], 'help_info' : 'export pdb file, value is true or false' },
  'BUILD_TESTS' : { 'value' : True, 'desc' : ['build awtk\'s mvvm\'s gtest demo'], 'help_info' : 'build awtk\'s mvvm\'s gtest demo, value is true or false' },
  'BUILD_DEMOS' : { 'value' : True, 'desc' : ['build awtk\'s mvvm\'s demo examples'], 'help_info' : 'build awtk\'s mvvm\'s demo examples, value is true or false' },
  'BUILD_TOOLS' : { 'value' : True, 'desc' : ['build awtk\'s mvvm\'s tools'], 'help_info' : 'build awtk\'s mvvm\'s tools, value is true or false' },
  'BUILD_DIR' : { 'value' : None, 'desc' : ['build dir, compile temp file dir'], 'help_info' : 'set build dir, save compile temp file dir, BUILD_DIR=XXXXX'},
  'APP_BIN_DIR' : { 'value' : None, 'desc' : ['build bin dir'], 'help_info' : 'set build bin dir, APP_BIN_DIR=XXXXX'},
  'APP_LIB_DIR' : { 'value' : None, 'desc' : ['build lib dir'], 'help_info' : 'set build lib dir, APP_LIB_DIR=XXXXX'},
}


def init(ARGUMENTS) :
  awtk_locator.init(ARGUMENTS)
  import app_helper_base as app
  if hasattr(app, 'set_compile_config') :
    app.set_compile_config(COMPILE_CONFIG)
  else :
    sys.exit("this is project must use new AWTK version !!!!!!!!!!")


