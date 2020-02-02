/**
 * File:   calculator.h
 * Author: AWTK Develop Team
 * Brief:  calculator
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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class calculator_t
 *
 * @annotation ["model"]
 * 计算器。
 *
 */
typedef struct _calculator_t {
  /**
   * @property {const char*} expr
   * @annotation ["readable", "writable"]
   * 表达式。
   */
  str_t expr;

} calculator_t;

/**
 * @method calculator_create
 * 创建calculator对象。
 *
 * @annotation ["constructor"]
 * @return {calculator_t*} 返回calculator对象。
 */
calculator_t* calculator_create(void);

/**
 * @method calculator_destroy
 * 销毁calculator对象。
 *
 * @annotation ["destructor"]
 * @param {calculator_t*} calculator calculator对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t calculator_destroy(calculator_t* calculator);

/**
 * @method calculator_get_expr
 * 获取当前表达式。
 * @param {calculator_t*} calculator calculator对象。
 *
 * @return {const char*} 返回表达式。
 */
const char* calculator_get_expr(calculator_t* calculator);

/**
 * @method calculator_set_expr
 * 设置当前表达式。
 *
 * @param {calculator_t*} calculator calculator对象。
 * @param {const char*} expr 表达式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t calculator_set_expr(calculator_t* calculator, const char* expr);

/**
 * @method calculator_add_char
 * 追加一个字符。
 *
 * @annotation ["command"]
 * @param {calculator_t*} calculator calculator对象。
 * @param {const char*} args 参数。
 *
 * @return {ret_t} 返回RET_OBJECT_CHANGED表示模型有变化，View需要刷新；返回其它表示失败。
 */
ret_t calculator_add_char(calculator_t* calculator, const char* args);

/**
 * @method calculator_remove_char
 * 删除最后一个字符串。
 *
 * @annotation ["command"]
 * @param {calculator_t*} calculator calculator对象。
 *
 * @return {ret_t} 返回RET_OBJECT_CHANGED表示模型有变化，View需要刷新；返回其它表示失败。
 */
ret_t calculator_remove_char(calculator_t* calculator);

/**
 * @method calculator_can_remove_char
 * 检查remove_char命令是否可以执行。
 *
 * @param {calculator_t*} calculator calculator对象。
 *
 * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
 */
bool_t calculator_can_remove_char(calculator_t* calculator);

/**
 * @method calculator_eval
 * 计算表达式。
 *
 * @annotation ["command"]
 * @param {calculator_t*} calculator calculator对象。
 *
 * @return {ret_t} 返回RET_OBJECT_CHANGED表示模型有变化，View需要刷新；返回其它表示失败。
 */
ret_t calculator_eval(calculator_t* calculator);

/**
 * @method calculator_can_eval
 * 检查eval命令是否可以执行。
 *
 * @param {calculator_t*} calculator calculator对象。
 *
 * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
 */
bool_t calculator_can_eval(calculator_t* calculator);

END_C_DECLS

#endif /*CALCULATOR_H*/
