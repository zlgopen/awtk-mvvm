/**
 * File:  view_model.cpp
 * Author: AWTK Develop Team
 * Brief:  view_model interface for cpp
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

#include "mvvm/cpp/view_model.hpp"

namespace vm {

ViewModel::ViewModel(navigator_request_t* request) {
  this->request = request;
  emitter_init(&(this->emitter));
}

ViewModel::~ViewModel() {
  emitter_deinit(&(this->emitter));
}

ret_t ViewModel::Exec(const char* name, const char* args) {
  return RET_NOT_IMPL;
}

bool_t ViewModel::CanExec(const char* name, const char* args) const {
  return FALSE;
}

ret_t ViewModel::GetProp(const char* name, value_t* v) const {
  return RET_NOT_FOUND;
}

ret_t ViewModel::SetProp(const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

ret_t ViewModel::Off(uint32_t id) {
  return emitter_off(&(this->emitter), id);
}

ret_t ViewModel::OffByCtx(void* ctx) {
  return emitter_off_by_ctx(&(this->emitter), ctx);
}

ret_t ViewModel::DispatchEvent(event_t* event) {
  return emitter_dispatch(&(this->emitter), event);
}

uint32_t ViewModel::On(uint32_t event, event_func_t on_event, void* ctx) {
  return emitter_on(&(this->emitter), event, on_event, ctx);
}

}  // namespace vm
