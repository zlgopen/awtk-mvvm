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

BEGIN_C_DECLS

/**
 * @class humidity_t
 *
 * 湿度控制器。
 *
 */
typedef struct _humidity_t {
  /**
   * @property {double} value
   * @annotation ["readable", "writable"]
   * 值。
   */
  double value;

  /*private*/
  double saved_value;
} humidity_t;

/**
 * @method humidity_create
 * 创建humidity对象。
 *
 * @annotation ["constructor"]
 * @return {humidity_t*} 返回humidity对象。 
 */ 
humidity_t* humidity_create(void);

/**
 * @method humidity_destroy
 * 销毁humidity对象。
 *
 * @annotation ["destructor"]
 * @param {humidity_t*} humidity humidity对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t humidity_destroy(humidity_t* humidity);

/**
 * @method humidity_set_value
 * 设置湿度。
 *
 * @param {humidity_t*} humidity humidity对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t humidity_set_value(humidity_t* humidity, double value);

/**
 * @method humidity_apply
 * 使用新设置的值生效。
 *
 * @annotation ["command"]
 * @param {humidity_t*} humidity humidity对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t humidity_apply(humidity_t* humidity);

/**
 * @method humidity_can_apply
 * 检查apply命令是否可以执行。
 *
 * @param {humidity_t*} humidity humidity对象。
 *
 * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
 */ 
bool_t humidity_can_apply(humidity_t* humidity);

END_C_DECLS

#endif /*HUMIDITY_H*/

