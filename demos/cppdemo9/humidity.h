/**
 * File:  temperature.h
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
 * 2020-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HUMIDITY_H
#define TK_HUMIDITY_H

#include "tkc/types_def.h"

/**
 * @class Humidity
 *
 * @annotation ["model", "cpp"]
 * 湿度控制器。
 *
 */
class Humidity {
 public:
  /**
   * @method Humidity
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @return {void} 返回无。
   */
  Humidity();

 public:
  /**
   * @property {double} value
   * @annotation ["readable", "writable"]
   * 值。
   */
  double value;

 public:
  /**
   * @method Apply
   * 使用新设置的值生效。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Apply(void);

  /**
   * @method CanApply
   * 检查apply命令是否可以执行。
   *
   * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
   */
  bool_t CanApply(void) const;

 private:
  double old_value;
};

#endif /*TK_HUMIDITY_H*/
