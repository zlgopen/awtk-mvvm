/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "jerryscript-port.h"
#include "jerryscript-ext/handler.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/jerryscript_awtk.h"
#include "mvvm/jerryscript/mvvm_jerryscript.h"

const char* s_boot_code =
    "var ValueConverters = {};\n \
                           var ValueValidators = {};\n \
                           var console = { \n \
                            log: function(args) { \n \
                              print(args); \n \
                            } \n \
                           }; \n \
                           console.log('hello awtk'); \n \
                           ";

ret_t mvvm_jerryscript_init(void) {
  jerry_init(JERRY_INIT_EMPTY);
  jerryx_handler_register_global((const jerry_char_t*)"print", jerryx_handler_print);

  return_value_if_fail(value_validator_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_converter_jerryscript_init() == RET_OK, RET_FAIL);

  jerryscript_awtk_init();

  jerryscript_run("boot", s_boot_code, strlen(s_boot_code));

  return RET_OK;
}

jerry_value_t jerryscript_eval(const char* name, const char* code, uint32_t code_size) {
  jerry_value_t jsret;
  jerry_value_t jscode = 0;
  return_value_if_fail(code != NULL && code_size > 0, jerry_create_null());

  jscode = jerry_parse((const jerry_char_t*)name, strlen(name), (const jerry_char_t*)code,
                       code_size, JERRY_PARSE_NO_OPTS);
  jerry_value_check(jscode);

  jsret = jerry_run(jscode);
  jerry_value_check(jsret);

  jerry_release_value(jscode);

  return jsret;
}

ret_t jerryscript_run(const char* name, const char* code, uint32_t code_size) {
  jerry_value_t jsret;
  return_value_if_fail(code != NULL && code_size > 0, RET_BAD_PARAMS);

  jsret = jerryscript_eval(name, code, code_size);
  jerry_release_value(jsret);

  return RET_OK;
}

ret_t mvvm_jerryscript_deinit(void) {
  value_converter_jerryscript_deinit();
  value_validator_jerryscript_deinit();
  jerryscript_awtk_deinit();
  jerry_cleanup();

  return RET_OK;
}
