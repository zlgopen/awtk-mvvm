
#ifndef TK_SHAPE_H
#define TK_SHAPE_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class shape_t
 *
 * shape info
 *
 */
typedef struct _shape_t{
  int32_t type;
  int32_t x;
  int32_t y;
  int32_t w;
  int32_t h;
  int32_t opacity;
  int32_t text_align;
  char* name;

  
  char* overview;
} shape_t;

/**
 * @class shape_view_model_t
 *
 * view model of shape
 *
 */
typedef struct _shape_view_model_t {
  view_model_t view_model;

  /*model object*/
  shape_t* shape;
} shape_view_model_t;

/**
 * @method shape_view_model_create
 * 创建shape view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* shape_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_SHAPE_H*/
