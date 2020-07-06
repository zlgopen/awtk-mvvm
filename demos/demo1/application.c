/**
 * File:   application.c
 * Author: AWTK Develop Team
 * Brief:  application
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "temperature_view_model.h"

ret_t application_init(void) {
  view_model_factory_register("temperature", temperature_view_model_create);

  return navigator_to("temperature1");
}

#include "../awtk_main.c"
