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

#include "temperature_ex.h"

#define PROP_TEMP "temp"
#define CMD_SAVE "save"

static ret_t temperature_ex_set_prop(object_t* obj, const char* name, const value_t* v) {
  temperature_ex_t* t = TEMPERATURE_EX(obj);

  if (tk_str_eq(PROP_TEMP, name)) {
    t->value = value_float(v);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t temperature_ex_get_prop(object_t* obj, const char* name, value_t* v) {
  temperature_ex_t* t = TEMPERATURE_EX(obj);

  if (tk_str_eq(PROP_TEMP, name)) {
    value_set_double(v, t->value);

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static bool_t temperature_ex_can_exec(object_t* obj, const char* name, const char* args) {
  temperature_ex_t* t = TEMPERATURE_EX(obj);

  if (tk_str_ieq(name, CMD_SAVE)) {
    return t->value != t->saved_value;
  } else {
    log_debug("not supported\n");
  }

  return FALSE;
}

static ret_t temperature_ex_exec(object_t* obj, const char* name, const char* args) {
  temperature_ex_t* t = TEMPERATURE_EX(obj);

  if (tk_str_ieq(name, CMD_SAVE)) {
    log_debug("save\n");
    t->saved_value = t->value;
  } else {
    log_debug("not supported\n");
    return RET_NOT_FOUND;
  }

  object_notify_changed(obj);

  return RET_OK;
}

static const object_vtable_t s_temperature_ex_vtable = {.type = "temperature",
                                                        .desc = "temperature",
                                                        .size = sizeof(temperature_ex_t),
                                                        .exec = temperature_ex_exec,
                                                        .can_exec = temperature_ex_can_exec,
                                                        .get_prop = temperature_ex_get_prop,
                                                        .set_prop = temperature_ex_set_prop};

model_t* temperature_ex_create(navigator_request_t* req) {
  return MODEL(object_create(&s_temperature_ex_vtable));
}
