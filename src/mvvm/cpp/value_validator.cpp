/**
 * File:  value_validator.cpp
 * Author: AWTK Develop Team
 * Brief:  value_validator interface for cpp
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "mvvm/cpp/value_validator.hpp"

namespace vm {

ValueValidator::ValueValidator() {
}

ValueValidator::~ValueValidator() {
}

bool_t ValueValidator::IsValid(const value_t* value, str_t* msg) {
  log_debug("%s\n", __FUNCTION__);

  return FALSE;
}

ret_t ValueValidator::Fix(value_t* value) {
  log_debug("%s\n", __FUNCTION__);

  return RET_NOT_IMPL;
}
}  // namespace vm
