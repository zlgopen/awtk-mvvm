#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "shape.h"


/***************shape***************/;

shape_t* shape_create(void) {
  shape_t* shape = TKMEM_ZALLOC(shape_t);
  return_value_if_fail(shape != NULL, NULL);

  shape->type = 0;
  shape->x = 1;
  shape->y = 2;
  shape->w = 3;
  shape->h = 4;
  shape->opacity = 5;
  shape->text_align = 1;

  return shape;
} 



static ret_t shape_destroy(shape_t* shape) {
  return_value_if_fail(shape != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(shape->name);
  TKMEM_FREE(shape->overview);


  TKMEM_FREE(shape);

  return RET_OK;
}


static char* shape_get_overview(shape_t* shape) {
  char buff[256];
  tk_snprintf(buff, sizeof(buff), "type=%d (%d %d %d %d) opacity=%d align=%d", 
      shape->type, shape->x, shape->y, shape->w, shape->h, shape->opacity, shape->text_align);

  shape->overview = tk_str_copy(shape->overview, buff);

  return shape->overview;
}

static ret_t shape_set_overview(shape_t* shape, char* value) {
  return RET_OK;
}

static bool_t shape_can_exec_save(shape_t* shape, const char* args) {
  return shape->name != NULL && shape->name[0] != '\0';
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
    shape->text_align = value_int32(v);
  } else if (tk_str_eq("name", name)) {
    shape->name = tk_str_copy(shape->name, value_str(v));
  } else if (tk_str_eq("overview", name)) {
    shape_set_overview(shape, value_str(v));
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
    int32_t type = shape->type;
    value_set_int32(v, type);
  } else if (tk_str_eq("x", name)) {
    int32_t x = shape->x;
    value_set_int32(v, x);
  } else if (tk_str_eq("y", name)) {
    int32_t y = shape->y;
    value_set_int32(v, y);
  } else if (tk_str_eq("w", name)) {
    int32_t w = shape->w;
    value_set_int32(v, w);
  } else if (tk_str_eq("h", name)) {
    int32_t h = shape->h;
    value_set_int32(v, h);
  } else if (tk_str_eq("opacity", name)) {
    int32_t opacity = shape->opacity;
    value_set_int32(v, opacity);
  } else if (tk_str_eq("text_align", name)) {
    int32_t text_align = shape->text_align;
    value_set_int32(v, text_align);
  } else if (tk_str_eq("name", name)) {
    char* name = shape->name;
    value_set_str(v, name);
  } else if (tk_str_eq("overview", name)) {
    char* overview = shape_get_overview(shape);
    value_set_str(v, overview);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static bool_t shape_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  shape_t* shape = vm->shape;

  if (tk_str_eq("save", name)) {
    return shape_can_exec_save(shape, args);
  } else {
    return FALSE;
  }
}

static ret_t shape_view_model_exec(object_t* obj, const char* name, const char* args) {
  shape_view_model_t* vm = (shape_view_model_t*)(obj);
  shape_t* shape = vm->shape;

  if (tk_str_eq("save", name)) {
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
  .on_destroy = shape_view_model_on_destroy
};

view_model_t* shape_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_shape_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  shape_view_model_t* shape_view_model = (shape_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  shape_view_model->shape = shape_create();
  ENSURE(shape_view_model->shape != NULL);

  return vm;
}
