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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "humidity_view_model.h"
#include "temperature_view_model.h"

ret_t application_init() {
  view_model_factory_register("temperature", temperature_view_model_create);
  view_model_factory_register("humidity", humidity_view_model_create);

  return navigator_to("temperature_humidity");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");

  return RET_OK;
}

#include "../main.inc"
