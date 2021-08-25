#include "mvvm/base/view_model.h"

#ifndef TK_DEVICES_VIEW_MODEL_H
#define TK_DEVICES_VIEW_MODEL_H

BEGIN_C_DECLS
/**
 * @class devices_view_model_t
 *
 * view model of devices
 *
 */
typedef struct _devices_view_model_t {
  view_model_t view_model;

  /*model object*/
  object_t* devices;
  bool_t unlocked;
  int32_t current_index;
} devices_view_model_t;

/**
 * @method devices_view_model_create
 * 创建book_store view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* devices_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_DEVICES_VIEW_MODEL_H*/
