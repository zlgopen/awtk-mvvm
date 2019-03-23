#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/view_model_delegate.h"

typedef struct _test_obj_t {
  int8_t i8;
  int16_t i16;
  int32_t i32;
  int64_t i64;
  uint8_t u8;
  uint16_t u16;
  uint32_t u32;
  uint64_t u64;
  float_t f;
  float f32;
  double_t f64;
  char* str;
  bool_t b;
  char data[32];

  int32_t save_count;
} test_obj_t;

static test_obj_t* test_obj_create(void) {
  return TKMEM_ZALLOC(test_obj_t);
}

#define MAX_SAVE_COUNT 100

static ret_t test_obj_save(test_obj_t* obj, const char* args) {
  int delta = args != NULL ? tk_atoi(args) : 1;
  if (obj->save_count < MAX_SAVE_COUNT) {
    obj->save_count += delta;

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static bool_t test_obj_can_save(test_obj_t* obj, const char* args) {
  int delta = args != NULL ? tk_atoi(args) : 1;
  return (obj->save_count + delta) <= MAX_SAVE_COUNT;
}

static ret_t test_obj_get_data(test_obj_t* obj, value_t* v) {
  value_set_str(v, obj->data);

  return RET_OK;
}

static ret_t test_obj_get_save_count(test_obj_t* obj, value_t* v) {
  value_set_int(v, obj->save_count);

  return RET_OK;
}

static ret_t test_obj_set_data(test_obj_t* obj, value_t* v) {
  tk_strncpy(obj->data, value_str(v), sizeof(obj->data) - 1);

  return RET_OK;
}

static ret_t test_obj_destroy(test_obj_t* obj) {
  TKMEM_FREE(obj->str);
  TKMEM_FREE(obj);

  return RET_OK;
}

static view_model_t* test_obj_create_view_model(void) {
  test_obj_t* obj = test_obj_create();
  view_model_t* view_model = view_model_delegate_create(obj, (tk_destroy_t)test_obj_destroy);

  VIEW_MODEL_SIMPLE_PROP(view_model, "i8", VALUE_TYPE_INT8, &(obj->i8));
  VIEW_MODEL_SIMPLE_PROP(view_model, "i16", VALUE_TYPE_INT16, &(obj->i16));
  VIEW_MODEL_SIMPLE_PROP(view_model, "i32", VALUE_TYPE_INT32, &(obj->i32));
  VIEW_MODEL_SIMPLE_PROP(view_model, "i64", VALUE_TYPE_INT64, &(obj->i64));

  VIEW_MODEL_SIMPLE_PROP(view_model, "u8", VALUE_TYPE_UINT8, &(obj->u8));
  VIEW_MODEL_SIMPLE_PROP(view_model, "u16", VALUE_TYPE_UINT16, &(obj->u16));
  VIEW_MODEL_SIMPLE_PROP(view_model, "u32", VALUE_TYPE_UINT32, &(obj->u32));
  VIEW_MODEL_SIMPLE_PROP(view_model, "u64", VALUE_TYPE_UINT64, &(obj->u64));

  VIEW_MODEL_SIMPLE_PROP(view_model, "f", VALUE_TYPE_FLOAT, &(obj->f));
  VIEW_MODEL_SIMPLE_PROP(view_model, "f32", VALUE_TYPE_FLOAT32, &(obj->f32));
  VIEW_MODEL_SIMPLE_PROP(view_model, "f64", VALUE_TYPE_DOUBLE, &(obj->f64));
  VIEW_MODEL_SIMPLE_PROP(view_model, "b", VALUE_TYPE_BOOL, &(obj->b));

  VIEW_MODEL_PROP(view_model, "data", test_obj_get_data, test_obj_set_data);
  VIEW_MODEL_PROP(view_model, "save_count", test_obj_get_save_count, NULL);
  VIEW_MODEL_COMMAND(view_model, "save", test_obj_save, test_obj_can_save);

  /*test repeat*/
  VIEW_MODEL_PROP(view_model, "data", test_obj_get_data, test_obj_set_data);
  VIEW_MODEL_COMMAND(view_model, "save", test_obj_save, test_obj_can_save);

  return view_model;
}
