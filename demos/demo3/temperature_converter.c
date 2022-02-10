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
#include "tkc/utils.h"
#include "temperature_converter.h"
#include "mvvm/base/value_converter_delegate.h"
#include "mvvm/base/value_converter_with_args_delegate.h"

static ret_t to_temp_f(const value_t* from, value_t* to) {
  value_set_double(to, value_int(from) * 1.8 + 32);

  return RET_OK;
}

static ret_t to_temp_c(const value_t* from, value_t* to) {
  value_set_double(to, (value_int(from) - 32) / 1.8);

  return RET_OK;
}

static ret_t to_temp_f_sprintf(const value_t* from, value_t* to, value_t args) {
  char temp[33] = {0};
  char* format = value_str(&args);
  tk_snprintf(temp, sizeof(temp), format, value_int(from) * 1.8 + 32);
  value_dup_str(to, temp);
  return RET_OK;
}


static void* create_temp_f_converter(void) {
  return value_converter_delegate_create(to_temp_c, to_temp_f);
}

static void* create_temp_f_converter_with_args(void) {
  return value_converter_with_args_delegate_create(NULL, to_temp_f_sprintf);
}

ret_t temperature_converter_init(void) {
  value_converter_register("fahrenheit", create_temp_f_converter);
  value_converter_register("fahrenheit_with_args", create_temp_f_converter_with_args);

  return RET_OK;
}
