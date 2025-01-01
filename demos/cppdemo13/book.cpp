
/**
 * File:  book.cpp
 * Author: AWTK Develop Team
 * Brief:  book
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
 * 2021-05-31 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include <string>
#include "book.h"
#include "mvvm/cpp/adapter.hpp"

tk_object_t* Book::Create(void) {
  return vm::To(new Book());
}

Book::Book() {
  char buff[64];
  tk_snprintf(buff, sizeof(buff), "book %d", (random() % 10000));

  this->stock = random() % 100;
  this->name = buff;
}

Book::~Book() {
}

ret_t Book::Exec(const char* name, const char* args) {
  if (tk_str_eq(name, "sale")) {
    this->stock--;
    return RET_OBJECT_CHANGED;
  }

  return RET_NOT_FOUND;
}

bool_t Book::CanExec(const char* name, const char* args) const {
  if (tk_str_eq(name, "sale")) {
    return this->stock > 0;
  }

  return FALSE;
}

ret_t Book::GetProp(const char* name, value_t* v) const {
  if (tk_str_eq(name, "name")) {
    value_set_str(v, this->name.c_str());
    return RET_OK;
  } else if (tk_str_eq(name, "stock")) {
    value_set_uint32(v, this->stock);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t Book::SetProp(const char* name, const value_t* v) {
  if (tk_str_eq(name, "name")) {
    this->name = value_str(v);
    return RET_OK;
  } else if (tk_str_eq(name, "stock")) {
    this->stock = value_uint32(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}
