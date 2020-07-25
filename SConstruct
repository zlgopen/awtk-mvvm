import os
import sys
import platform

def getAwtkRoot():
  #for pc
  awtk_root = '../awtk'
  #for linux-fb
  #awtk_root = '../awtk-linux-fb'
  return os.path.abspath(awtk_root)


AWTK_ROOT = getAwtkRoot()
sys.path.insert(0, AWTK_ROOT)
import awtk_config as awtk

APP_ROOT    = os.path.normpath(os.getcwd())
APP_SRC     = os.path.join(APP_ROOT, 'src')
APP_BIN_DIR = os.path.join(APP_ROOT, 'bin')
APP_LIB_DIR = os.path.join(APP_ROOT, 'lib')
RES_ROOT    = awtk.TK_DEMO_ROOT.replace("\\", "\\\\")
TK_JS_ROOT  = os.path.normpath(os.getcwd())
TK_JS_3RD_ROOT = os.path.join(TK_JS_ROOT, '3rd')

os.environ['APP_SRC'] = APP_SRC;
os.environ['APP_ROOT'] = APP_ROOT;
os.environ['BIN_DIR'] = APP_BIN_DIR;
os.environ['LIB_DIR'] = APP_LIB_DIR;
os.environ['BUILD_SHARED'] = str(awtk.isBuildShared())

TK_3RD_DIRS = [
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/include'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/arg'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/common'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/debugger'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/handler'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-ext/module'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/include'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/base'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/builtin-objects'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/operations'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/jcontext'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/jrt'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/parser'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/parser/js'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/parser/regexp'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/vm'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/api'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/debugger'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/lit'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/jmem'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/profiles'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-port/default/include'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerryscript/jerry-core/ecma/builtin-objects/typedarray'),
  os.path.join(TK_JS_3RD_ROOT, 'nativefiledialog/nativefiledialog/src/include'),
]

APP_LIBS = ['streams']
APP_LIBPATH = [APP_LIB_DIR, APP_BIN_DIR]
APP_CPPPATH = TK_3RD_DIRS + [APP_SRC, APP_ROOT]
APP_CFLAGS = '-DRES_ROOT=\"\\\"'+RES_ROOT+'\\\"\" '

os.environ['WITH_JS'] = 'true'

if 'WITH_JS' in os.environ:
  APP_LIBS += ['jerryscript']
  APP_CFLAGS +=' -DWITH_JERRYSCRIPT -DJERRY_DEBUGGER  -DJERRY_ESNEXT=0 '
  SConscripts = ['3rd/jerryscript/SConscript', 'src/SConscript', 'demos/SConscript', 'tests/SConscript']
else:
  SConscripts = ['src/SConscript', 'demos/SConscript']

APP_LINKFLAGS = ''
APP_CCFLAGS = APP_CFLAGS
if awtk.isBuildShared():
  AWTK_LIBS = awtk.SHARED_LIBS
  awtk.copySharedLib(AWTK_ROOT, APP_BIN_DIR, 'awtk');

  if awtk.OS_NAME == 'Linux':
    APP_LINKFLAGS += ' -Wl,-rpath=' + APP_BIN_DIR + ' '
else:
  AWTK_LIBS = awtk.STATIC_LIBS

if hasattr(awtk, 'CC'):
  DefaultEnvironment(
    CC=awtk.CC,
    CXX=awtk.CXX,
    LD=awtk.LD,
    AR=awtk.AR,
    STRIP=awtk.STRIP,
    
    CPPPATH   = APP_CPPPATH + awtk.CPPPATH,
    LINKFLAGS = awtk.LINKFLAGS + APP_LINKFLAGS,
    LIBS      = APP_LIBS + AWTK_LIBS,
    LIBPATH   = APP_LIBPATH + awtk.LIBPATH,
    CFLAGS    = APP_CFLAGS + awtk.CFLAGS, 
    CCFLAGS   = APP_CCFLAGS + awtk.CCFLAGS, 
    OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
    OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)
else:
  DefaultEnvironment(
    CPPPATH   = APP_CPPPATH + awtk.CPPPATH,
    LINKFLAGS = awtk.LINKFLAGS + APP_LINKFLAGS,
    LIBS      = APP_LIBS + AWTK_LIBS,
    LIBPATH   = APP_LIBPATH + awtk.LIBPATH,
    CFLAGS    = APP_CFLAGS + awtk.CFLAGS, 
    CCFLAGS   = APP_CCFLAGS + awtk.CCFLAGS, 
    OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
    OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS)

SConscript(SConscripts)

