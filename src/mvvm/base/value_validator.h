/**
 * File:   value_validator.h
 * Author: AWTK Develop Team
 * Brief:  value validator
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VALUE_VALIDATOR_H
#define TK_VALUE_VALIDATOR_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

struct _value_validator_t;
typedef struct _value_validator_t value_validator_t;

typedef value_validator_t* (*value_validator_create_t)(const char* name);
typedef bool_t (*value_validator_is_valid_t)(value_validator_t* validator, const value_t* value,
                                             str_t* msg);
typedef ret_t (*value_validator_fix_t)(value_validator_t* validator, value_t* value);

/**
 * @class value_validator_t
 * @parent tk_object_t
 *
 * 值校验器。
 *
 * 用户在界面上输入的类型可能是无效的，value_validator负责将检查用户输入的有效性。
 *
 */
struct _value_validator_t {
  tk_object_t object;

  tk_object_t* context;

  /*private*/
  value_validator_is_valid_t is_valid;
  value_validator_fix_t fix;
};

/**
 * @method value_validator_is_valid
 * 检查值是否有效。
 *
 * @param {value_validator_t*} validator validator对象。
 * @param {value_t*} value 待验证的值。
 * @param {str_t*} str 返回错误信息。
 *
 * @return {bool_t} 返回TRUE表示有效，否则表示无效。
 */
bool_t value_validator_is_valid(value_validator_t* validator, const value_t* value, str_t* msg);

/**
 * @method value_validator_fix
 * 将无效值修改为正确的值。
 *
 * @param {value_validator_t*} validator validator对象。
 * @param {value_t*} value 修正后的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_fix(value_validator_t* validator, value_t* value);

/**
 * @method value_validator_set_context
 * 设置上下文对象。
 *
 * 有时需要根据一个上下文，才能决定数据是否有效。
 *
 * @param {value_validator_t*} validator validator对象。
 * @param {tk_object_t*} context 上下文对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_set_context(value_validator_t* validator, tk_object_t* context);

/**
 * @method value_validator_create
 * 创建指定名称的值校验器。
 *
 * @annotation ["static"]
 * @param {const char*} name 名称。
 *
 * @return {value_validator_t*} 返回validator对象。
 */
value_validator_t* value_validator_create(const char* name);

/**
 * @method value_validator_register
 *
 * 注册值校验器的创建函数。
 * @annotation ["static"]
 * @param {const char*} name 名称。
 * @param {tk_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_register(const char* name, tk_create_t create);

/**
 * @method value_validator_register_generic
 *
 * 注册值转换器的通用创建函数(主要用于脚本语言)。
 * @annotation ["static"]
 * @param {value_validator_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_register_generic(value_validator_create_t create);

/**
 * @method value_validator_init
 *
 * 初始化值校验器的全局对象。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_init(void);

/**
 * @method value_validator_deinit
 *
 * 释放值校验器的全局对象。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_deinit(void);

#define VALUE_VALIDATOR(validator) ((value_validator_t*)(validator))

END_C_DECLS

#endif /*TK_VALUE_VALIDATOR_H*/
