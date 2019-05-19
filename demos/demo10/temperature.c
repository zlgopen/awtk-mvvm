/**
 * File:   temperature.c
 * Author: AWTK Develop Team
 * Brief:  temperature
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/base/view_model_delegate.h"

#include "temperature.h"

#define PROP_TEMP "temp"
#define CMD_QUIT "quit"

static ret_t on_timer(const timer_info_t* info) {
  object_t* view_model = OBJECT(info->ctx);

  int32_t temp = object_get_prop_int(view_model, PROP_TEMP, 0) + 1;
  object_set_prop_int(view_model, PROP_TEMP, temp);

  return temp < 10 ? RET_REPEAT : RET_REMOVE;
}

static ret_t temperature_quit(temperature_t* t, const char* args) {
  tk_quit();

  return RET_OK;
}

view_model_t* temperature_view_model_create(navigator_request_t* req) {
  temperature_t* t = TKMEM_ZALLOC(temperature_t);
  view_model_t* view_model = view_model_delegate_create(t, default_destroy);

  VIEW_MODEL_SIMPLE_PROP(view_model, PROP_TEMP, VALUE_TYPE_DOUBLE, &(t->value));
  VIEW_MODEL_COMMAND(view_model, CMD_QUIT, temperature_quit, NULL);

  timer_add(on_timer, view_model, 1000);

  return view_model;
}
