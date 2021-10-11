#include "mvvm/mvvm.h"

#ifndef APP_SCRIPT_MAIN
#define APP_SCRIPT_MAIN "app"
#endif

#ifndef APP_SCRIPT_VALUE_CONVERTER
#define APP_SCRIPT_VALUE_CONVERTER "value_converter"
#endif

#ifndef APP_SCRIPT_VALUE_VALIDATOR
#define APP_SCRIPT_VALUE_VALIDATOR "value_validator"
#endif

#ifndef MVVM_APP_GLOBAL_INIT
#define MVVM_APP_GLOBAL_INIT()
#endif /*MVVM_APP_GLOBAL_INIT*/

#ifndef MVVM_APP_GLOBAL_DEINIT
#define MVVM_APP_GLOBAL_DEINIT()
#endif /*MVVM_APP_GLOBAL_DEINIT*/

static ret_t mvvm_app_run_default_scripts(void) {
#ifdef WITH_JERRYSCRIPT
  if (mvvm_jerryscript_run(APP_SCRIPT_MAIN) != RET_OK) {
    mvvm_jerryscript_run(APP_SCRIPT_VALUE_CONVERTER);
    mvvm_jerryscript_run(APP_SCRIPT_VALUE_VALIDATOR);
  }
#endif /*WITH_JERRYSCRIPT*/

  return RET_OK;
}

static ret_t mvvm_app_init(void) {
  mvvm_init();

#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_init();
#endif /*WITH_JERRYSCRIPT*/

  return RET_OK;
}

static ret_t mvvm_app_deinit(void) {
#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_deinit();
#endif /*WITH_JERRYSCRIPT*/

  mvvm_deinit();

  return RET_OK;
}
