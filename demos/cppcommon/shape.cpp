
/**
 * File:  shape.cpp
 * Author: AWTK Develop Team
 * Brief:  shape view model
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

#include "tkc/utils.h"
#include "shape.hpp"

Shape::Shape(navigator_request_t* request) : ViewModel(request) {
  this->type = 1;
  this->x = 10;
  this->y = 20;
  this->w = 30;
  this->h = 40;
  this->opacity = 50;
  this->text_align = 1;
  str_init(&(this->name), 10);
  str_init(&(this->overview), 10);
}

Shape::~Shape() {
  str_reset(&(this->name));
  str_reset(&(this->overview));
}

ret_t Shape::Exec(const char* name, const char* args) {
  if (tk_str_eq("change_type", name)) {
    this->type = tk_atoi(args);
    return RET_OBJECT_CHANGED;
  } else if (tk_str_eq("save", name)) {
    return RET_OBJECT_CHANGED;
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

bool_t Shape::CanExec(const char* name, const char* args) const {
  if (tk_str_eq("change_type", name)) {
    return TRUE;
  } else if (tk_str_eq("save", name)) {
    return this->name.size > 0;
  } else {
    return FALSE;
  }
}

ret_t Shape::GetProp(const char* name, value_t* v) const {
  if (tk_str_eq("type", name)) {
    value_set_int32(v, this->type);
  } else if (tk_str_eq("x", name)) {
    value_set_int32(v, this->x);
  } else if (tk_str_eq("y", name)) {
    value_set_int32(v, this->y);
  } else if (tk_str_eq("w", name)) {
    value_set_int32(v, this->w);
  } else if (tk_str_eq("h", name)) {
    value_set_int32(v, this->h);
  } else if (tk_str_eq("opacity", name)) {
    value_set_int32(v, this->opacity);
  } else if (tk_str_eq("text_align", name)) {
    value_set_int32(v, this->text_align);
  } else if (tk_str_eq("name", name)) {
    value_set_str(v, this->name.str);
  } else if (tk_str_eq("overview", name)) {
    char buff[256];
    tk_snprintf(buff, sizeof(buff), "%s: type=%d (%d %d %d %d) opacity=%d align=%d", this->name.str,
                this->type, this->x, this->y, this->w, this->h, this->opacity, this->text_align);

    str_set(&(this->overview), buff);
    value_set_str(v, this->overview.str);

  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

ret_t Shape::SetProp(const char* name, const value_t* v) {
  if (tk_str_eq("type", name)) {
    this->type = value_int32(v);
  } else if (tk_str_eq("x", name)) {
    this->x = value_int32(v);
  } else if (tk_str_eq("y", name)) {
    this->y = value_int32(v);
  } else if (tk_str_eq("w", name)) {
    this->w = value_int32(v);
  } else if (tk_str_eq("h", name)) {
    this->h = value_int32(v);
  } else if (tk_str_eq("opacity", name)) {
    this->opacity = value_int32(v);
  } else if (tk_str_eq("text_align", name)) {
    this->text_align = value_int(v);
  } else if (tk_str_eq("name", name)) {
    str_from_value(&(this->name), v);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

view_model_t* shape_view_model_create(navigator_request_t* req) {
  return vm::To(new Shape(req));
}
