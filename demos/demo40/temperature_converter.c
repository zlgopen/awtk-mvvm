/**
 * File:   temperature_converter.c
 * Author: AWTK Develop Team
 * Brief:  temperature converter
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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "temperature_converter.h"
#include "mvvm/base/value_converter_delegate.h"

static ret_t to_temp_f(const value_t* from, value_t* to) {
  value_set_double(to, value_int(from) * 1.8 + 32);

  return RET_OK;
}

static ret_t to_temp_c(const value_t* from, value_t* to) {
  value_set_double(to, (value_int(from) - 32) / 1.8);

  return RET_OK;
}

static void* create_temp_f_converter(void) {
  return value_converter_delegate_create(to_temp_c, to_temp_f);
}

ret_t temperature_converter_init(void) {
  value_converter_register("fahrenheit", create_temp_f_converter);

  return RET_OK;
}
