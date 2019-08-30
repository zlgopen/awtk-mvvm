/**
 * File:  iview_model.cpp
 * Author: AWTK Develop Team
 * Brief:  view_model array interface for cpp
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/cpp/view_model_array.hpp"

namespace vm {

ViewModelArray::ViewModelArray(navigator_request_t* request) {
  this->request = request;
  emitter_init(&(this->emitter));
}

ViewModelArray::~ViewModelArray() {
  emitter_deinit(&(this->emitter));
}

uint32_t ViewModelArray::GetSize() const {
  return 0;
}

ret_t ViewModelArray::Exec(int32_t index, const char* name, const char* args) {
  return RET_NOT_IMPL;
}

bool_t ViewModelArray::CanExec(int32_t index, const char* name, const char* args) const {
  return FALSE;
}

ret_t ViewModelArray::GetProp(int32_t index, const char* name, value_t* v) const {
  return RET_NOT_FOUND;
}

ret_t ViewModelArray::SetProp(int32_t index, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

ret_t ViewModelArray::Off(uint32_t id) {
  return emitter_off(&(this->emitter), id);
}

ret_t ViewModelArray::OffByCtx(void* ctx) {
  return emitter_off_by_ctx(&(this->emitter), ctx);
}

ret_t ViewModelArray::DispatchEvent(event_t* event) {
  return emitter_dispatch(&(this->emitter), event);
}

uint32_t ViewModelArray::On(uint32_t event, event_func_t on_event, void* ctx) {
  return emitter_on(&(this->emitter), event, on_event, ctx);
}

}  // namespace vm
