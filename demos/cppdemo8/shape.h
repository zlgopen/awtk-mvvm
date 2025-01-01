/**
 * File:  shape.h
 * Author: AWTK Develop Team
 * Brief:  shape
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

#ifndef TK_SHAPE_HPP
#define TK_SHAPE_HPP

#include <string>
#include "tkc/types_def.h"

using std::string;

/**
 * @class Shape
 *
 * @annotation ["model", "cpp"]
 * 温度控制器。
 *
 */
class Shape {
 public:
  /**
   * @method Shape
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @return {void} 返回无。
   */
  Shape(void);
  ~Shape(void);

 public:
  /**
   * @property {int32_t} type
   * @annotation ["readable", "writable"]
   * 类型。
   */
  int32_t type;

  /**
   * @property {int32_t} x
   * @annotation ["readable", "writable"]
   * x。
   */
  int32_t x;
  /**
   * @property {int32_t} y
   * @annotation ["readable", "writable"]
   * y。
   */
  int32_t y;
  /**
   * @property {int32_t} w
   * @annotation ["readable", "writable"]
   * w。
   */
  int32_t w;
  /**
   * @property {int32_t} h
   * @annotation ["readable", "writable"]
   * h。
   */
  int32_t h;
  /**
   * @property {int32_t} opacity
   * @annotation ["readable", "writable"]
   * opacity。
   */
  int32_t opacity;
  /**
   * @property {int32_t} align
   * @annotation ["readable", "writable"]
   * align。
   */
  int32_t align;
  /**
   * @property {string} name
   * @annotation ["readable", "writable"]
   * name。
   */
  string name;
  /**
   * @property {string} overview
   * @annotation ["readable", "writable"]
   * overview。
   */
  string overview;

 public:
  /**
   * @method ChangeType
   * 修改类型。
   *
   * @annotation ["command:changeType"]
   * @param {int32_t} type 类型。
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t ChangeType(int32_t type);
  /**
   * @method Save
   * 保存。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Save(void);
  /**
   * @method CanSave
   * 检查Save命令是否可以执行。
   *
   * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
   */
  bool_t CanSave(void) const;

  /**
   * @method GetOverview
   * 获取overview。
   *
   * @return {const char*} 返回overview。
   */
  const char* GetOverview();
};

#endif /*TK_SHAPE_HPP*/
