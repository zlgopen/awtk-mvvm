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
#include "temperature_view_model.h"
#include "temperature_timer.h"

#define PROP_TEMP "value"
#define PROP_TIME "timer"

static ret_t on_timer(const timer_info_t* info) {
  object_t* view_model = OBJECT(info->ctx);

  int32_t time = object_get_prop_int(view_model, PROP_TIME, 0) - 1;
  int32_t temp = object_get_prop_int(view_model, PROP_TEMP, 0);
  if (temp < 100) {
    object_set_prop_int(view_model, PROP_TEMP, temp + 1);
  }
  object_set_prop_int(view_model, PROP_TIME, time);

  return time > 0 ? RET_REPEAT : RET_REMOVE;
}

view_model_t* temperature_view_model_timer_create(navigator_request_t* req) {
  view_model_t* view_model = temperature_view_model_create(req);

  timer_add(on_timer, view_model, 1000);
  object_set_prop_int(OBJECT(view_model), PROP_TEMP, 0);
  object_set_prop_int(OBJECT(view_model), PROP_TIME, 10);

  return view_model;
}
