
/**
 * File:  calculator.h
 * Author: AWTK Develop Team
 * Brief: calculator
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CALCULATOR_HPP
#define TK_CALCULATOR_HPP

#include <string>
#include "tkc/types_def.h"

using std::string;

/**
 * @class Calculator
 *
 * @annotation ["model", "cpp"]
 * 计算器。
 *
 */
class Calculator {
 public:
  /**
   * @method Calculator
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @return {void} 返回无。
   */
  Calculator();
  ~Calculator();

 public:
  /**
   * @property {string} expr
   * @annotation ["readable", "writable"]
   * 表达式。
   */
  string expr;

 public:
  /**
   * @method AddChar
   * 追加一个字符。
   *
   * @annotation ["command:add_char"]
   * @param {char} c 字符。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t AddChar(char c);

  /**
   * @method RemoveChar
   * 删除最后一个字符。
   *
   * @annotation ["command:remove_char"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t RemoveChar(void);

  /**
   * @method CanRemoveChar
   * 检查RemoveChar命令是否可以执行。
   *
   * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
   */
  bool_t CanRemoveChar(void) const;

  /**
   * @method Eval
   * 计算当前表达式的值。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Eval(void);

  /**
   * @method CanEval
   * 检查eval命令是否可以执行。
   *
   * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
   */
  bool_t CanEval(void) const;
};

#endif /*TK_CALCULATOR_HPP*/
