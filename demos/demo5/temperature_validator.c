/**
 * File:   temperature_validator.c
 * Author: AWTK Develop Team
 * Brief:  temperature validator
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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "temperature_validator.h"
#include "mvvm/base/value_validator_delegate.h"

static bool_t is_valid_water_temp(const value_t* value, str_t* msg) {
  int32_t temp = value_int(value);

  if(temp <= 20) {
    str_set(msg, "two low");
    return FALSE;
  } else if(temp >= 60) {
    str_set(msg, "two high");
    return FALSE;
  } else {
    return TRUE;
  }
}

static void* create_water_temp_validator(void) {
  return value_validator_delegate_create(is_valid_water_temp);
}

ret_t temperature_validator_init(void) {
  value_validator_register("water_temp", create_water_temp_validator);

  return RET_OK;
}
