/**
 * File:   humidity.c
 * Author: AWTK Develop Team
 * Brief:  humidity
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"

#include "humidity.h"
#define PROP_HUMIDITY "humidity"

static ret_t humidity_set_prop(object_t* obj, const char* name, const value_t* v) {
  humidity_t* t = HUMIDITY(obj);

  if (tk_str_eq(PROP_HUMIDITY, name)) {
    t->value = value_float(v);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t humidity_get_prop(object_t* obj, const char* name, value_t* v) {
  humidity_t* t = HUMIDITY(obj);

  if (tk_str_eq(PROP_HUMIDITY, name)) {
    value_set_double(v, t->value);

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static const object_vtable_t s_humidity_vtable = {.type = "humidity",
                                                  .desc = "humidity",
                                                  .size = sizeof(humidity_t),
                                                  .get_prop = humidity_get_prop,
                                                  .set_prop = humidity_set_prop};

model_t* humidity_create(navigator_request_t* req) {
  return MODEL(object_create(&s_humidity_vtable));
}
