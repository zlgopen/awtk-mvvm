import os
import platform

OS_NAME=platform.system()
APP_ROOT = os.path.normpath(os.getcwd())
APP_SRC= os.path.join(APP_ROOT, 'src')
APP_3RD_ROOT = os.path.join(APP_ROOT, '3rd')
APP_BIN_DIR=os.path.join(APP_ROOT, 'bin')
APP_LIB_DIR=os.path.join(APP_ROOT, 'lib')
TK_JS_3RD_ROOT = APP_3RD_ROOT

TK_ROOT = os.path.normpath(os.getcwd()+'/../awtk')
TK_SRC = os.path.join(TK_ROOT, 'src')
TK_3RD_ROOT = os.path.join(TK_ROOT, '3rd')
TK_BIN_DIR=os.path.join(TK_ROOT, 'bin')
TK_LIB_DIR=os.path.join(TK_ROOT, 'lib')
GTEST_ROOT = os.path.join(TK_ROOT, '3rd/gtest/googletest')

os.environ['APP_ROOT'] = APP_ROOT;
os.environ['BIN_DIR'] = APP_BIN_DIR;
os.environ['LIB_DIR'] = APP_LIB_DIR;

os.environ['TK_ROOT'] = TK_ROOT;
os.environ['TK_BIN_DIR'] = TK_BIN_DIR;
os.environ['TK_LIB_DIR'] = TK_LIB_DIR;
os.environ['GTEST_ROOT'] = GTEST_ROOT;
os.environ['TK_3RD_ROOT'] = TK_3RD_ROOT;
os.environ['APP_SRC'] = APP_SRC;

OS_LIBPATH=[]
OS_CPPPATH=[]
OS_FLAGS='-g -Wall'
OS_SUBSYSTEM_CONSOLE=''
OS_SUBSYSTEM_WINDOWS=''
OS_LINKFLAGS=''

COMMON_CCFLAGS = ' -DHAS_STD_MALLOC -DHAS_STDIO -DWITH_SDL -DWITH_FS_RES -DWITH_STB_IMAGE -DWITH_STB_FONT '
if OS_NAME == 'Darwin':
  OS_LIBS = ['stdc++', 'pthread', 'm', 'dl']
  OS_LINKFLAGS='-framework OpenGL'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -D__APPLE__ -DHAS_PTHREAD -DMACOS -DENABLE_MEM_LEAK_CHECK1 '
  COMMON_CCFLAGS=COMMON_CCFLAGS+' -DENABLE_CURSOR '

elif OS_NAME == 'Linux':
  OS_LIBS = ['GL', 'gtk-3','gdk-3','Xext', 'X11', 'sndio','stdc++', 'pthread', 'm', 'dl']
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DLINUX -DHAS_PTHREAD'
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_TIMER_UNIX -DSDL_VIDEO_DRIVER_X11 -DSDL_VIDEO_DRIVER_X11_SUPPORTS_GENERIC_EVENTS '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_AUDIO_DRIVER_SNDIO -DSDL_VIDEO_OPENGL_GLX -DSDL_VIDEO_RENDER_OGL '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_LOADSO_DLOPEN -DSDL_VIDEO_OPENGL_EGL -DSDL_VIDEO_OPENGL_ES2 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '

elif OS_NAME == 'Windows':
  OS_LIBS=['gdi32', 'user32','winmm.lib','imm32.lib','version.lib','shell32.lib','ole32.lib','Oleaut32.lib','Advapi32.lib']
  OS_FLAGS='-DWIN32 -D_WIN32 -DWINDOWS /EHsc -D_CONSOLE  /DEBUG /Od  /FS /Z7 '
  COMMON_CCFLAGS = COMMON_CCFLAGS + ' -DSDL_REAL_API -DSDL_HAPTIC_DISABLED -DSDL_SENSOR_DISABLED -DSDL_JOYSTICK_DISABLED '
  OS_LINKFLAGS='/MACHINE:X64 /DEBUG'
  OS_SUBSYSTEM_CONSOLE='/SUBSYSTEM:CONSOLE  '
  OS_SUBSYSTEM_WINDOWS='/SUBSYSTEM:WINDOWS  '
  
LIBS=['mvvm', 'awtk', 'gpinyin', 'awtk', 'linebreak', 'nanovg', 'SDL2', 'glad'] + OS_LIBS

CCFLAGS=OS_FLAGS + COMMON_CCFLAGS 

TK_JS_JERRYSCRIPT_DIRS = [
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-ext/include'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-ext/arg'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-ext/common'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-ext/debugger'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-ext/handler'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-ext/module'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/include'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/ecma'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/ecma/base'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/ecma/builtin-objects'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/ecma/operations'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/jcontext'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/jrt'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/parser'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/parser/js'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/parser/regexp'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/vm'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/api'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/debugger'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/lit'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/jmem'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/profiles'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-port/default/include'),
  os.path.join(TK_JS_3RD_ROOT, 'jerryscript/jerry-core/ecma/builtin-objects/typedarray'),
]

CPPPATH=[
  APP_ROOT,
  TK_ROOT, 
  TK_SRC, 
  TK_3RD_ROOT, 
  APP_SRC,
  APP_3RD_ROOT,
  os.path.join(TK_SRC, 'ext_widgets')] + TK_JS_JERRYSCRIPT_DIRS;

LIBPATH = [TK_LIB_DIR, APP_LIB_DIR]

DefaultEnvironment(CCFLAGS = CCFLAGS, 
  CPPPATH = CPPPATH,
  LIBS=LIBS,
  LINKFLAGS=OS_LINKFLAGS,
  OS_SUBSYSTEM_CONSOLE=OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=OS_SUBSYSTEM_WINDOWS,
  LIBPATH=LIBPATH + OS_LIBPATH)

SConscript(['3rd/SConscript', 'src/SConscript', 'demos/SConscript', 'tests/SConscript'])

