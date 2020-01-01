/**
 * File:   temperature.cpp
 * Author: AWTK Develop Team
 * Brief:  temperature
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "../cppcommon/temperature.hpp"

#include "temperature.hpp"

#define PROP_TEMP "value"

static ret_t on_timer(const timer_info_t* info) {
  object_t* view_model = OBJECT(info->ctx);

  int32_t temp = object_get_prop_int(view_model, PROP_TEMP, 0) + 1;
  object_set_prop_int(view_model, PROP_TEMP, temp);

  return temp < 10 ? RET_REPEAT : RET_REMOVE;
}

view_model_t* temperature_view_model_timer_create(navigator_request_t* req) {
  view_model_t* view_model = temperature_view_model_create(req);

  timer_add(on_timer, view_model, 1000);
  object_set_prop_int(OBJECT(view_model), PROP_TEMP, 0);

  return view_model;
}
