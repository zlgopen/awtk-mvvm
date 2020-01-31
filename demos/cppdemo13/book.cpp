
/**
 * File:  book.cpp
 * Author: AWTK Develop Team
 * Brief:  book
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
 * 2020-01-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "book.h"

Book::Book() {
}

Book::~Book() {
}

ret_t Book::Sale() {
  this->stock--;

  return RET_OBJECT_CHANGED;
}

bool_t Book::CanSale(void) const {
  return this->stock > 0;
}
