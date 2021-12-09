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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef HUMIDITY_H
#define HUMIDITY_H

#include "tkc/types_def.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class humidity_t
 * @parent tk_object_t
 * @annotation ["model"]
 * 湿度控制器。
 *
 */
typedef struct _humidity_t {
  tk_object_t object;

  /**
   * @property {double} humi
   * @annotation ["readable", "writable"]
   * 值。
   */
  double humi;

  /*private*/
  double saved_humi;
} humidity_t;

/**
 * @method humidity_create
 * 创建humidity对象。
 *
 * @annotation ["constructor"]
 * @return {tk_object_t*} 返回humidity对象。
 */
tk_object_t* humidity_create(void);

END_C_DECLS

#endif /*HUMIDITY_H*/
