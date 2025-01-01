/**
 * File:   view_model_compositor.h
 * Author: AWTK Develop Team
 * Brief:  view_model_compositor
 *
 * Copyright (c) 2020 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_COMPOSITOR_H
#define TK_VIEW_MODEL_COMPOSITOR_H

#include "tkc/darray.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_compositor_t;
typedef struct _model_compositor_t view_model_compositor_t;

/**
 * @class view_model_compositor_t
 * @parent view_model_t
 *
 * 将多个view model组合起来，当一个view model使用。
 *
 */
struct _model_compositor_t {
  view_model_t view_model;

  /*private*/
  darray_t view_models;
};

/**
 * @method view_model_compositor_create
 * 创建compositor对象。
 *
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_compositor_create(navigator_request_t* req);

/**
 * @method view_model_compositor_add
 * 向compositor对象中增加一个view model对象。
 *
 * @param {view_model_t*} view_model compositor对象。
 * @param {view_model_t*} vm 待加入的view model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_compositor_add(view_model_t* view_model, view_model_t* vm);

#define VIEW_MODEL_COMPOSITOR(view_model) ((view_model_compositor_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_COMPOSITOR_H*/
