import os
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

APP_CFLAGS = '-DMVVM_DLL_EXPORT '
APP_LIBS = ['streams']
if 'WITH_JS' in os.environ:
  APP_LIBS += ['jerryscript']
  APP_CFLAGS +=' -DWITH_JERRYSCRIPT -DJERRY_DEBUGGER  -DJERRY_ESNEXT=0 '
  SConscripts = ['3rd/jerryscript/SConscript', 
    'src/SConscript', 
    'demos/SConscript', 
    'src/run_js_mvvm/SConscript', 
    'tests/SConscript']
else:
  SConscripts = ['src/SConscript', 'demos/SConscript']

helper = app.Helper(ARGUMENTS);
helper.set_dll_def('src/mvvm.def').add_cpppath(TK_3RD_DIRS).add_ccflags(APP_CFLAGS).add_libs(APP_LIBS).call(DefaultEnvironment)

SConscript(SConscripts)
