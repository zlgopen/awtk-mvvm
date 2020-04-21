/**
 * File:   view_model_object_wrapper.h
 * Author: AWTK Develop Team
 * Brief:  view_model_object_wrapper
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-04-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_OBJECT_WRAPPPER_H
#define TK_VIEW_MODEL_OBJECT_WRAPPPER_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_object_wrapper_t;
typedef struct _model_object_wrapper_t view_model_object_wrapper_t;

/**
 * @class view_model_object_wrapper_t
 * @parent view_model_t
 *
 * 把object包装成view model。
 *
 */
struct _model_object_wrapper_t {
  view_model_t view_model;

  object_t* obj;
};

/**
 * @method view_model_object_wrapper_create
 * 创建view_model对象。
 *
 * @param {object_t*} obj 对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_object_wrapper_create(object_t* obj);

#define VIEW_MODEL_OBJECT_WRAPPPER(view_model) ((view_model_object_wrapper_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_OBJECT_WRAPPPER_H*/
