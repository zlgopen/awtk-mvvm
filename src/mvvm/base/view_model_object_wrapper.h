/**
 * File:   view_model_object_wrapper.h
 * Author: AWTK Develop Team
 * Brief:  view_model_object_wrapper
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-04-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_OBJECT_WRAPPPER_H
#define TK_VIEW_MODEL_OBJECT_WRAPPPER_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _view_model_object_wrapper_t;
typedef struct _view_model_object_wrapper_t view_model_object_wrapper_t;

/**
 * @class view_model_object_wrapper_t
 * @parent view_model_t
 *
 * 把object包装成view model。
 *
 */
struct _view_model_object_wrapper_t {
  view_model_t view_model;

  /*private*/
  object_t* obj;

  /**
   * 属性路径的前缀。
   * obj可能是树型结构，如conf-obj，有时把其某个子节点包装成ViewModel，可以带来很大方便性和灵活性。
   * 属性路径的前缀用来指定子节点的位置。
   **/
  char* prop_prefix;
};

/**
 * @method view_model_object_wrapper_create
 * 创建view_model对象。
 *
 * @param {object_t*} obj 待包装的对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_object_wrapper_create(object_t* obj);

/**
 * @method view_model_object_wrapper_create_ex
 * 创建view_model对象。
 *
 * @param {object_t*} obj 待包装的对象。
 * @param {const char*} prop_prefix 属性路径的前缀(可以为NULL)。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_object_wrapper_create_ex(object_t* obj, const char* prop_prefix);

#define VIEW_MODEL_OBJECT_WRAPPPER(view_model) ((view_model_object_wrapper_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_OBJECT_WRAPPPER_H*/
