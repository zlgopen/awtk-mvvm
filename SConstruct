import os
import re
import platform
import scripts.app_helper as app
import scripts.scons_argv
scripts.scons_argv.init(ARGUMENTS)

helper = app.Helper(ARGUMENTS);
complie_helper = helper.get_curr_config()

def dll_def_processor():
  content = ''

  with open('src/mvvm.def', 'r') as f:
    content = f.read()

    regex = [];
    if (not 'WITH_JS' in os.environ or os.environ['WITH_JS'] != 'true') and (not 'WITH_IOTJS' in os.environ or os.environ['WITH_IOTJS'] != 'true') :
      regex += [' +jerry.+\n', ' +.+_jerryscript_.+\n', ' +jsvalue.+\n', ' +js_.+\n', ' +jsobj_.+\n', ' +object_js_.*\n', ' +object_is_object_js_.*\n']
    if not 'WITH_IOTJS' in os.environ or os.environ['WITH_IOTJS'] != 'true' :
      regex += ['mvvm_iotjs_.+\n']
    for r in regex:
      reobj = re.compile(r)
      content, number = reobj.subn('', content)

  with open('src/mvvm.def', 'w') as f:
    f.write(content)


TK_3RD_DIRS = []
APP_LIBPATH = []
SConscripts = ['src/SConscript']
APP_LIBS = []
APP_CFLAGS = '-DMVVM_DLL_EXPORT '
APP_3RD_ROOT = os.path.normpath(os.path.join(os.getcwd(), '3rd'))

APP_TOOLS_ROOT = os.path.normpath(os.path.join(os.getcwd(), 'tools'))
os.environ['APP_TOOLS_ROOT'] = APP_TOOLS_ROOT

BUILD_TOOLS = True
BUILD_TESTS = True
BUILD_DEMOS = True
if complie_helper.get_value('LINUX_FB', True) :
  BUILD_TOOLS = False
  BUILD_TESTS = False

os.environ['BUILD_DEMOS'] = str(complie_helper.get_value('BUILD_DEMOS', True)).lower()
os.environ['BUILD_TESTS'] = str(complie_helper.get_value('BUILD_TESTS', BUILD_TESTS)).lower()
os.environ['BUILD_TOOLS'] = str(complie_helper.get_value('BUILD_TOOLS', BUILD_TOOLS)).lower()
os.environ['WITH_JS'] = str(complie_helper.get_value('WITH_JERRYSCRIPT', True)).lower()
if 'WITH_JS' in os.environ and os.environ['WITH_JS'] == 'true':
  os.environ['WITH_JS_SNAPSHOT'] = 'true'
  os.environ['WITH_JS_EXTERNAL_CONTEXT'] = 'true'
  os.environ['WITH_IOTJS'] = str(complie_helper.get_value('WITH_IOTJS', True)).lower()

  APP_CFLAGS +=' -DWITH_JERRYSCRIPT '
  if 'WITH_JS_SNAPSHOT' in os.environ and os.environ['WITH_JS_SNAPSHOT'] == 'true':
    APP_CFLAGS +=' -DWITH_JERRYSCRIPT_SNAPSHOT '
  if 'WITH_JS_EXTERNAL_CONTEXT' in os.environ and os.environ['WITH_JS_EXTERNAL_CONTEXT'] == 'true':
    APP_CFLAGS += ' -DJERRY_EXTERNAL_CONTEXT=1 -DJERRY_GLOBAL_HEAP_SIZE=2048 '
  else:
    APP_CFLAGS += ' -DCONFIG_MEM_HEAP_AREA_SIZE=2097152 '

  SConscripts += ['src/run_js_mvvm/SConscript']

if 'WITH_IOTJS' in os.environ and os.environ['WITH_IOTJS'] == 'true':
  OS_NAME = platform.system();
  IOTJS_ROOT = os.path.join(APP_3RD_ROOT, 'iotjs')
  APP_LIBPATH = [ os.path.join(IOTJS_ROOT, 'lib') ]
  APP_CFLAGS +=' -DWITH_IOTJS'

  if OS_NAME == 'Windows' :
    APP_LIBS += [ 'ws2_32.lib', 'UserEnv.lib', 'iphlpapi.lib']
    APP_CFLAGS +=' -DWIN32 -D_WINDOWS -DDEBUG -DENABLE_DEBUG_LOG -DJERRY_DEBUGGER -DEXPERIMENTAL -D__x86_64__ '
  elif OS_NAME == 'Linux':
    APP_LIBS += ['pthread', 'm', 'dl', 'rt']
    APP_CFLAGS +=' -DDEBUG -DENABLE_DEBUG_LOG -D__x86_64__ '
  APP_LIBS += [ 'iotjs', 'jerry-core', 'jerry-port-default', 'jerry-ext', 'tuv' , 'http-parser' ]

  TK_3RD_DIRS += [
    os.path.join(IOTJS_ROOT, 'include'),
    os.path.join(IOTJS_ROOT, 'src'),
    os.path.join(IOTJS_ROOT, 'deps/jerry/jerry-port/default/include'),
    os.path.join(IOTJS_ROOT, 'deps/jerry/jerry-ext/include'),
    os.path.join(IOTJS_ROOT, 'deps/jerry/jerry-core/include'),
    os.path.join(IOTJS_ROOT, 'deps/http-parser'),
    os.path.join(IOTJS_ROOT, 'deps/libtuv/include'),
    ]
elif 'WITH_JS' in os.environ and os.environ['WITH_JS'] == 'true':
  SConscripts += ['3rd/jerryscript/SConscript']
  APP_LIBS += ['jerryscript']
  TK_3RD_DIRS += [
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/include'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/arg'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/common'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/debugger'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/handler'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/module'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/include'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/base'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/builtin-objects'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/operations'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/jcontext'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/jrt'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/parser'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/parser/js'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/parser/regexp'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/vm'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/api'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/debugger'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/lit'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/jmem'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/profiles'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-port/default/include'),
    os.path.join(APP_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/builtin-objects/typedarray'),
    os.path.join(APP_3RD_ROOT, 'nativefiledialog/nativefiledialog/src/include'),
  ]
else:
  os.environ['BUILD_TESTS'] = 'false'

if 'BUILD_DEMOS' in os.environ and os.environ['BUILD_DEMOS'] == 'true':
  SConscripts += ['demos/SConscript']

if 'BUILD_TESTS' in os.environ and os.environ['BUILD_TESTS'] == 'true':
  SConscripts += ['tests/SConscript']

if 'BUILD_TOOLS' in os.environ and os.environ['BUILD_TOOLS'] == 'true':
  if 'WITH_JS' in os.environ and os.environ['WITH_JS'] == 'true' :
    SConscripts += ['tools/mvvm_factory_gen/SConscript']
  SConscripts += ['tools/mvvm_prop_gen/SConscript']

helper.set_dll_def('src/mvvm.def').set_dll_def_processor(dll_def_processor).add_cpppath(TK_3RD_DIRS).add_ccflags(APP_CFLAGS).add_libs(APP_LIBS).add_libpath(APP_LIBPATH).call(DefaultEnvironment)

helper.SConscript(SConscripts)
