/**
 * File:   book_view_model.h
 * Author: AWTK Develop Team
 * Brief:  book_view_model
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
 * 2021-05-28 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include "book.h"
#include "mvvm/base/view_model.h"

#ifndef TK_BOOK_VIEW_MODEL_H
#define TK_BOOK_VIEW_MODEL_H

BEGIN_C_DECLS
/**
 * @class book_view_model_t
 *
 * view model of book
 *
 */

/**
 * @method book_view_model_create
 * 创建book view model对象。
 *
 * @annotation ["constructor"]
 * @param {tk_object_t*} book book的obj对象。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* book_view_model_create(tk_object_t* book);

END_C_DECLS

#endif /*TK_BOOK_VIEW_MODEL_H*/
