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

#include "../cppcommon/home.hpp"
#include "../cppcommon/room_settings.hpp"

ret_t application_init() {
  view_model_factory_register("home", home_view_model_create);
  view_model_factory_register("room_settings", room_settings_view_model_create);

  return navigator_to("home");
}

#include "../awtk_main.c"
