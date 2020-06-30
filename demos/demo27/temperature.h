/**
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
   * @property {double} temp
   * @annotation ["readable", "writable"]
   * 值。
   */
  double temp;
} temperature_t;

END_C_DECLS

#endif /*TEMPERATURE_H*/
