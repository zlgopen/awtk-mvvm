/**
 * File:   book_view_model.cpp
 * Author: AWTK Develop Team
 * Brief:  book_view_model
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

#include "book_view_model.h"
#include "mvvm/base/view_model_object_wrapper.h"

view_model_t* book_view_model_create(tk_object_t* book) {
  return_value_if_fail(book != NULL, NULL);

  return view_model_object_wrapper_create(book);
}
