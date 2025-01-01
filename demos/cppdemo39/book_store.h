
/**
 * File:  book_store.h
 * Author: AWTK Develop Team
 * Brief:  book_store
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

#ifndef TK_BOOK_STORE_HPP
#define TK_BOOK_STORE_HPP

#include <string>
#include <vector>
#include "book.h"

using std::string;
using std::vector;

/**
 * @class BookStore
 *
 * @annotation ["model", "cpp", "collection:Book"]
 * book store。
 *
 */
class BookStore {
 public:
  /**
   * @method BookStore
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @return {void} 返回无。
   */
  BookStore(void);
  ~BookStore();

  /**
   * @property {uint32_t} items
   * @annotation ["readable", "fake"]
   * 总数。
   */

 public:
  /**
   * @method Get
   * 获取指定序数的图书。
   *
   * @return {Book*} 返回指定序数的图书。
   */
  Book* Get(uint32_t index);

  /**
   * @method GetItems
   * 获取总数。
   *
   * @return {uint32_t} 返回总数。
   */
  uint32_t GetItems(void) const;

 public:
  /**
   * @method Remove
   * 删除指定的图书。
   *
   * @annotation ["command"]
   * @param {uint32_t} index 序数。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Remove(uint32_t index);

  /**
   * @method CanRemove
   * 检查Remove命令是否可以执行。
   * @param {uint32_t} index 序数。
   *
   * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
   */
  bool_t CanRemove(uint32_t index);

  /**
   * @method Clear
   * 清除全部图书。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Clear();

  /**
   * @method Add
   * 增加一本书。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Add();

 private:
  vector<Book> books;
};

#endif /*TK_BOOK_STORE_HPP*/
