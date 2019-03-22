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
 * 2019-02-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/view_model_delegate.h"

#include "temperature.h"

static ret_t temperature_save(temperature_t* t, const char* args) {
  log_debug("save\n");
  t->saved_value = t->value;

  return RET_OBJECT_CHANGED;
}

static bool_t temperature_can_save(temperature_t* t, const char* args) {
  return t->value != t->saved_value;
}

view_model_t* temperature_create(navigator_request_t* req) {
  temperature_t* t = TKMEM_ZALLOC(temperature_t);
  view_model_t* view_model = view_model_delegate_create(t, default_destroy);

  VIEW_MODEL_SIMPLE_PROP(view_model, "temp", VALUE_TYPE_INT32, &(t->value));
  VIEW_MODEL_COMMAND(view_model, "save", temperature_save, temperature_can_save);

  return view_model;
}
