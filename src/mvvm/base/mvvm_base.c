/**
 * File:   mvvm.h
 * Author: BASE Develop Team
 * Brief:  mvvm base global functions.
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
 * 2019-02-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/base/mvvm_base.h"

ret_t mvvm_base_init(void) {
  return_value_if_fail(view_model_factory_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_converter_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_validator_init() == RET_OK, RET_FAIL);
  navigator_set(navigator_create());
  return_value_if_fail(navigator() != NULL, RET_FAIL);
  view_model_app_conf_register();

  return RET_OK;
}

ret_t mvvm_base_deinit(void) {
  view_model_factory_deinit();
  value_converter_deinit();
  value_validator_deinit();
  tk_object_unref(TK_OBJECT(navigator()));
  navigator_set(NULL);

  return RET_OK;
}
