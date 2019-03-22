/**
 * File:   temperature.h
 * Author: AWTK Develop Team
 * Brief:  temperature
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

#ifndef TK_TEMPERATURE_H
#define TK_TEMPERATURE_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class temperature_t
 *
 * 温度对象。
 *
 */
typedef struct _temperature_t {
  view_model_t view_model;

  int32_t value;

  int32_t saved_value;
} temperature_t;

/**
 * @method temperature_create
 * 创建temperature对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* temperature_create(navigator_request_t* req);

#define TEMPERATURE(t) ((temperature_t*)(t))

END_C_DECLS

#endif /*TK_TEMPERATURE_H*/
