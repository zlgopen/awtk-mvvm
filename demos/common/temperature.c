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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"

#include "temperature.h"
#define PROP_TEMP "temp"

static ret_t temperature_set_prop(object_t* obj, const char* name, const value_t* v) {
  temperature_t* t = TEMPERATURE(obj);

  if (tk_str_eq(PROP_TEMP, name)) {
    t->value = value_float(v);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t temperature_get_prop(object_t* obj, const char* name, value_t* v) {
  temperature_t* t = TEMPERATURE(obj);

  if (tk_str_eq(PROP_TEMP, name)) {
    value_set_double(v, t->value);

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t temperature_on_destroy(object_t* obj) {
  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_temperature_vtable = {.type = "temperature",
                                                     .desc = "temperature",
                                                     .size = sizeof(temperature_t),
                                                     .get_prop = temperature_get_prop,
                                                     .set_prop = temperature_set_prop,
                                                     .on_destroy = temperature_on_destroy};

view_model_t* temperature_create(navigator_request_t* req) {
  return view_model_init(VIEW_MODEL(object_create(&s_temperature_vtable)));
}
