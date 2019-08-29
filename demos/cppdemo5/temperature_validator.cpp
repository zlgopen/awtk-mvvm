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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "temperature_validator.hpp"
#include "mvvm/cpp/cpp_adapter.hpp"

class TemperatureValidator : public ValueValidator {
  public:
    virtual bool_t IsValid(const value_t* value, str_t* msg) {
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

    virtual ret_t Fix(value_t* value) {
      return RET_OK;
    }
};

static void* create_water_temp_validator(void) {
  return value_validator_cpp_create(new TemperatureValidator());
}

ret_t temperature_validator_init(void) {
  value_validator_register("water_temp", create_water_temp_validator);

  return RET_OK;
}
