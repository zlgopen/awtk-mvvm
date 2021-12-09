﻿/**
 * File:   value_validator_delegate.c
 * Author: AWTK Develop Team
 * Brief:  value validator delegate
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
 * 2019-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/base/value_validator_delegate.h"

static const object_vtable_t s_value_validator_delegate_vtable = {
    .type = "value_validator_delegate",
    .desc = "value_validator_delegate",
    .size = sizeof(value_validator_delegate_t),
    .is_collection = FALSE};

static bool_t value_validator_delegate_is_valid(value_validator_t* c, const value_t* value,
                                                str_t* msg) {
  value_validator_delegate_t* value_convert_delegate = VALUE_VALIDATOR_DELEGATE(c);

  return value_convert_delegate->is_valid(value, msg);
}

static ret_t value_validator_delegate_fix(value_validator_t* c, value_t* value) {
  value_validator_delegate_t* value_convert_delegate = VALUE_VALIDATOR_DELEGATE(c);

  return value_convert_delegate->fix(value);
}

static ret_t value_validator_fix_default(value_t* value) {
  return RET_FAIL;
}

value_validator_t* value_validator_delegate_create(value_is_valid_t is_valid, value_fix_t fix) {
  tk_object_t* obj = NULL;
  value_validator_t* value_convert = NULL;
  return_value_if_fail(is_valid != NULL, NULL);
  value_validator_delegate_t* value_convert_delegate = NULL;

  obj = tk_object_create(&s_value_validator_delegate_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_VALIDATOR(obj);
  value_convert->fix = value_validator_delegate_fix;
  value_convert->is_valid = value_validator_delegate_is_valid;

  value_convert_delegate = VALUE_VALIDATOR_DELEGATE(obj);
  value_convert_delegate->fix = fix;
  value_convert_delegate->is_valid = is_valid;

  if (fix == NULL) {
    value_convert_delegate->fix = value_validator_fix_default;
  }

  return value_convert;
}
