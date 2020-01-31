
/**
 * File:  book_store.cpp
 * Author: AWTK Develop Team
 * Brief:  book_store
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

#include "tkc/utils.h"
#include "book_store.h"

BookStore::BookStore(void) {
}

BookStore::~BookStore() {
}

Book* BookStore::Get(uint32_t index) {
  return &(this->books[index]);
}

uint32_t BookStore::GetItems(void) const {
  return this->books.size();
}

ret_t BookStore::Remove(uint32_t index) {
  this->books.erase(this->books.begin() + index);

  return RET_ITEMS_CHANGED;
}

bool_t BookStore::CanRemove(uint32_t index) {
  return index < this->books.size();
}

ret_t BookStore::Clear() {
  this->books.clear();

  return RET_ITEMS_CHANGED;
}

ret_t BookStore::Add() {
  Book book;
  char buff[64];
  tk_snprintf(buff, sizeof(buff), "book %d", (random() % 10000));

  book.stock = random() % 10 + 1;
  book.name = buff;

  this->books.push_back(book);

  return RET_ITEMS_CHANGED;
}
