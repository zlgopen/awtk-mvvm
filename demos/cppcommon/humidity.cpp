
/**
 * File:  humidity.cpp
 * Author: AWTK Develop Team
 * Brief:  humidity view model
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "humidity.hpp"

#define PROP_HUMIDITY "humidity"

Humidity::Humidity(navigator_request_t* request) : ViewModel(request) {
  this->value = 20;
  this->old_value = 0;
}

Humidity::~Humidity() {
}

ret_t Humidity::Exec(const char* name, const char* args) {
  if (tk_str_eq("apply", name)) {
    this->old_value = this->value;
    return RET_OBJECT_CHANGED;
  } else {
    return RET_NOT_FOUND;
  }
}

bool_t Humidity::CanExec(const char* name, const char* args) const {
  if (tk_str_eq("apply", name)) {
    return this->value != this->old_value;
  } else {
    return FALSE;
  }
}

ret_t Humidity::GetProp(const char* name, value_t* v) const {
  if (tk_str_eq(PROP_HUMIDITY, name)) {
    value_set_double(v, this->value);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t Humidity::SetProp(const char* name, const value_t* v) {
  if (tk_str_eq(PROP_HUMIDITY, name)) {
    this->value = value_double(v);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

view_model_t* humidity_view_model_create(navigator_request_t* req) {
  return vm::To(new Humidity(req));
}
