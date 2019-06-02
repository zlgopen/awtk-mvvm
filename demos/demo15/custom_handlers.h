
#ifndef TK_CUSTOM_HANDLERS_H
#define TK_CUSTOM_HANDLERS_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class custom_handlers_t
 *
 * custom_handlers
 *
 */
typedef struct _custom_handlers_t {
} custom_handlers_t;

/**
 * @class custom_handlers_view_model_t
 *
 * view model of custom_handlers
 *
 */
typedef struct _custom_handlers_view_model_t {
  view_model_t view_model;

  /*model object*/
  custom_handlers_t* custom_handlers;
} custom_handlers_view_model_t;

/**
 * @method custom_handlers_view_model_create
 * 创建custom_handlers view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* custom_handlers_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_CUSTOM_HANDLERS_H*/
