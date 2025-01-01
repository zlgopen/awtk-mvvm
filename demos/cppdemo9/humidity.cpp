
/**
 * File:  humidity.cpp
 * Author: AWTK Develop Team
 * Brief:  humidity
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

#include "humidity.h"

Humidity::Humidity(void) {
  this->value = 20;
  this->old_value = 0;
}

ret_t Humidity::Apply(void) {
  this->old_value = this->value;

  return RET_OBJECT_CHANGED;
}

bool_t Humidity::CanApply(void) const {
  return this->old_value != this->value;
}
