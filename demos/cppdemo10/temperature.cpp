
/**
 * File:  temperature.cpp
 * Author: AWTK Develop Team
 * Brief:  temperature
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "temperature.h"

Temperature::Temperature(void) {
  this->value = 20;
  this->timer = 10;
  this->old_value = 0;
}

ret_t Temperature::Apply(void) {
  this->old_value = this->value;

  return RET_OBJECT_CHANGED;
}

bool_t Temperature::CanApply(void) const {
  return this->old_value != this->value;
}
