/**
 * File:   books.h
 * Author: AWTK Develop Team
 * Brief:  books
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
 * 2019-03-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BOOKS_H
#define TK_BOOKS_H

#include "tkc/darray.h"
#include "mvvm/base/view_model_array.h"

BEGIN_C_DECLS

/**
 * @class book_info_t
 *
 * 图书。
 */
typedef struct _book_info_t {
  /**
   * @property {char*} name
   * @annotation ["readable"]
   * 书名。
   */
  char name[TK_NAME_LEN + 1];
  /**
   * @property {uint32_t} stock
   * @annotation ["readable"]
   * 库存(大于0时才可以出售)。
   */
  uint32_t stock;
} book_info_t;

/**
 * @class books_t
 *
 * 图书列表。
 *
 */
typedef struct _books_t {
  view_model_array_t view_model_array;

  /*private*/
  darray_t books;
} books_t;

/**
 * @method books_create
 * 创建books对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* books_create(navigator_request_t* req);

/**
 * @method books_clear
 * 清除全部图书。
 *
 * @param {view_model_t*} view_model books对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t books_clear(view_model_t* view_model);

/**
 * @method books_size
 * 返回图书数目。
 *
 * @param {view_model_t*} view_model books对象。
 *
 * @return {uint32_t} 返回图书数目。
 */
uint32_t books_size(view_model_t* view_model);

/**
 * @method books_remove
 * 移出指定索引的图书。
 *
 * @param {view_model_t*} view_model books对象。
 * @param {uint32_t} index 图书的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t books_remove(view_model_t* view_model, uint32_t index);

/**
 * @method books_get
 * 获取指定索引的图书。
 *
 * @param {view_model_t*} view_model books对象。
 * @param {uint32_t} index 图书的索引。
 *
 * @return {book_info_t*} 返回指定索引的图书
 */
book_info_t* books_get(view_model_t* view_model, uint32_t index);

/**
 * @method books_add
 * 增加图书。
 *
 * @param {view_model_t*} view_model books对象。
 * @param {const char*} name 图书名称。
 * @param {uint32_t} stock 库存。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t books_add(view_model_t* view_model, const char* name, uint32_t stock);

/**
 * @method books_sale
 * 出售图书。
 *
 * @param {view_model_t*} view_model books对象。
 * @param {uint32_t} index 图书的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t books_sale(view_model_t* view_model, uint32_t index);

#define BOOKS(t) ((books_t*)(t))

#define BOOK_PROP_NAME "name"
#define BOOK_PROP_STOCK "stock"
#define BOOK_PROP_STYLE "style"

#define BOOK_CMD_ADD "add"
#define BOOK_CMD_SALE "sale"
#define BOOK_CMD_CLEAR "clear"
#define BOOK_CMD_REMOVE "remove"

END_C_DECLS

#endif /*TK_BOOKS_H*/
