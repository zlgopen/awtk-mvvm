/**
 * File:   temperature_sensor_random.c
 * Author: AWTK Develop Team
 * Brief:  temperature_sensor_random
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
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/events.h"
#include "temperature_sensor_random.h"

static ret_t temperature_sensor_random_on_destroy(object_t* obj) {
  temperature_sensor_t* temperature_sensor = TEMPERATURE_SENSOR(obj);

  timer_remove(temperature_sensor->timer_id);
  temperature_sensor->timer_id = TK_INVALID_ID;

  return RET_OK;
}

static ret_t temperature_sensor_random_set_prop(object_t* obj, const char* name, const value_t* v) {
  temperature_sensor_t* temperature_sensor = TEMPERATURE_SENSOR(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TEMPERATURE_SENSOR_PROP_SAMPLE_INTERVAL)) {
    int32_t interval = value_int(v);

    if (interval > 0) {
      timer_modify(temperature_sensor->timer_id, interval);
    } else {
      timer_modify(temperature_sensor->timer_id, 0xffffffff);
    }
    temperature_sensor->sample_interval = interval;

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t temperature_sensor_sample(object_t* obj) {
  event_t e = event_init(EVT_VALUE_CHANGED, obj);
  temperature_sensor_t* temperature_sensor = TEMPERATURE_SENSOR(obj);

  temperature_sensor->value = random() % 100;

  emitter_dispatch(EMITTER(obj), &e);

  return RET_REPEAT;
}

static ret_t temperature_sensor_on_timer(const timer_info_t* info) {
  temperature_sensor_sample(OBJECT(info->ctx));

  return RET_REPEAT;
}

static ret_t temperature_sensor_random_get_prop(object_t* obj, const char* name, value_t* v) {
  temperature_sensor_t* temperature_sensor = TEMPERATURE_SENSOR(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TEMPERATURE_SENSOR_PROP_SAMPLE_INTERVAL)) {
    value_set_int(v, temperature_sensor->sample_interval);
    return RET_OK;
  } else if (tk_str_eq(name, TEMPERATURE_SENSOR_PROP_VALUE)) {
    value_set_int(v, temperature_sensor->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_temperature_sensor_random_vtable = {
    .type = "temperature_sensor_random",
    .desc = "temperature_sensor_random",
    .size = sizeof(temperature_sensor_random_t),
    .is_collection = FALSE,
    .on_destroy = temperature_sensor_random_on_destroy,
    .get_prop = temperature_sensor_random_get_prop,
    .set_prop = temperature_sensor_random_set_prop};

object_t* temperature_sensor_random_create(const char* args) {
  object_t* obj = object_create(&s_temperature_sensor_random_vtable);
  temperature_sensor_t* temperature_sensor = TEMPERATURE_SENSOR(obj);
  return_value_if_fail(temperature_sensor != NULL, NULL);

  temperature_sensor_sample(obj);
  temperature_sensor->timer_id = timer_add(temperature_sensor_on_timer, obj, 5000);

  return obj;
}
