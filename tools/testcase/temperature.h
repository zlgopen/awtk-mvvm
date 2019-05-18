
#ifndef TK_TEMPERATURE_H
#define TK_TEMPERATURE_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class temperature_t
 *
 * temperature controller
 *
 */
typedef struct _temperature_t {
  view_model_t view_model;
  double value;

  double old_value;
} temperature_t;

/**
 * @method temperature_create
 * 创建temperature对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* temperature_create(navigator_request_t* req);

#define TEMPERATURE(t) ((temperature_t*)(t))

END_C_DECLS

#endif /*TK_TEMPERATURE_H*/
