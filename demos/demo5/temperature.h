﻿/**
 * File:   temperature.h
 * Author: AWTK Develop Team
 * Brief:  temperature
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class temperature_t
 *
 * @annotation ["model"]
 * 温度控制器。
 *
 */
typedef struct _temperature_t {
  /**
   * @property {double} value
   * @annotation ["readable", "writable"]
   * 值。
   */
  double value;
} temperature_t;

/**
 * @method temperature_create
 * 创建temperature对象。
 *
 * @annotation ["constructor"]
 * @return {temperature_t*} 返回temperature对象。
 */
temperature_t* temperature_create(void);

/**
 * @method temperature_destroy
 * 销毁temperature对象。
 *
 * @annotation ["destructor"]
 * @param {temperature_t*} temperature temperature对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t temperature_destroy(temperature_t* temperature);

END_C_DECLS

#endif /*TEMPERATURE_H*/
