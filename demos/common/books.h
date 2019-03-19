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
#include "mvvm/base/model.h"

BEGIN_C_DECLS

typedef struct _book_info_t {
  char name[TK_NAME_LEN + 1];
  uint32_t stock;
} book_info_t;

/**
 * @class books_t
 *
 * 图书列表。
 *
 */
typedef struct _books_t {
  model_t model;

  darray_t books;
} books_t;

/**
 * @method books_create
 * 创建books对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {model_t} 返回model_t对象。
 */
model_t* books_create(navigator_request_t* req);

uint32_t books_count(model_t* model);
ret_t books_clear(model_t* model);
ret_t books_remove(model_t* model, uint32_t index);
ret_t books_add(model_t* model, const char* name, uint32_t stock);
book_info_t* books_get(model_t* model, uint32_t index);
ret_t books_set_stock(model_t* model, uint32_t index, uint32_t stock);
ret_t books_set_stock_delta(model_t* model, uint32_t index, int32_t delta);

#define BOOKS(t) ((books_t*)(t))

#define BOOK_PROP_NAME "name"
#define BOOK_PROP_STOCK "stock"
#define BOOK_PROP_STYLE "style"

#define BOOK_CMD_ADD "add"
#define BOOK_CMD_CLEAR "clear"
#define BOOK_CMD_SALE "sale"
#define BOOK_CMD_REMOVE "remove"

END_C_DECLS

#endif /*TK_BOOKS_H*/
