/**
 * File:   jsobj.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript wrapper
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/str.h"
#include "jerryscript.h"

BEGIN_C_DECLS

bool_t jsobj_has_prop(jerry_value_t obj, const char* name);
bool_t jsobj_has_prop_func(jerry_value_t obj, const char* name);

jerry_value_t jerry_create_str(const char* str);
ret_t jsobj_get_prop(jerry_value_t obj, const char* name, value_t* v, str_t* temp);
jerry_value_t jsobj_get_prop_value(jerry_value_t obj, const char* name);
object_t* jsobj_get_prop_object(jerry_value_t obj, const char* name);
void* jsobj_get_prop_pointer(jerry_value_t obj, const char* name);

ret_t jsobj_set_prop_pointer(jerry_value_t obj, const char* name, void* p);
ret_t jsobj_set_prop_object(jerry_value_t obj, const char* name, object_t* ptr);
ret_t jsobj_set_prop_str(jerry_value_t obj, const char* name, const char* value);
ret_t jsobj_set_prop_value(jerry_value_t obj, const char* name, jerry_value_t prop_value);
ret_t jsobj_set_prop(jerry_value_t obj, const char* name, const value_t* v, str_t* temp);
ret_t jsobj_set_prop_func(jerry_value_t obj, const char* name, jerry_external_handler_t handler_p);
ret_t jsobj_set_prop_int(jerry_value_t obj, const char* name, int32_t value);

/**
 * @method jerry_value_to_value
 * annotation ["global"]
 * export for test only
 */
ret_t jerry_value_to_value(jerry_value_t value, value_t* v, str_t* temp);

/**
 * @method jerry_value_from_value
 * annotation ["global"]
 * export for test only
 */
jerry_value_t jerry_value_from_value(const value_t* v, str_t* temp);

/**
 * @method jerry_value_to_pointer
 * annotation ["global"]
 * export for test only
 */
void* jerry_value_to_pointer(jerry_value_t value);

/**
 * @method jerry_value_from_pointer
 * annotation ["global"]
 * export for test only
 */
jerry_value_t jerry_value_from_pointer(void* ptr);
/**
 * @method jerry_value_check
 * annotation ["global"]
 * export for test only
 */
ret_t jerry_value_check(jerry_value_t value);

/**
 * @method jerry_value_to_value
 * annotation ["global"]
 * export for test only
 */
ret_t jerry_value_to_obj(jerry_value_t value, object_t* obj);

/**
 * @method jerry_value_to_value
 * annotation ["global"]
 * export for test only
 */
ret_t jerry_value_from_obj(jerry_value_t value, object_t* obj);

#define JSOBJ_ON_RESULT "onResult"

END_C_DECLS

#endif /*TK_JSOBJ_H*/
