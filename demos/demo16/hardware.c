/**
 * File:   hardware.c
 * Author: AWTK Develop Team
 * Brief:  hardware
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "awtk.h"

#include "base/widget_factory.h"
#include "mvvm/hardware/buzzer_log.h"
#include "mvvm/awtk/widget_hardware.h"
#include "mvvm/hardware/device_factory.h"
#include "mvvm/hardware/temperature_sensor_random.h"

widget_t* widget_buzzer_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_hardware_create(parent, x, y, w, h, BUZZER_TYPE, NULL);
}

widget_t* widget_temperature_sensor_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_hardware_create(parent, x, y, w, h, TEMPERATURE_SENSOR_TYPE, NULL);
}

ret_t hardware_init(void) {
  device_factory_init();
  device_factory_register(BUZZER_TYPE, buzzer_log_create);
  device_factory_register(TEMPERATURE_SENSOR_TYPE, temperature_sensor_random_create);

  widget_factory_register(widget_factory(), BUZZER_TYPE, widget_buzzer_create);
  widget_factory_register(widget_factory(), TEMPERATURE_SENSOR_TYPE,
                          widget_temperature_sensor_create);

  return RET_OK;
}
