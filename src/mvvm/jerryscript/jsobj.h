/**
 * File:   jsobj.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript wrapper
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_JSOBJ_H
#define TK_JSOBJ_H

#include "mvvm/jerryscript/jerry_script_helper.h"
#include "tkc/str.h"
#include "tkc/value.h"

BEGIN_C_DECLS

typedef jerry_value_t jsvalue_t;
typedef jerry_external_handler_t jsfunc_t;
typedef jerry_object_type_t jsobj_type_t;

#define jsvalue_check(v) jerry_value_check(v)
#define jsvalue_ref(v) jerry_acquire_value(v)
#define jsvalue_unref(v) jerry_release_value(v)
#define jsvalue_get_global_object() jerry_get_global_object()
#define jsvalue_to_number(v) jerry_get_number_value(v)
#define jsvalue_from_number(v) jerry_create_number(v)
#define jsvalue_to_boolean(v) jerry_value_to_boolean(v)
#define jsvalue_from_boolean(v) jerry_create_boolean(v)
#define jsvalue_is_boolean(v) jerry_value_is_boolean(v)
#define jsvalue_is_undefined(v) jerry_value_is_undefined(v)
#define jsvalue_is_object(v) jerry_value_is_object(v)
#define jsvalue_is_function(v) jerry_value_is_function(v)
#define JS_UNDEFINED jerry_create_undefined()
#define JS_EMPTY_OBJ jerry_create_object()
#define JS_EMPTY_ARRAY jerry_create_array(0)

#define jsfunc_register_global(name, wrap_func) \
  jerryx_handler_register_global((const jerry_char_t*)name, wrap_func)
#define jsfunc_call(func, this_value, argv, argc) jerry_call_function(func, this_value, argv, argc)
#define JSFUNC_DECL(func_name)                                                        \
  jsvalue_t func_name(const jerry_call_info_t* call_info_p, const jsvalue_t args_p[], \
                      const jerry_length_t args_count)

#define JSOBJ_PROP_NATIVE_OBJ "$o"
#define JSOBJ_PROP_ON_RESULT "onResult"

/**
 * @method js_gc
 * gc。
 * @annotation ["global"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t js_gc();

/**
 * @method jsvalue_get_global
 * 获取全局js对象。
 * @annotation ["global"]
 * @param {const char*} name 对象名称。
 *
 * @return {jsvalue_t} 返回对象句柄。
 */
jsvalue_t jsvalue_get_global(const char* name);

/**
 * @method jsvalue_compare
 * 比较js对象。
 * @annotation ["global"]
 * @param {jsvalue_t} value value对象。
 * @param {jsvalue_t} other other对象。
 *
 * @return {int32_t} 相等返回0
 */
int32_t jsvalue_compare(jsvalue_t value, jsvalue_t other);

/**
 * @method jsvalue_from_utf8
 * 将字符串转化为js对象句柄。
 * @annotation ["global"]
 * @param {const char*} str 字符串。
 *
 * @return {jsvalue_t} 返回对象句柄。
 */
jsvalue_t jsvalue_from_utf8(const char* str);

/**
 * @method jsvalue_to_utf8
 * 将js对象句柄转化为字符串。
 * @annotation ["global"]
 * @param {jsvalue_t} value 对象句柄。
 * @param {str_t*} temp 保存字符串。
 *
 * @return {char*} 返回字符串指针。
 */
char* jsvalue_to_utf8(jsvalue_t value, str_t* temp);

/**
 * @method jsvalue_from_obj
 * 将object对象转换为js对象。
 * > 如果obj不是object_js_base_t的派生，则转换后的js对象为含有如下接口的空对象：
 * > setProp(name, val)：用于设置obj的属性值；
 * > getProp(name)：用于获取obj的属性值；
 * > canExec(name, args)：用于判断obj的命令是否可执行；
 * > exec(name, args)：用于执行obj的命令；
 * @annotation ["global"]
 * @param {tk_object_t*} obj obj对象。
 *
 * @return {jsvalue_t} 返回js对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsvalue_from_obj(tk_object_t* obj);

/**
 * @method jsvalue_from_obj_copy
 * 将object对象转换为js对象。
 * > 如果obj不是object_js_base_t的派生，则拷贝object对象的属性并转换为js对象。
 * @annotation ["global"]
 * @param {tk_object_t*} obj obj对象。
 *
 * @return {jsvalue_t} 返回js对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsvalue_from_obj_copy(tk_object_t* obj);

/**
 * @method jsvalue_to_obj
 * 将js对象转换为object对象。
 * @annotation ["global"]
 * @param {jsvalue_t} value js对象。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* jsvalue_to_obj(jsvalue_t value);

/**
 * @method jsvalue_from_value
 * 将value值或字符串转换为js对象。
 * @annotation ["global"]
 * @param {const value_t*} v 值。
 * @param {str_t*} temp 字符串。
 *
 * @return {jsvalue_t} 返回js对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsvalue_from_value(const value_t* v, str_t* temp);

/**
 * @method jsvalue_to_value
 * 将js对象转换为value值或字符串。
 * @annotation ["global"]
 * @param {jsvalue_t} value js对象。
 * @param {value_t*} v 返回的值。
 * @param {str_t*} temp 返回的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsvalue_to_value(jsvalue_t value, value_t* v, str_t* temp);

/**
 * @method jsobj_register_global
 * 将object_t对象注册为指定的全局js对象。
 * @annotation ["global"]
 * @param {const char*} name 全局js对象的名称。
 * @param {tk_object_t*} obj tk_object_t对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_register_global(const char* name, tk_object_t* obj);

/**
 * @method jsobj_unregister_global
 * 注销指定名称的全局js对象。
 * @annotation ["global"]
 * @param {const char*} name 全局js对象的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_unregister_global(const char* name);

/**
 * @method jsobj_set_native_ptr
 * 设置js对象的原生obj。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {tk_object_t*} ptr 原生obj。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_native_ptr(jsvalue_t obj, tk_object_t* ptr);

/**
 * @method jsobj_get_native_ptr
 * 获取js对象的原生obj。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 *
 * @return {tk_object_t*} 原生obj。
 */
tk_object_t* jsobj_get_native_ptr(jsvalue_t obj);

/**
 * @method jsobj_delete_native_ptr
 * 删除js对象的原生obj。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_delete_native_ptr(jsvalue_t obj);

/**
 * @method jsobj_get_prop_count
 * 计算js对象中的属性个数。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 *
 * @return {uint32_t} 返回属性个数。
 */
uint32_t jsobj_get_prop_count(jsvalue_t obj);

/**
 * @method jsobj_get_prop_value_by_index
 * 获取js对象中指定索引的属性对象。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {uint32_t} index 属性索引。
 *
 * @return {jsvalue_t} 返回属性的js对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsobj_get_prop_value_by_index(jsvalue_t obj, uint32_t index);

/**
 * @method jsobj_set_prop_value_by_index
 * 设置js对象中指定索引的属性对象。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {uint32_t} index 属性索引。
 * @param {jsvalue_t} prop_value 属性js对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop_value_by_index(jsvalue_t obj, uint32_t index, jsvalue_t prop_value);

/**
 * @method jsobj_get_prop_by_index
 * 获取js对象中指定索引的属性值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {uint32_t} index 属性索引。
 * @param {value_t*} v 返回属性的值。
 * @param {str_t*} temp 返回属性的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_get_prop_by_index(jsvalue_t obj, uint32_t index, value_t* v, str_t* temp);

/**
 * @method jsobj_set_prop_by_index
 * 设置js对象中指定索引的属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {uint32_t} index 属性索引。
 * @param {value_t*} v 属性的值。
 * @param {str_t*} temp 属性的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop_by_index(jsvalue_t obj, uint32_t index, const value_t* v, str_t* temp);

/**
 * @method jsobj_remove_prop_by_index
 * 删除js对象中指定索引的属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {uint32_t} index 属性索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_remove_prop_by_index(jsvalue_t obj, uint32_t index);

/**
 * @method jsobj_get_keys
 * 获取js对象中可枚举属性组成的数组。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 *
 * @return {jsvalue_t} 返回枚举数组对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsobj_get_keys(jsvalue_t obj);

/**
 * @method jsobj_get_prop_value
 * 获取js对象属性值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 *
 * @return {jsvalue_t} 返回值的js对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsobj_get_prop_value(jsvalue_t obj, const char* name);

/**
 * @method jsobj_set_prop_value
 * 设置js对象属性值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 * @param {jsvalue_t} prop_value 值的js对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop_value(jsvalue_t obj, const char* name, jsvalue_t prop_value);

/**
 * @method jsobj_has_prop
 * 判断js对象中是否存在属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 *
 * @return {bool_t} 返回true表示存在。
 */
bool_t jsobj_has_prop(jsvalue_t obj, const char* name);

/**
 * @method jsobj_has_prop_func
 * 判断js对象中是否存在函数属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 函数名称。
 *
 * @return {bool_t} 返回true表示存在。
 */
bool_t jsobj_has_prop_func(jsvalue_t obj, const char* name);

/**
 * @method jsobj_get_prop
 * 获取js对象属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 * @param {value_t*} v 返回属性的值。
 * @param {str_t*} temp 返回属性的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_get_prop(jsvalue_t obj, const char* name, value_t* v, str_t* temp);

/**
 * @method jsobj_set_prop
 * 设置js对象属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 * @param {value_t*} v 属性的值。
 * @param {str_t*} temp 属性的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop(jsvalue_t obj, const char* name, const value_t* v, str_t* temp);

/**
 * @method jsobj_set_prop_int
 * 设置js对象指定属性的整数类型的值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 * @param {int} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop_int(jsvalue_t obj, const char* name, int v);

/**
 * @method jsobj_set_prop_str
 * 设置js对象指定属性的字符串类型的值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称。
 * @param {const char*} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop_str(jsvalue_t obj, const char* name, const char* v);

/**
 * @method jsobj_set_prop_func
 * 设置js对象指定属性的函数（绑定函数）。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称（js函数名）。
 * @param {jsfunc_t} func 绑定函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_set_prop_func(jsvalue_t obj, const char* name, jsfunc_t func);

/**
 * @method jsobj_remove_prop
 * 删除js对象指定属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 属性名称（js函数名）。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_remove_prop(jsvalue_t obj, const char* name);

/**
 * @method jsobj_foreach_prop
 * 遍历js对象属性。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {tk_visit_t} on_prop 回调函数。
 * @param {void*} ctx 回调函数上下文。
 * @param {str_t*} temp_name 属性名称。
 * @param {str_t*} temp_value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_foreach_prop(jsvalue_t obj, tk_visit_t on_prop, void* ctx, str_t* temp_name,
                         str_t* temp_value);

/**
 * @method jsobj_exec_ex
 * 执行js对象中的指定命令。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 命令名称。
 * @param {jsvalue_t} jsargs js对象参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_exec_ex(jsvalue_t obj, const char* name, jsvalue_t jsargs);

/**
 * @method jsobj_exec
 * 执行js对象中的指定命令。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 命令名称。
 * @param {const value_t*} args value参数。
 * @param {str_t*} temp 字符串参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_exec(jsvalue_t obj, const char* name, const value_t* args, str_t* temp);

/**
 * @method jsobj_exec_ex_value
 * 执行js对象中的指定命令并获取返回值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 命令名称。
 * @param {jsvalue_t} jsargs_p js对象参数列表。
 * @param {uint32_t} jsargs_c js对象参数个数。
 *
 * @return {jsvalue_t} 返回值的js对象（使用完毕需要调用 jsvalue_unref 接口将引用计数减1）。
 */
jsvalue_t jsobj_exec_ex_value(jsvalue_t obj, const char* name, jsvalue_t* jsargs_p,
                              uint32_t jsargs_c);

/**
 * @method jsobj_exec_value
 * 执行js对象中的指定命令并获取返回值。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 命令名称。
 * @param {const value_t*} args_p value参数列表。
 * @param {uint32_t} args_c value参数个数。
 * @param {value_t*} v 返回的值。
 * @param {str_t*} temp 字符串参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jsobj_exec_value(jsvalue_t obj, const char* name, const value_t* args_p, uint32_t args_c,
                       value_t* v, str_t* temp);

/**
 * @method jsobj_can_exec
 * 检查js对象中是否可以执行指定命令。
 * @annotation ["global"]
 * @param {jsvalue_t} obj js对象。
 * @param {const char*} name 命令名称。
 * @param {const value_t*} args value参数。
 * @param {str_t*} temp 字符串参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
bool_t jsobj_can_exec(jsvalue_t obj, const char* name, const value_t* args, str_t* temp);

/**
 * @method jerry_value_check
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_acquire_value
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_release_value
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_get_global_object
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_get_number_value
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_create_number
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_value_is_boolean
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_value_to_boolean
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_create_boolean
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_value_is_undefined
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_value_is_object
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_value_is_function
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_create_undefined
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_create_array
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_create_object
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerryx_handler_register_global
 * @annotation ["global"]
 * export jerryscript function for external call
 */

/**
 * @method jerry_call_function
 * @annotation ["global"]
 * export jerryscript function for external call
 */

END_C_DECLS

#endif /*TK_JSOBJ_H*/
