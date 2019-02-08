/**
 * File:   application.c
 * Author: AWTK Develop Team
 * Brief:  application
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

#include "humidity.h"
#include "temperature.h"

ret_t application_init() {
  NAVIGATOR_ADD_HANDLER("demo9_main", NULL);
  NAVIGATOR_ADD_HANDLER("humidity", humidity_create);
  NAVIGATOR_ADD_HANDLER("temperature9", temperature_create);

  return navigator_to("demo9_main");
}

#include "../awtk_main.c"
