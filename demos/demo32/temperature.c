/**
 * File:   temperature.c
 * Author: AWTK Develop Team
 * Brief:  temperature
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "temperature.h"

static bool_t temperature_is_modified(temperature_t* temperature) {
  return_value_if_fail(temperature != NULL, FALSE);

  return temperature->saved_temp != temperature->temp;
}

static ret_t temperature_apply(temperature_t* temperature) {
  return_value_if_fail(temperature != NULL, RET_BAD_PARAMS);

  temperature->saved_temp = temperature->temp;
  return RET_OBJECT_CHANGED;
}

static ret_t temperature_reset(temperature_t* temperature) {
  return_value_if_fail(temperature != NULL, RET_BAD_PARAMS);

  temperature->temp = temperature->saved_temp;

  return RET_OBJECT_CHANGED;
}

static ret_t temperature_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  temperature_t* temperature = (temperature_t*)obj;

  if (tk_str_ieq("temp", name)) {
    value_set_double(v, temperature->temp);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}
static ret_t temperature_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  temperature_t* temperature = (temperature_t*)obj;

  if (tk_str_ieq("temp", name)) {
    temperature->temp = value_double(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t temperature_can_exec(tk_object_t* obj, const char* name, const char* args) {
  temperature_t* temperature = (temperature_t*)obj;

  if (tk_str_ieq("apply", name)) {
    return temperature_is_modified(temperature);
  } else if (tk_str_ieq("reset", name)) {
    return temperature_is_modified(temperature);
  }

  return FALSE;
}

static ret_t temperature_exec(tk_object_t* obj, const char* name, const char* args) {
  temperature_t* temperature = (temperature_t*)obj;

  if (tk_str_ieq("apply", name)) {
    return temperature_apply(temperature);
  } else if (tk_str_ieq("reset", name)) {
    return temperature_reset(temperature);
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_temperature_vtable = {.type = "temperature",
                                                     .desc = "temperature",
                                                     .size = sizeof(temperature_t),
                                                     .get_prop = temperature_get_prop,
                                                     .set_prop = temperature_set_prop,
                                                     .can_exec = temperature_can_exec,
                                                     .exec = temperature_exec};

tk_object_t* temperature_create(void) {
  tk_object_t* obj = tk_object_create(&s_temperature_vtable);

  return obj;
}
