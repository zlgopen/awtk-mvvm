/**
 * File:   temperature_view.c
 * Author: AWTK Develop Team
 * Brief:  temperature view
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "temperature.h"
#include "temperature_validator.h"

ret_t application_init() {
  temperature_validator_init();

  vm_open_window("temperature5", temperature_create());

  return RET_OK;
}

#include "../awtk_main.c"
