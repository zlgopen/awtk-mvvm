
/**
 * File:   book.h
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
 * 2021-05-28 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_BOOK_H
#define TK_BOOK_H

#include "tkc/object.h"
#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class book_t
 * @parent object_t
 * @annotation ["model"]
 * 图书信息。
 *
 */
typedef struct _book_t {
  object_t obj;
  /**
   * @property {str_t} name
   * @annotation ["readable", "writable"]
   * 书名。
   */
  str_t name;
  /**
   * @property {uint32_t} stock
   * @annotation ["readable", "writable"]
   * 库存。
   */
  uint32_t stock;
} book_t;

/**
 * @method book_create
 * 创建book的obj对象。
 *
 * @annotation ["constructor"]
 * @return {object_t*} 返回book的obj对象。
 */
object_t* book_create(void);

/**
 * @method book_cast
 * 转换为book对象。
 * 
 * @annotation ["cast"]
 * @param {object_t*} obj object。
 * 
 * @return {book_t*} 返回book对象。
 */
book_t* book_cast(object_t* obj);

#define BOOK(obj) book_cast(obj)

END_C_DECLS

#endif /*TK_BOOK_H*/
