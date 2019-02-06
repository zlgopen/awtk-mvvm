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

#include "mvvm/mvvm.h"

ret_t mvvm_jerryscript_init(void) {
  return_value_if_fail(model_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_validator_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_converter_jerryscript_init() == RET_OK, RET_FAIL);

  return RET_OK;
}

ret_t jerryscript_run(const char* name, const char* code, uint32_t code_size) {
  ret_t ret = RET_FAIL;
  jerry_value_t jscode = 0;
  jerry_value_t jsret = 0;
  return_value_if_fail(code != NULL && code_size > 0, RET_BAD_PARAMS);

  jscode = jerry_parse((const jerry_char_t*)name, strlen(name), (const jerry_char_t*)code,
                       code_size, JERRY_PARSE_NO_OPTS);
  jsret = jerry_run(jscode);
  ret = jerry_value_check(jsret);

  jerry_release_value(jscode);
  jerry_release_value(jsret);

  return ret;
}

ret_t mvvm_jerryscript_deinit(void) {
  model_jerryscript_deinit();
  value_converter_jerryscript_deinit();
  value_validator_jerryscript_deinit();

  return RET_OK;
}
