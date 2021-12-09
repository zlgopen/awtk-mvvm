﻿/**
 * File:   view_model_dummy.h
 * Author: AWTK Develop Team
 * Brief:  view_model_dummy
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_DUMMY_H
#define TK_VIEW_MODEL_DUMMY_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_dummy_t;
typedef struct _model_dummy_t view_model_dummy_t;

/**
 * @class view_model_dummy_t
 * @parent view_model_t
 *
 * dummy view_model
 *
 */
struct _model_dummy_t {
  view_model_t view_model;

  tk_object_t* props;
};

/**
 * @method view_model_dummy_create
 * 创建dummy模型对象。
 *
 * 对于一些简单的窗口，只需要简单的导航(打开或关闭窗口)，可以不用实现自己的模型，而使用dummy模型。
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_dummy_create(navigator_request_t* req);

#define VIEW_MODEL_DUMMY(view_model) ((view_model_dummy_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_DUMMY_H*/
