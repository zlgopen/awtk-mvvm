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
#include "mvvm/jerryscript/mvvm_jerryscript.h"

ret_t mvvm_init(void) {
  return_value_if_fail(value_converter_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_validator_init() == RET_OK, RET_FAIL);
#ifdef WITH_JERRYSCRIPT
  return_value_if_fail(mvvm_jerryscript_init() == RET_OK, RET_FAIL);
#endif /*WITH_JERRYSCRIPT*/

  return RET_OK;
}

ret_t mvvm_deinit(void) {
  value_converter_deinit();
  value_validator_deinit();
#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_deinit();
#endif /*WITH_JERRYSCRIPT*/

  return RET_OK;
}
