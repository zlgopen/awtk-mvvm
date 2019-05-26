
#ifndef TK_TEST_OBJ_H
#define TK_TEST_OBJ_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class test_obj_t
 *
 * 
 *
 */
typedef struct _test_obj_t{
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
  bool_t b;
  float f32;
  double f64;
  int32_t save_count;
  str_t data;
  float_t f;
} test_obj_t;

/**
 * @class test_obj_view_model_t
 *
 * view model of test_obj
 *
 */
typedef struct _test_obj_view_model_t {
  view_model_t view_model;

  /*model object*/
  test_obj_t* test_obj;
} test_obj_view_model_t;

/**
 * @method test_obj_view_model_create
 * 创建test_obj view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* test_obj_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_TEST_OBJ_H*/
