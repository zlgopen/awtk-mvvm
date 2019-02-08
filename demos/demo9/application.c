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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"

#include "main.h"
#include "humidity.h"
#include "temperature.h"

ret_t application_init() {
  NAVIGATOR_HANDLER_WRAP("demo9_main", main_create);
  NAVIGATOR_HANDLER_WRAP("humidity", humidity_create);
  NAVIGATOR_HANDLER_WRAP("temperature9", temperature_create);

  navigator_to("demo9_main");

  return RET_OK;
}

#include "../awtk_main.c"
