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
 * 2021-04-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TEMPERATURE_H
#define TK_TEMPERATURE_H

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/emitter.h"
#include "tkc/event.h"

BEGIN_C_DECLS

/**
 * @class temperature_t
 * @parent emitter_t
 * @annotation ["model"]
 * 用于演示模型触发界面更新。
 *
 *> 需要继承emitter_t!!
 */
typedef struct _temperature_t {
  emitter_t emitter;

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
 * @method temperature_set_value
 * 设置value。
 *
 * @param {temperature_t*} temperature temperature对象。
 * @param {double} value 温度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t temperature_set_value(temperature_t* temperature, double value);

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

#endif /*TK_TEMPERATURE_H*/
