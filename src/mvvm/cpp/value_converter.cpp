/**
 * File:  value_converter.cpp
 * Author: AWTK Develop Team
 * Brief:  value_converter interface for cpp
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
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/cpp/value_converter.hpp"

namespace vm {

ValueConverter::ValueConverter() {
}

ValueConverter::~ValueConverter() {
}

ret_t ValueConverter::ToModel(const value_t* from, value_t* to) {
  log_debug("%s\n", __FUNCTION__);

  return RET_NOT_IMPL;
}

ret_t ValueConverter::ToView(const value_t* from, value_t* to) {
  log_debug("%s\n", __FUNCTION__);

  return RET_NOT_IMPL;
}
}  // namespace vm
