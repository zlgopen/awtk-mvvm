/**
 * File:   book_store_view_model.h
 * Author: AWTK Develop Team
 * Brief:  book_store_view_model
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

#include "mvvm/base/view_model.h"

#ifndef TK_BOOK_STORE_VIEW_MODEL_H
#define TK_BOOK_STORE_VIEW_MODEL_H

BEGIN_C_DECLS
/**
 * @class book_store_view_model_t
 *
 * view model of book_store
 *
 */
typedef struct _book_store_view_model_t {
  view_model_t view_model;

  /*model object*/
  object_t* abook_store;
} book_store_view_model_t;

/**
 * @method book_store_view_model_create
 * 创建book_store view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* book_store_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_BOOK_STORE_VIEW_MODEL_H*/
