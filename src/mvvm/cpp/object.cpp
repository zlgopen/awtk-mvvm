/**
 * File:  object.cpp
 * Author: AWTK Develop Team
 * Brief:  object interface for cpp
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-05-28 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include "mvvm/cpp/object.hpp"

namespace vm {

Object::Object() : adapter(NULL) {
  emitter_init(&(this->emitter));
}

Object::~Object() {
  emitter_deinit(&(this->emitter));
}

ret_t Object::Exec(const char* name, const char* args) {
  return RET_NOT_IMPL;
}

bool_t Object::CanExec(const char* name, const char* args) const {
  return FALSE;
}

ret_t Object::GetProp(const char* name, value_t* v) const {
  return RET_NOT_FOUND;
}

ret_t Object::SetProp(const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

ret_t Object::Off(uint32_t id) {
  return emitter_off(&(this->emitter), id);
}

ret_t Object::OffByCtx(void* ctx) {
  return emitter_off_by_ctx(&(this->emitter), ctx);
}

ret_t Object::DispatchEvent(event_t* event) {
  return emitter_dispatch(&(this->emitter), event);
}

uint32_t Object::On(uint32_t event, event_func_t on_event, void* ctx) {
  return emitter_on(&(this->emitter), event, on_event, ctx);
}

ret_t Object::NotifyPropsChanged() {
  event_t e = event_init(EVT_PROPS_CHANGED, this);

  this->DispatchEvent(&e);

  return RET_OK;
}

ret_t Object::NotifyItemsChanged(Object* target /* = NULL*/) {
  event_t e = event_init(EVT_ITEMS_CHANGED, target != NULL ? target : this);

  this->DispatchEvent(&e);

  return RET_OK;
}
}  // namespace vm
