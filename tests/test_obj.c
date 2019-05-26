#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "test_obj.h"


/***************test_obj***************/;

test_obj_t* test_obj_create(void) {
  test_obj_t* test_obj = TKMEM_ZALLOC(test_obj_t);
  return_value_if_fail(test_obj != NULL, NULL);

  str_init(&(test_obj->data), 10);


  return test_obj;
} 

static ret_t test_obj_destroy(test_obj_t* test_obj) {
  return_value_if_fail(test_obj != NULL, RET_BAD_PARAMS);

  str_reset(&(test_obj->data));


  TKMEM_FREE(test_obj);

  return RET_OK;
}


static char* test_obj_get_data(test_obj_t* test_obj) {
  return test_obj->data.str;
}


static ret_t test_obj_set_data(test_obj_t* test_obj, const value_t* v) {
  str_from_value(&(test_obj->data), v);

  return RET_OK;
}

static bool_t test_obj_can_exec_save(test_obj_t* test_obj, const char* args) {
  return TRUE;
}

static ret_t test_obj_save(test_obj_t* test_obj, const char* args) {
  test_obj->save_count++;
  return RET_OBJECT_CHANGED;
}


static bool_t test_obj_can_exec_foo(test_obj_t* test_obj, const char* args) {
  return TRUE;
}

static ret_t test_obj_foo(test_obj_t* test_obj, const char* args) {
  
  return RET_OBJECT_CHANGED;
}


static ret_t test_obj_bar(test_obj_t* test_obj, const char* args) {
  
  return RET_OBJECT_CHANGED;
}

/***************test_obj_view_model***************/

static ret_t test_obj_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  test_obj_view_model_t* vm = (test_obj_view_model_t*)(obj);
  test_obj_t* test_obj = vm->test_obj;

  if (tk_str_eq("i8", name)) {
    test_obj->i8 =  value_int8(v);
  } else if (tk_str_eq("i16", name)) {
    test_obj->i16 =  value_int16(v);
  } else if (tk_str_eq("i32", name)) {
    test_obj->i32 =  value_int32(v);
  } else if (tk_str_eq("i64", name)) {
    test_obj->i64 =  value_int64(v);
  } else if (tk_str_eq("u8", name)) {
    test_obj->u8 =  value_uint8(v);
  } else if (tk_str_eq("u16", name)) {
    test_obj->u16 =  value_uint16(v);
  } else if (tk_str_eq("u32", name)) {
    test_obj->u32 =  value_uint32(v);
  } else if (tk_str_eq("u64", name)) {
    test_obj->u64 =  value_uint64(v);
  } else if (tk_str_eq("b", name)) {
    test_obj->b =  value_bool(v);
  } else if (tk_str_eq("f32", name)) {
    test_obj->f32 =  value_float(v);
  } else if (tk_str_eq("f64", name)) {
    test_obj->f64 =  value_double(v);
  } else if (tk_str_eq("save_count", name)) {
    test_obj->save_count =  value_int32(v);
  } else if (tk_str_eq("data", name)) {
    test_obj_set_data(test_obj, v);
  } else if (tk_str_eq("f", name)) {
    test_obj->f =  value_float(v);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static ret_t test_obj_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  test_obj_view_model_t* vm = (test_obj_view_model_t*)(obj);
  test_obj_t* test_obj = vm->test_obj;

  if (tk_str_eq("i8", name)) {
    value_set_int8(v, test_obj->i8);
  } else if (tk_str_eq("i16", name)) {
    value_set_int16(v, test_obj->i16);
  } else if (tk_str_eq("i32", name)) {
    value_set_int32(v, test_obj->i32);
  } else if (tk_str_eq("i64", name)) {
    value_set_int64(v, test_obj->i64);
  } else if (tk_str_eq("u8", name)) {
    value_set_uint8(v, test_obj->u8);
  } else if (tk_str_eq("u16", name)) {
    value_set_uint16(v, test_obj->u16);
  } else if (tk_str_eq("u32", name)) {
    value_set_uint32(v, test_obj->u32);
  } else if (tk_str_eq("u64", name)) {
    value_set_uint64(v, test_obj->u64);
  } else if (tk_str_eq("b", name)) {
    value_set_bool(v, test_obj->b);
  } else if (tk_str_eq("f32", name)) {
    value_set_float(v, test_obj->f32);
  } else if (tk_str_eq("f64", name)) {
    value_set_double(v, test_obj->f64);
  } else if (tk_str_eq("save_count", name)) {
    value_set_int32(v, test_obj->save_count);
  } else if (tk_str_eq("data", name)) {
    value_set_str(v, test_obj_get_data(test_obj));
  } else if (tk_str_eq("f", name)) {
    value_set_float(v, test_obj->f);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static bool_t test_obj_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  test_obj_view_model_t* vm = (test_obj_view_model_t*)(obj);
  test_obj_t* test_obj = vm->test_obj;

  if (tk_str_eq("save", name)) {
    return test_obj_can_exec_save(test_obj, args);
  } else if (tk_str_eq("foo", name)) {
    return test_obj_can_exec_foo(test_obj, args);
  } else if (tk_str_eq("bar", name)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static ret_t test_obj_view_model_exec(object_t* obj, const char* name, const char* args) {
  test_obj_view_model_t* vm = (test_obj_view_model_t*)(obj);
  test_obj_t* test_obj = vm->test_obj;

  if (tk_str_eq("save", name)) {
    return test_obj_save(test_obj, args);
  } else if (tk_str_eq("foo", name)) {
    return test_obj_foo(test_obj, args);
  } else if (tk_str_eq("bar", name)) {
    return test_obj_bar(test_obj, args);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

static ret_t test_obj_view_model_on_destroy(object_t* obj) {
  test_obj_view_model_t* vm = (test_obj_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  test_obj_destroy(vm->test_obj);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_test_obj_view_model_vtable = {
  .type = "test_obj",
  .desc = "test_obj",
  .size = sizeof(test_obj_view_model_t),
  .exec = test_obj_view_model_exec,
  .can_exec = test_obj_view_model_can_exec,
  .get_prop = test_obj_view_model_get_prop,
  .set_prop = test_obj_view_model_set_prop,
  .on_destroy = test_obj_view_model_on_destroy
};

view_model_t* test_obj_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_test_obj_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  test_obj_view_model_t* test_obj_view_model = (test_obj_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  test_obj_view_model->test_obj = test_obj_create();
  ENSURE(test_obj_view_model->test_obj != NULL);

  return vm;
}
