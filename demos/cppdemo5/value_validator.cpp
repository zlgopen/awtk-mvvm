/**
 * File:   value_validator.c
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "value_validator.hpp"

void* TemperatureValidator::Create(void) {
  return vm::To(new TemperatureValidator());
}

bool_t TemperatureValidator::IsValid(const value_t* value, str_t* msg) {
  int32_t temp = value_int(value);

  if (temp <= 20) {
    str_set(msg, "too low");
    return FALSE;
  } else if (temp >= 60) {
    str_set(msg, "too high");
    return FALSE;
  } else {
    str_set(msg, "normal");
    return TRUE;
  }
}

ret_t TemperatureValidator::Fix(value_t* value) {
  return RET_OK;
}
