/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils
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
 * 2019-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_UTILS_H
#define TK_MVVM_UTILS_H

#include "tkc/str.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

#define COMMAND_ARGS_STRING_PREFIX "string?"
#define COMMAND_ARGS_FSCRIPT_PREFIX "fscript?"

/**
 * @method tk_destruct_array_prop_name
 * 从完整属性名中取出属性名和索引。
 * @annotation ["global"]
 * @param {const char*} name 属性名。
 * @param {uint32_t*} index  返回索引。
 *
 * @return {const char*} 返回属性名。
 */
const char* tk_destruct_array_prop_name(const char* name, uint32_t* index);

/**
 * @method tk_command_arguments_to_object
 * 将字符串类型的命令参数转换为object_t。
 * @annotation ["global"]
 * @param {const char*} args 命令参数。
 * @param {value_t*} obj 返回转换后的结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_command_arguments_to_object(const char* args, tk_object_t* obj);

/**
 * @method tk_command_arguments_from_object
 * 将object_t类型的命令参数转换为字符串。
 * @annotation ["global"]
 * @param {const value_t*} args 命令参数。
 * @param {uint32_t*} temp  返回转换后的结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_command_arguments_from_object(tk_object_t* args, str_t* temp);

/**
 * @method tk_command_arguments_fscript
 * 读取命令参数上各参数的内容并执行。
 * @annotation ["global"]
 * @param {const value_t*} args 命令参数。
 * @param {uint32_t*} temp  返回转换后的结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_command_arguments_fscript(tk_object_t* args, tk_object_t* ctx);

/**
 * @method str_random
 * 生产一个随机字符串。
 *
 * @param {str_t*} str str对象。
 * @param {const char*} format格式。
 * @param {uint32_t} max 最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_random(str_t* str, const char* format, uint32_t max);

/**
 * @method tk_is_valid_prop_name
 * 判断是否是合法的属性名。
 * @annotation ["global"]
 * @param {const char*} name 属性名。
 *
 * @return {bool_t} 返回TRUE表示是合法的属性名，否则表示不是。
 */
bool_t tk_is_valid_prop_name(const char* name);

/**
 * @method model_init_sub_object_with_args
 * 初始化子对象。
 * @annotation ["global"]
 * @param {tk_object_t*} model 模型对象。
 * @param {const char*} prefix 前缀。
 * @param {tk_object_t*} args 参数。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_init_sub_object_with_args(tk_object_t* model, const char* prefix, tk_object_t* args);

#define destruct_array_prop_name tk_destruct_array_prop_name

END_C_DECLS

#endif /*TK_MVVM_UTILS_H*/
