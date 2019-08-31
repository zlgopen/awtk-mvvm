
/**
 * File:  books.hpp
 * Author: AWTK Develop Team
 * Brief:  books view model
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
 * 2019-09-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "books.hpp"

Book::Book() {
  this->stock = 0;
}

Book::~Book() {
}

ret_t Book::Sale() {
  this->stock--;

  return RET_OK;
}

/*==============================*/

Books::Books(navigator_request_t* request) : ViewModelArray(request) {
}

Books::~Books() {
}
  
ret_t Books::AddBook() {
  Book book;
  char buff[64];
  tk_snprintf(buff, sizeof(buff), "book %d", (random() % 10000));

  book.stock = random() % 100;
  book.name = buff;

  this->books.push_back(book);

  return RET_ITEMS_CHANGED;
}

uint32_t Books::GetSize(void) const {
  return this->books.size();
}

ret_t Books::Exec(int32_t index, const char* name) {
  if(tk_str_eq(name, "add")) {
    return this->AddBook();
  } else if(tk_str_eq(name, "remove")) {
    return this->Remove(index);
  } else if(tk_str_eq(name, "sale")) {
    return this->Sale(index);
  } else if(tk_str_eq(name, "clear")) {
    return this->Clear();
  }

  return RET_OK;
}

bool_t Books::CanExec(int32_t index, const char* name) const {
  if(tk_str_eq(name, "add")) {
    return TRUE;
  } else if(tk_str_eq(name, "remove")) {
    return this->books.size() > index;
  } else if(tk_str_eq(name, "clear")) {
    return this->books.size() > 0;
  }

  return TRUE;
}

ret_t Books::GetProp(int32_t index, const char* name, value_t* v) const {
  const Book& book = this->books[index];

  if(tk_str_eq(name, "name")) {
     value_set_str(v, book.name.c_str());

     return RET_OK;
  } else if(tk_str_eq(name, "stock")) {
    value_set_int(v, book.stock);
     
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t Books::SetProp(int32_t index, const char* name, const value_t* v) {
  Book& book = this->books[index];

  if(tk_str_eq(name, "stock")) {
    book.stock = value_int(v); 
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t Books::Sale(int32_t index) {
  Book& book = this->books[index];

  book.Sale();

  return RET_OBJECT_CHANGED;
}


ret_t Books::Remove(int32_t index) {
  this->books.erase(this->books.begin() + index);

  return RET_ITEMS_CHANGED;
}

ret_t Books::Clear() {
  this->books.clear();

  return RET_ITEMS_CHANGED;
}

view_model_t* books_view_model_create(navigator_request_t* req) {
  return vm::To(new Books(req));
}


