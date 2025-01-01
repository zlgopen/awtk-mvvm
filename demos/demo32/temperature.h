/**
 * File:   temperature.h
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

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "tkc/types_def.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class temperature_t
 * @parent tk_object_t
 * @annotation ["model"]
 * 温度控制器。
 *
 */
typedef struct _temperature_t {
  tk_object_t object;

  /**
   * @property {double} temp
   * @annotation ["readable", "writable"]
   * 值。
   */
  double temp;

  /*private*/
  double saved_temp;
} temperature_t;

/**
 * @method temperature_create
 * 创建temperature对象。
 *
 * @annotation ["constructor"]
 * @return {tk_object_t*} 返回temperature对象。
 */
tk_object_t* temperature_create(void);

END_C_DECLS

#endif /*TEMPERATURE_H*/
