/**
 * File:   humidity.h
 * Author: AWTK Develop Team
 * Brief:  humidity
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

#ifndef HUMIDITY_H
#define HUMIDITY_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class humidity_t
 *
 * @annotation ["model"]
 * humidy控制器。
 *
 */
typedef struct _humidity_t {
  /**
   * @property {double} humi
   * @annotation ["readable", "writable"]
   * 值。
   */
  double humi;
} humidity_t;

END_C_DECLS

#endif /*HUMIDITY_H*/
