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

#ifndef TK_JSOBJ_4_MVVM_H
#define TK_JSOBJ_4_MVVM_H

#include "mvvm/base/navigator_request.h"
#include "mvvm/jerryscript/jsobj.h"

BEGIN_C_DECLS

ret_t jerry_script_register_mvvm_factory(void);

jsvalue_t jsvalue_get_view_model(const char* name);
ret_t js_view_model_set_native_ptr(jsvalue_t obj, void* ptr);

/**
 * @method js_view_model_get_native_ptr
 * annotation ["global"]
 * export for test only
 */
void* js_view_model_get_native_ptr(jsvalue_t obj);
ret_t js_view_model_delete_native_ptr(jsvalue_t obj);

jsvalue_t jsvalue_get_value_converter(const char* name);
ret_t js_value_converter_to_view(jsvalue_t converter, const value_t* from, value_t* to,
                                 str_t* temp);
ret_t js_value_converter_to_model(jsvalue_t converter, const value_t* from, value_t* to,
                                  str_t* temp);

jsvalue_t jsvalue_get_value_validator(const char* name);
bool_t js_value_validator_is_valid(jsvalue_t validator, const value_t* v, str_t* msg);
ret_t js_value_validator_fix(jsvalue_t validator, value_t* v);

/**
 * @method jsvalue_to_navigator_request
 * annotation ["global"]
 * export for test only
 */
navigator_request_t* jsvalue_to_navigator_request(jsvalue_t obj);

/**
 * @method jsvalue_from_navigator_request
 * annotation ["global"]
 * export for test only
 */
jsvalue_t jsvalue_from_navigator_request(navigator_request_t* req);

END_C_DECLS

#endif /*TK_JSOBJ_4_MVVM_H*/
