/**
 * File:   application.cpp
 * Author: AWTK Develop Team
 * Brief:  application
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "temperature_view_model.h"
#include "value_converter.hpp"

static ret_t value_converters_init(void) {
  value_converter_register("fahrenheit", TemperatureConverter::Create);

  return RET_OK;
}

ret_t application_init() {
  value_converters_init();
  view_model_factory_register("temperature_ex", temperature_view_model_create);

  return navigator_to("temperature3");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");

  return RET_OK;
}

#include "../main.inc"
