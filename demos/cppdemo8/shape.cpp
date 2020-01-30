/**
 * File:  shape.cpp
 * Author: AWTK Develop Team
 * Brief:  shape
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/utils.h"
#include "shape.h"

Shape::Shape(void) {
  this->type = 1;
  this->x = 10;
  this->y = 20;
  this->w = 30;
  this->h = 40;
  this->opacity = 50;
  this->text_align = 1;
}

Shape::~Shape() {
}

ret_t Shape::ChangeType(int32_t type) {
  this->type = type;

  return RET_OBJECT_CHANGED;
}

ret_t Shape::Save() {
  log_debug("save\n");
  return RET_OK;
}

bool_t Shape::CanSave() const {
  return this->name.size() > 0;
}

const char* Shape::GetOverview() {
  char buff[256];
  tk_snprintf(buff, sizeof(buff), 
       "%s: type=%d (%d %d %d %d) opacity=%d align=%d", this->name.c_str(),
      this->type, this->x, this->y, this->w, this->h, this->opacity, 
      this->text_align);
  this->overview = buff;

  return this->overview.c_str();
}

