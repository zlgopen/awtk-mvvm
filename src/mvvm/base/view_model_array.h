/**
 * File:   view_model_array.h
 * Author: AWTK Develop Team
 * Brief:  view_model_array
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ARRAY_H
#define TK_VIEW_MODEL_ARRAY_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _view_model_array_t;
typedef struct _view_model_array_t view_model_array_t;

/**
 * @class view_model_array_t
 * @parent view_model_t
 *
 * array view model。
 *
 */
struct _view_model_array_t {
  view_model_t view_model;

  int32_t cursor;

  /*private*/
  str_t temp_prop;
  str_t temp_expr;
};

/**
 * @method view_model_array_create
 * 创建view_model对象。
 *
 * @param {model_t*} model model对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_array_create(model_t* model);

#define VIEW_MODEL_ARRAY(view_model) ((view_model_array_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_ARRAY_H*/
