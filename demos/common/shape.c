#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "shape.h"

/***************shape***************/;

shape_t* shape_create(void) {
  shape_t* shape = TKMEM_ZALLOC(shape_t);
  return_value_if_fail(shape != NULL, NULL);

  shape->type = 1;
  shape->x = 10;
  shape->y = 20;
  shape->w = 30;
  shape->h = 40;
  shape->opacity = 50;
  shape->text_align = 1;
  str_init(&(shape->name), 10);
  str_init(&(shape->overview), 10);

  return shape;
}

static ret_t shape_destroy(shape_t* shape) {
  return_value_if_fail(shape != NULL, RET_BAD_PARAMS);

  str_reset(&(shape->name));
  str_reset(&(shape->overview));

  TKMEM_FREE(shape);

  return RET_OK;
}

static int32_t shape_get_text_align(shape_t* shape) {
  return shape->text_align;
}

static ret_t shape_set_text_align(shape_t* shape, const value_t* v) {
  shape->text_align = value_int32(v);

  return RET_OK;
}

static char* shape_get_name(shape_t* shape) {
  return shape->name.str;
}

static ret_t shape_set_name(shape_t* shape, const value_t* v) {
  str_from_value(&(shape->name), v);

  return RET_OK;
}

static char* shape_get_overview(shape_t* shape) {
  char buff[256];
  tk_snprintf(buff, sizeof(buff), "%s: type=%d (%d %d %d %d) opacity=%d align=%d", shape->name.str,
              shape->type, shape->x, shape->y, shape->w, shape->h, shape->opacity,
              shape->text_align);

  str_set(&(shape->overview), buff);

  return shape->overview.str;
}

static ret_t shape_change_type(shape_t* shape, const char* args) {
  shape->type = tk_atoi(args);

  return RET_OBJECT_CHANGED;
}

static bool_t shape_can_exec_save(shape_t* shape, const char* args) {
  return shape->name.size > 0;
}

static ret_t shape_save(shape_t* shape, const char* args) {
  return RET_OBJECT_CHANGED;
}

/***************shape_view_model***************/

static ret_t shape_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  shape_t* shape = vm->shape;

  if (tk_str_eq("type", name)) {
    shape->type = value_int32(v);
  } else if (tk_str_eq("x", name)) {
    shape->x = value_int32(v);
  } else if (tk_str_eq("y", name)) {
    shape->y = value_int32(v);
  } else if (tk_str_eq("w", name)) {
    shape->w = value_int32(v);
  } else if (tk_str_eq("h", name)) {
    shape->h = value_int32(v);
  } else if (tk_str_eq("opacity", name)) {
    shape->opacity = value_int32(v);
  } else if (tk_str_eq("text_align", name)) {
    shape_set_text_align(shape, v);
  } else if (tk_str_eq("name", name)) {
    shape_set_name(shape, v);
  } else if (tk_str_eq("overview", name)) {
    str_from_value(&(shape->overview), v);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static ret_t shape_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  shape_t* shape = vm->shape;

  if (tk_str_eq("type", name)) {
    value_set_int32(v, shape->type);
  } else if (tk_str_eq("x", name)) {
    value_set_int32(v, shape->x);
  } else if (tk_str_eq("y", name)) {
    value_set_int32(v, shape->y);
  } else if (tk_str_eq("w", name)) {
    value_set_int32(v, shape->w);
  } else if (tk_str_eq("h", name)) {
    value_set_int32(v, shape->h);
  } else if (tk_str_eq("opacity", name)) {
    value_set_int32(v, shape->opacity);
  } else if (tk_str_eq("text_align", name)) {
    value_set_int32(v, shape_get_text_align(shape));
  } else if (tk_str_eq("name", name)) {
    value_set_str(v, shape_get_name(shape));
  } else if (tk_str_eq("overview", name)) {
    value_set_str(v, shape_get_overview(shape));
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static bool_t shape_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  shape_t* shape = vm->shape;

  if (tk_str_eq("change_type", name)) {
    return TRUE;
  } else if (tk_str_eq("save", name)) {
    return shape_can_exec_save(shape, args);
  } else {
    return FALSE;
  }
}

static ret_t shape_view_model_exec(object_t* obj, const char* name, const char* args) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  shape_t* shape = vm->shape;

  if (tk_str_eq("change_type", name)) {
    return shape_change_type(shape, args);
  } else if (tk_str_eq("save", name)) {
    return shape_save(shape, args);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

static ret_t shape_view_model_on_destroy(object_t* obj) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  shape_destroy(vm->shape);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_shape_view_model_vtable = {
    .type = "shape",
    .desc = "shape info",
    .size = sizeof(shape_view_model_t),
    .exec = shape_view_model_exec,
    .can_exec = shape_view_model_can_exec,
    .get_prop = shape_view_model_get_prop,
    .set_prop = shape_view_model_set_prop,
    .on_destroy = shape_view_model_on_destroy};

view_model_t* shape_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_shape_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  shape_view_model_t* shape_view_model = (shape_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  shape_view_model->shape = shape_create();
  ENSURE(shape_view_model->shape != NULL);

  return vm;
}
