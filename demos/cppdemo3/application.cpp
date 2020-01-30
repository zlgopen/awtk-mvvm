/**
 * File:   application.cpp
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
#include "temperature_view_model.h"
#include "temperature_converter.hpp"

ret_t application_init() {
  temperature_converter_init();
  view_model_factory_register("temperature", temperature_view_model_create);

  return navigator_to("temperature3");
}

#include "../awtk_main.c"
