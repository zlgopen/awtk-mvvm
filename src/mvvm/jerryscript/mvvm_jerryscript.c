/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/jerryscript_awtk.h"
#include "mvvm/jerryscript/mvvm_jerryscript.h"
#include "mvvm/jerryscript/jerry_script_helper.h"

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
  jerry_script_register_builtins();

  return_value_if_fail(value_validator_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_converter_jerryscript_init() == RET_OK, RET_FAIL);

  jerryscript_awtk_init();
  jerry_script_eval_buff(s_boot_code, strlen(s_boot_code), "mvvm_boot.js", TRUE);

  return RET_OK;
}

ret_t mvvm_jerryscript_deinit(void) {
  value_converter_jerryscript_deinit();
  value_validator_jerryscript_deinit();
  jerryscript_awtk_deinit();

  jerry_cleanup();

  return RET_OK;
}
