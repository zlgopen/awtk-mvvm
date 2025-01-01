/**
 * File:   humidity.c
 * Author: AWTK Develop Team
 * Brief:  humidity
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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "humidity.h"

static bool_t humidity_is_modified(humidity_t* humidity) {
  return_value_if_fail(humidity != NULL, FALSE);

  return humidity->saved_humi != humidity->humi;
}

static ret_t humidity_apply(humidity_t* humidity) {
  return_value_if_fail(humidity != NULL, RET_BAD_PARAMS);

  humidity->saved_humi = humidity->humi;
  return RET_OBJECT_CHANGED;
}

static ret_t humidity_reset(humidity_t* humidity) {
  return_value_if_fail(humidity != NULL, RET_BAD_PARAMS);

  humidity->humi = humidity->saved_humi;
  return RET_OBJECT_CHANGED;
}

static ret_t humidity_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  humidity_t* humidity = (humidity_t*)obj;

  if (tk_str_ieq("humi", name)) {
    value_set_double(v, humidity->humi);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}
static ret_t humidity_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  humidity_t* humidity = (humidity_t*)obj;

  if (tk_str_ieq("humi", name)) {
    humidity->humi = value_double(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t humidity_can_exec(tk_object_t* obj, const char* name, const char* args) {
  humidity_t* humidity = (humidity_t*)obj;

  if (tk_str_ieq("apply", name)) {
    return humidity_is_modified(humidity);
  } else if (tk_str_ieq("reset", name)) {
    return humidity_is_modified(humidity);
  }

  return FALSE;
}

static ret_t humidity_exec(tk_object_t* obj, const char* name, const char* args) {
  humidity_t* humidity = (humidity_t*)obj;

  if (tk_str_ieq("apply", name)) {
    return humidity_apply(humidity);
  } else if (tk_str_ieq("reset", name)) {
    return humidity_reset(humidity);
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_humidity_vtable = {.type = "humidity",
                                                  .desc = "humidity",
                                                  .size = sizeof(humidity_t),
                                                  .get_prop = humidity_get_prop,
                                                  .set_prop = humidity_set_prop,
                                                  .can_exec = humidity_can_exec,
                                                  .exec = humidity_exec};

tk_object_t* humidity_create(void) {
  tk_object_t* obj = tk_object_create(&s_humidity_vtable);

  return obj;
}
