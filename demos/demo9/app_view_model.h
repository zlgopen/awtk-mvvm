﻿
/*This file is generated by code generator*/

#include "mvvm/base/view_model.h"

#ifndef TK_APP_VIEW_MODEL_H
#define TK_APP_VIEW_MODEL_H

BEGIN_C_DECLS
/**
 * @class app_view_model_t
 *
 * view model of app
 *
 */
typedef struct _app_view_model_t {
  view_model_t view_model;

  bool_t running;
} app_view_model_t;

/**
 * @method app_view_model_create
 * 创建app view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* app_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_APP_VIEW_MODEL_H*/