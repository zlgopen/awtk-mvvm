
/**
 * File:  book.h
 * Author: AWTK Develop Team
 * Brief:  book
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_BOOK_H
#define TK_BOOK_H

#include <string>
#include "tkc/types_def.h"
#include "mvvm/cpp/adapter.hpp"

using std::string;

class Book : public vm::Object {
 public:
  string name;
  uint32_t stock;

 public:
  /**
   * @method Create
   * Book obj对象的Create函数。
   *
   * @annotation ["constructor"]
   * @return {object_t*} 返回Book的obj对象。
   */
  static object_t* Create();

  /**
   * @method Book
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @return {void} 返回无。
   */
  Book();
  ~Book();

 public:
  virtual ret_t Exec(const char* name, const char* args);
  virtual bool_t CanExec(const char* name, const char* args) const;
  virtual ret_t GetProp(const char* name, value_t* v) const;
  virtual ret_t SetProp(const char* name, const value_t* v);
};

/**
 * @method book_create
 * 创建book的obj对象。
 *
 * @annotation ["constructor"]
 * @return {object_t*} 返回book的obj对象。
 */
object_t* book_create(void);

#endif /*TK_BOOK_H*/
