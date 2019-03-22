/**
 * File:   shape.h
 * Author: AWTK Develop Team
 * Brief:  shape
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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SHAPE_H
#define TK_SHAPE_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class shape_t
 *
 * 温度对象。
 *
 */
typedef struct _shape_t {
  view_model_t view_model;

  int32_t type;
  int32_t x;
  int32_t y;
  int32_t w;
  int32_t h;
  char* name;
  int32_t opacity;
  int32_t align;

  str_t overview;
} shape_t;

/**
 * @method shape_create
 * 创建shape对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* shape_create(navigator_request_t* req);

#define SHAPE(t) ((shape_t*)(t))

END_C_DECLS

#endif /*TK_SHAPE_H*/
