/**
 * File:   temperature_sensor.h
 * Author: AWTK Develop Team
 * Brief:  temperature_sensor
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_TEMPERATURE_SENSOR_H
#define TK_TEMPERATURE_SENSOR_H

#include "mvvm/hardware/device_object.h"

BEGIN_C_DECLS

struct _temperature_sensor_t;
typedef struct _temperature_sensor_t temperature_sensor_t;

/**
 * @class temperature_sensor_t
 * @parent device_object_t
 *
 * 温度传感器。
 */
struct _temperature_sensor_t {
  device_object_t device_object;

  /**
   * @property {double} value
   * @annotation ["get_prop"]
   * 最新的温度。
   */
  double value;

  /**
   * @property {int32_t} sample_interval
   * @annotation ["set_prop","get_prop"]
   * 采样时间间隔(ms)。
   */
  int32_t sample_interval;

  /*private*/
  uint32_t timer_id;
};

#define TEMPERATURE_SENSOR(object) ((temperature_sensor_t*)(object))

#define TEMPERATURE_SENSOR_TYPE "temperature_sensor"

#define TEMPERATURE_SENSOR_PROP_VALUE "value"
#define TEMPERATURE_SENSOR_PROP_SAMPLE_INTERVAL "sample_interval"

END_C_DECLS

#endif /*TK_TEMPERATURE_SENSOR_H*/
