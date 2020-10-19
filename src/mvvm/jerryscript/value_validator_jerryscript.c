/**
 * File:   value_validator_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented value_validator
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "jerryscript-port.h"
#include "jerryscript-ext/handler.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"

static const object_vtable_t s_value_validator_jerryscript_vtable = {
    .type = "value_validator_jerryscript",
    .desc = "value_validator_jerryscript",
    .size = sizeof(value_validator_jerryscript_t)};

static bool_t value_validator_jerryscript_is_valid(value_validator_t* c, const value_t* v,
                                                   str_t* msg) {
  object_t* obj = OBJECT(c);

  return jsvalue_validator_is_valid(obj->name, v, msg) == RET_OK;
}

static ret_t value_validator_jerryscript_fix(value_validator_t* c, value_t* v) {
  object_t* obj = OBJECT(c);

  return jsvalue_validator_fix(obj->name, v);
}

static value_validator_t* value_validator_jerryscript_create(const char* name) {
  object_t* obj = NULL;
  value_validator_t* validator = NULL;
  return_value_if_fail(jsvalue_validator_exist(name), NULL);

  obj = object_create(&s_value_validator_jerryscript_vtable);
  return_value_if_fail(obj != NULL, NULL);

  validator = VALUE_VALIDATOR(obj);
  validator->is_valid = value_validator_jerryscript_is_valid;
  validator->fix = value_validator_jerryscript_fix;
  object_set_name(obj, name);

  return validator;
}

ret_t value_validator_jerryscript_init(void) {
  return value_validator_register_generic(value_validator_jerryscript_create);
}

ret_t value_validator_jerryscript_deinit(void) {
  return RET_OK;
}
