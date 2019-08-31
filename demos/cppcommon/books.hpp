
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

#ifndef TK_BOOKS_HPP
#define TK_BOOKS_HPP

#include <string>
#include <vector>
#include "mvvm/cpp/adapter.hpp"

using std::string;
using std::vector;

class Book {
public:
  Book();
  ~Book();
  ret_t Sale();
public:
  string name;
  uint32_t stock;
};

class Books : public vm::ViewModelArray {
  public:
    Books(navigator_request_t* request);
    virtual ~Books();
  
  public:
    virtual uint32_t GetSize(void) const;
    virtual ret_t Exec(int32_t index, const char* name);
    virtual bool_t CanExec(int32_t index, const char* name) const;
    virtual ret_t GetProp(int32_t index, const char* name, value_t* v) const;
    virtual ret_t SetProp(int32_t index, const char* name, const value_t* v);
    virtual ret_t Remove(int32_t index);
    virtual ret_t Clear();

  public:
    ret_t AddBook();
    ret_t Sale(int32_t index);

  private:
    vector<Book> books;
};

view_model_t* books_view_model_create(navigator_request_t* req);

#endif/*TK_BOOKS_HPP*/

