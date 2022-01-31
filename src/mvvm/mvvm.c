﻿/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "awtk.h"
#include "mvvm/mvvm.h"
#include "mvvm/base/mvvm_base.h"
#include "mvvm/base/custom_binder.h"

ret_t mvvm_init(void) {
  mvvm_base_init();
  custom_binder_init();

  log_debug("mvvm_init\n");
  return_value_if_fail(mvvm_awtk_init() == RET_OK, RET_FAIL);

  return RET_OK;
}

ret_t mvvm_deinit(void) {
  mvvm_awtk_deinit();
  mvvm_base_deinit();
  custom_binder_deinit();

  log_debug("mvvm_deinit\n");

  return RET_OK;
}
