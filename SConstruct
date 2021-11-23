import os
import re
import scripts.app_helper as app

APP_3RD_ROOT = os.path.normpath(os.path.join(os.getcwd(), '3rd'))

TK_3RD_DIRS = [
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

os.environ['WITH_JS'] = 'true'
os.environ['WITH_JS_SNAPSHOT'] = 'true'
os.environ['WITH_JS_EXTERNAL_CONTEXT'] = 'true'

APP_CFLAGS = '-DMVVM_DLL_EXPORT '
APP_LIBS = ['streams']

if 'WITH_JS' in os.environ and os.environ['WITH_JS'] == 'true':
  APP_LIBS += ['jerryscript']
  APP_CFLAGS +=' -DWITH_JERRYSCRIPT '

  if 'WITH_JS_SNAPSHOT' in os.environ and os.environ['WITH_JS_SNAPSHOT'] == 'true':
    APP_CFLAGS +=' -DWITH_JERRYSCRIPT_SNAPSHOT '

  if 'WITH_JS_EXTERNAL_CONTEXT' in os.environ and os.environ['WITH_JS_EXTERNAL_CONTEXT'] == 'true':
    APP_CFLAGS += ' -DJERRY_EXTERNAL_CONTEXT=1 -DJERRY_GLOBAL_HEAP_SIZE=2048 '
  else:
    APP_CFLAGS += ' -DCONFIG_MEM_HEAP_AREA_SIZE=2097152 '

  SConscripts = ['3rd/jerryscript/SConscript',
    'src/SConscript',
    'demos/SConscript',
    'src/run_js_mvvm/SConscript',
    'tests/SConscript',
    'tools/mvvm_factory_gen/SConscript']

else:
  SConscripts = ['src/SConscript', 'demos/SConscript']

def dll_def_processor():
  content = ''

  with open('src/mvvm.def', 'r') as f:
    content = f.read()

    if not 'WITH_JS' in os.environ or os.environ['WITH_JS'] != 'true':
      regex = [' +jerry.+\n', ' +.+_jerryscript_.+\n', ' +jsvalue.+\n', ' +js_.+\n', ' +jsobj_.+\n', ' +object_js_.*\n', ' +object_is_object_js_.*\n']
      for r in regex:
        reobj = re.compile(r)
        content, number = reobj.subn('', content)

  if helper.awtk.TARGET_ARCH == 'x86':
    with open('src/cpp_x86.def', 'r') as f:
      content += f.read()
  else:
    with open('src/cpp.def', 'r') as f:
      content += f.read()

  with open('src/mvvm.def', 'w') as f:
    f.write(content)

helper = app.Helper(ARGUMENTS);
helper.set_dll_def('src/mvvm.def').set_dll_def_processor(dll_def_processor).add_cpppath(TK_3RD_DIRS).add_ccflags(APP_CFLAGS).add_libs(APP_LIBS).call(DefaultEnvironment)

helper.SConscript(SConscripts)
