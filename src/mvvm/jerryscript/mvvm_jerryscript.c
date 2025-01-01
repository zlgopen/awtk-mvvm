/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/view_model_jerryscript.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"
#include "mvvm/jerryscript/jerryscript_awtk.h"
#include "mvvm/jerryscript/mvvm_jerryscript.h"

ret_t mvvm_jerryscript_init(void) {
  jerry_script_init();
  jerry_script_register_mvvm_factory();

  return_value_if_fail(value_validator_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_converter_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(view_model_jerryscript_init() == RET_OK, RET_FAIL);

  jerryscript_awtk_init();

  return RET_OK;
}

ret_t mvvm_jerryscript_run(const char* filename) {
  return jerry_script_eval_file(filename, TRUE);
}

ret_t mvvm_jerryscript_deinit(void) {
  jerryscript_awtk_deinit();
  value_converter_jerryscript_deinit();
  value_validator_jerryscript_deinit();
  view_model_jerryscript_deinit();
  jerry_script_deinit();

  return RET_OK;
}
