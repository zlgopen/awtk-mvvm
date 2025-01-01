
/**
 * File:  book.h
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
 * 2020-01-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BOOK_H
#define TK_BOOK_H

#include <string>
#include <vector>
#include "tkc/types_def.h"

using std::string;
using std::vector;

/**
 * @class Book
 * @annotation ["model", "cpp"]
 * 图书信息。
 *
 */
class Book {
 public:
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
  /**
   * @method Sale
   * 卖掉一本。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Sale();

  /**
   * @method CanSale
   * 检查sale命令是否可以执行。
   *
   * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
   */
  bool_t CanSale(void) const;

 public:
  /**
   * @property {string} name
   * @annotation ["readable", "writable"]
   * 书名。
   */
  string name;

  /**
   * @property {uint32_t} stock
   * @annotation ["readable", "writable"]
   * 库存。
   */
  uint32_t stock;
};

#endif /*TK_BOOK_H*/
