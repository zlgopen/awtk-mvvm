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

#include "jsobj.h"
#include "mvvm/base/navigator_request.h"

BEGIN_C_DECLS

jerry_value_t jsobj_get_model(const char* name);

/**
 * @method jerry_value_from_navigator_request
 * annotation ["global"]
 * export for test only
 */
jerry_value_t jerry_value_from_navigator_request(navigator_request_t* req);

/**
 * @method jerry_value_to_navigator_request
 * annotation ["global"]
 * export for test only
 */
navigator_request_t* jerry_value_to_navigator_request(jerry_value_t value);

ret_t jsobj_exec(jerry_value_t obj, const char* name, const char* args);
ret_t jsobj_exec_ex(jerry_value_t obj, const char* name, jerry_value_t jsargs);
bool_t jsobj_can_exec(jerry_value_t obj, const char* name, const char* args);

bool_t jsvalue_converter_exist(const char* name);
ret_t jsvalue_converter_to_view(const char* name, const value_t* from, value_t* to, str_t* temp);
ret_t jsvalue_converter_to_model(const char* name, const value_t* from, value_t* to, str_t* temp);

bool_t jsvalue_validator_exist(const char* name);
ret_t jsvalue_validator_is_valid(const char* name, const value_t* v, str_t* msg);
ret_t jsvalue_validator_fix(const char* name, value_t* v);

#define JSOBJ_VALUE_CONVERTERS "ValueConverters"
#define JSOBJ_VALUE_CONVERTER_TO_VIEW "toView"
#define JSOBJ_VALUE_CONVERTER_TO_VIEW_MODEL "toModel"
#define JSOBJ_VALUE_VALIDATORS "ValueValidators"
#define JSOBJ_VALUE_VALIDATOR_FIX "fix"
#define JSOBJ_VALUE_VALIDATOR_IS_VALID "isValid"
#define JSOBJ_VALUE_VALIDATOR_RESULT "result"
#define JSOBJ_VALUE_VALIDATOR_MESSAGE "message"

END_C_DECLS

#endif /*TK_JSOBJ_4_MVVM_H*/
