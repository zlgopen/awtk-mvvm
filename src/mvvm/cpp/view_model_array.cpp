/**
 * File:  iview_model.cpp
 * Author: AWTK Develop Team
 * Brief:  view_model array interface for cpp
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/cpp/view_model_array.hpp"

namespace vm {

ViewModelArray::ViewModelArray(navigator_request_t* request) : ViewModel(request) {
}

ViewModelArray::~ViewModelArray() {
}

uint32_t ViewModelArray::GetSize() const {
  return 0;
}

ret_t ViewModelArray::Exec(int32_t index, const char* name) {
  return RET_NOT_IMPL;
}

bool_t ViewModelArray::CanExec(int32_t index, const char* name) const {
  return FALSE;
}

ret_t ViewModelArray::GetProp(int32_t index, const char* name, value_t* v) const {
  return RET_NOT_FOUND;
}

ret_t ViewModelArray::SetProp(int32_t index, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

ret_t ViewModelArray::Remove(int32_t index) {
  return RET_NOT_FOUND;
}

ret_t ViewModelArray::Clear() {
  return RET_NOT_IMPL;
}

}  // namespace vm
