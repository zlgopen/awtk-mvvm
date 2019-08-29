
/**
 * File:  temperature.cpp
 * Author: AWTK Develop Team
 * Brief:  temperature 
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
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "temperature.hpp"

Temperature::~Temperature() {
}
  
ret_t Temperature::Exec(const char* name, const char* args) {
  if (tk_str_eq("apply", name)) {
    this->old_value = this->value;
  
    return RET_OBJECT_CHANGED;
  } else {
    return RET_NOT_FOUND;
  }
}

bool_t Temperature::CanExec(const char* name, const char* args) {
  if (tk_str_eq("apply", name)) {
    return this->value != this->old_value;
  } else {
    return FALSE;
  }
}

ret_t Temperature::GetProp(const char* name, value_t* v) {
  if (tk_str_eq("value", name)) {
    value_set_double(v, this->value);
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t Temperature::SetProp(const char* name, const value_t* v) {
  if (tk_str_eq("value", name)) {
    this->value = value_double(v);

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

#include "mvvm/cpp/cpp_adapter.hpp"

view_model_t* TemperatureCreate(navigator_request_t* req) {
  return view_model_cpp_create(new Temperature(req));
}

