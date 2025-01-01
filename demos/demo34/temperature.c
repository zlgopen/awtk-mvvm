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
 * 2021-04-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/events.h"
#include "temperature.h"

static ret_t on_timer(const timer_info_t* info) {
  temperature_t* temperature = (temperature_t*)(info->ctx);

  temperature_set_value(temperature, temperature->value + 1);

  return temperature->value > 10 ? RET_REMOVE : RET_REPEAT;
}

temperature_t* temperature_create(void) {
  temperature_t* temperature = TKMEM_ZALLOC(temperature_t);
  return_value_if_fail(temperature != NULL, NULL);
  emitter_init(EMITTER(temperature));

  timer_add(on_timer, temperature, 1000);

  return temperature;
}

ret_t temperature_set_value(temperature_t* temperature, double value) {
  return_value_if_fail(temperature != NULL, RET_BAD_PARAMS);
  temperature->value = value;
  emitter_dispatch_simple_event(EMITTER(temperature), EVT_PROPS_CHANGED);

  return RET_OBJECT_CHANGED;
}

ret_t temperature_destroy(temperature_t* temperature) {
  return_value_if_fail(temperature != NULL, RET_BAD_PARAMS);
  emitter_deinit(EMITTER(temperature));
  TKMEM_FREE(temperature);

  return RET_OK;
}
