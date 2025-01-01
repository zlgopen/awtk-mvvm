/**
 * File:   temperature_sensor_random.h
 * Author: AWTK Develop Team
 * Brief:  temperature_sensor_random
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_TEMPERATURE_SENSOR_RANDOM_H
#define TK_TEMPERATURE_SENSOR_RANDOM_H

#include "tkc/object.h"
#include "mvvm/hardware/temperature_sensor.h"

BEGIN_C_DECLS

struct _temperature_sensor_random_t;
typedef struct _temperature_sensor_random_t temperature_sensor_random_t;

/**
 * @class temperature_sensor_random_t
 * @parent tk_object_t
 *
 * 模拟的温度传感器，随机生成0-100之间的温度。
 *
 */
struct _temperature_sensor_random_t {
  temperature_sensor_t temperature_sensor;
};

/**
 * @method temperature_sensor_random_create
 * 创建temperature_sensor对象。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* temperature_sensor_random_create(const char* args);

#define TEMPERATURE_SENSOR_RANDOM(object) ((temperature_sensor_random_t*)(object))

END_C_DECLS

#endif /*TK_TEMPERATURE_SENSOR_RANDOM_H*/
