/**
 * File:   view_model_jerryscript_object_wrapper.h
 * Author: AWTK Develop Team
 * Brief:  view model formed by wrapping jerry script object
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-02-24 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#ifndef TK_VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER_H
#define TK_VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER_H

#include "mvvm/base/view_model.h"
#include "mvvm/jerryscript/jsobj.h"

BEGIN_C_DECLS

/**
 * @class view_model_jerryscript_object_wrapper_t
 * @parent view_model_t
 *
 * wrap jerryscript object to a view_model
 *
 */
typedef struct _view_model_jerryscript_object_wrapper_t {
  view_model_t view_model;

  /*private*/
  tk_object_t* obj;
} view_model_jerryscript_object_wrapper_t;

/**
 * @method view_model_jerryscript_object_wrapper_create
 * 将jsobj包装成view_model。
 *
 * @param {jsvalue_t*} jsobj js对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_jerryscript_object_wrapper_create(jsvalue_t jsobj,
                                                           navigator_request_t* req);

bool_t view_model_is_jerryscript_object_wrapper(view_model_t* view_model);

#define VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model) \
  ((view_model_jerryscript_object_wrapper_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER_H*/
