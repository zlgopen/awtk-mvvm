/**
 * File:   value_validator_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented value_validator
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"

static const object_vtable_t s_value_validator_jerryscript_vtable = {
    .type = "value_validator_jerryscript",
    .desc = "value_validator_jerryscript",
    .size = sizeof(value_validator_jerryscript_t)};

static bool_t value_validator_jerryscript_is_valid(value_validator_t* c, const value_t* v,
                                                   str_t* msg) {
  bool_t ret = FALSE;
  jsvalue_t validator = jsvalue_get_value_validator(TK_OBJECT(c)->name);

  ret = js_value_validator_is_valid(validator, v, msg);
  jsvalue_unref(validator);

  return ret;
}

static ret_t value_validator_jerryscript_fix(value_validator_t* c, value_t* v) {
  ret_t ret = RET_OK;
  jsvalue_t validator = jsvalue_get_value_validator(TK_OBJECT(c)->name);

  ret = js_value_validator_fix(validator, v);
  jsvalue_unref(validator);

  return ret;
}

static value_validator_t* value_validator_jerryscript_create(const char* name) {
  tk_object_t* obj = NULL;
  jsvalue_t jsobj = jsvalue_get_value_validator(name);
  return_value_if_fail(jsvalue_check(jsobj) == RET_OK, NULL);

  if (jsvalue_is_object(jsobj)) {
    obj = tk_object_create(&s_value_validator_jerryscript_vtable);
    if (obj != NULL) {
      value_validator_t* validator = VALUE_VALIDATOR(obj);
      value_validator_jerryscript_t* jsvalidator = VALUE_VALIDATOR_JERRYSCRIPT(obj);

      validator->is_valid = value_validator_jerryscript_is_valid;
      validator->fix = value_validator_jerryscript_fix;
      tk_object_set_name(obj, name);
    }
  }
  jsvalue_unref(jsobj);

  return VALUE_VALIDATOR(obj);
}

ret_t value_validator_jerryscript_init(void) {
  return value_validator_register_generic(value_validator_jerryscript_create);
}

ret_t value_validator_jerryscript_deinit(void) {
  return RET_OK;
}
