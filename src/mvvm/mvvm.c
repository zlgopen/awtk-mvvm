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
#include "mvvm/base/mvvm_base.h"

ret_t mvvm_init(void) {
  mvvm_base_init();

#ifdef WITH_JERRYSCRIPT
  return_value_if_fail(mvvm_jerryscript_init() == RET_OK, RET_FAIL);
#endif /*WITH_JERRYSCRIPT*/
  return_value_if_fail(mvvm_awtk_init() == RET_OK, RET_FAIL);

  return RET_OK;
}

ret_t mvvm_deinit(void) {
#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_deinit();
#endif /*WITH_JERRYSCRIPT*/
  mvvm_awtk_deinit();
  mvvm_base_deinit();

  return RET_OK;
}
