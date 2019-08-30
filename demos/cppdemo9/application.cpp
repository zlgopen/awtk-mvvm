/**
 * File:   application.cpp
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"

#include "../cppcommon/humidity.hpp"
#include "../cppcommon/temperature.hpp"

ret_t application_init() {
  view_model_factory_register("humidity", humidity_view_model_create);
  view_model_factory_register("temperature", temperature_view_model_create);

  return navigator_to("demo9_main");
}

#include "../awtk_main.c"
