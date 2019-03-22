/**
 * File:   mvvm_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  mvvm jerryscript global functions.
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_JERRYSCRIPT_H
#define TK_MVVM_JERRYSCRIPT_H

#include "mvvm/base/binding_context.h"
#include "mvvm/jerryscript/view_model_jerryscript.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"

BEGIN_C_DECLS

/**
 * @method mvvm_jerryscript_init
 * 初始化MVVM jerryscript。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_jerryscript_init(void);

/**
 * @method jerryscript_run
 * 执行js代码。
 * @param {const char*} name 文件名。
 * @param {const char*} code 代码。
 * @param {uint32_t} code_size 代码长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jerryscript_run(const char* name, const char* code, uint32_t code_size);

/**
 * @method jerryscript_eval
 * 执行js代码，并返回jerry对象。
 * @param {const char*} name 文件名。
 * @param {const char*} code 代码。
 * @param {uint32_t} code_size 代码长度。
 *
 * @return {jerry_value_t} 返回jerry_value对象。
 */
jerry_value_t jerryscript_eval(const char* name, const char* code, uint32_t code_size);

/**
 * @method mvvm_jerryscript_deinit
 * ~初始化MVVM jerryscript。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_jerryscript_deinit(void);

END_C_DECLS

#endif /*TK_MVVM_JERRYSCRIPT_H*/
