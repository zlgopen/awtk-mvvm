/**
 * File:   temperature_validator.c
 * Author: AWTK Develop Team
 * Brief:  temperature validator
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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "temperature_validator.h"
#include "mvvm/base/value_validator_delegate.h"

static bool_t is_valid_water_temp(const value_t* value, str_t* msg) {
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

static ret_t fix_water_temp(value_t* value) {
  /*
    int32_t temp = value_int(value);

    if (temp <= 20) {
      value_set_int(value, 20);
    } else if (temp >= 60) {
      value_set_int(value, 60);
    }
  */
  return RET_OK;
}

static void* create_water_temp_validator(void) {
  return value_validator_delegate_create(is_valid_water_temp, fix_water_temp);
}

ret_t temperature_validator_init(void) {
  value_validator_register("water_temp", create_water_temp_validator);

  return RET_OK;
}
