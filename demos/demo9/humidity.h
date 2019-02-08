/**
 * File:   humidity.h
 * Author: AWTK Develop Team
 * Brief:  humidity
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HUMIDITY_H
#define TK_HUMIDITY_H

#include "mvvm/base/model.h"

BEGIN_C_DECLS

/**
 * @class humidity_t
 *
 * 湿度对象。
 *
 */
typedef struct _humidity_t {
  model_t model;

  double value;
} humidity_t;

/**
 * @method humidity_create
 * 创建humidity对象。
 *
 * @annotation ["constructor"]
 * @param {void*} args 参数(目前没有，仅仅为了于model_create_t保持一致)
 *
 * @return {model_t} 返回model_t对象。
 */
model_t* humidity_create(void* args);

#define HUMIDITY(t) ((humidity_t*)(t))

END_C_DECLS

#endif /*TK_HUMIDITY_H*/
