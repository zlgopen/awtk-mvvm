
#ifndef TK_CALCULATOR_H
#define TK_CALCULATOR_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class calculator_t
 *
 * a simple calculator
 *
 */
typedef struct _calculator_t{
  str_t expr;
} calculator_t;

/**
 * @class calculator_view_model_t
 *
 * view model of calculator
 *
 */
typedef struct _calculator_view_model_t {
  view_model_t view_model;

  /*model object*/
  calculator_t* calculator;
} calculator_view_model_t;

/**
 * @method calculator_view_model_create
 * 创建calculator view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* calculator_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_CALCULATOR_H*/
