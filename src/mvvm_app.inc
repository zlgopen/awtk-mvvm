#include "mvvm/mvvm.h"

#ifndef APP_JS_MAIN
#define APP_JS_MAIN "app"
#endif

#ifndef APP_JS_VALUE_CONVERTER
#define APP_JS_VALUE_CONVERTER "value_converter"
#endif

#ifndef APP_JS_VALUE_VALIDATOR
#define APP_JS_VALUE_VALIDATOR "value_validator"
#endif

static ret_t mvvm_app_init(void) {
  mvvm_init();

#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_init();
  if (mvvm_jerryscript_run(APP_JS_MAIN) != RET_OK) {
    mvvm_jerryscript_run(APP_JS_VALUE_CONVERTER);
    mvvm_jerryscript_run(APP_JS_VALUE_VALIDATOR);
  }
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
