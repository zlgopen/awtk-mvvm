/**
 * File:   value_converter.c
 * Author: AWTK Develop Team
 * Brief:  value converter
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-01 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include "value_converter.hpp"

void* TemperatureConverter::Create(void) {
  return vm::To(new TemperatureConverter());
}

ret_t TemperatureConverter::ToModel(const value_t* from, value_t* to) {
  value_set_double(to, (value_int(from) - 32) / 1.8);

  return RET_OK;
}

ret_t TemperatureConverter::ToView(const value_t* from, value_t* to) {
  value_set_double(to, value_int(from) * 1.8 + 32);

  return RET_OK;
}