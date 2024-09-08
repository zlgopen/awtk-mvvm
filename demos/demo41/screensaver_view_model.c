﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "screensaver_view_model.h"

static ret_t screensaver_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  screensaver_t* ascreensaver = ((screensaver_view_model_t*)(obj))->ascreensaver;

  if (tk_str_ieq("time", name)) {
    ascreensaver->time = value_int32(v);

    return RET_OK;
  }
  
  return RET_NOT_FOUND;
}


static ret_t screensaver_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  screensaver_t* ascreensaver = ((screensaver_view_model_t*)(obj))->ascreensaver;

  if (tk_str_ieq("time", name)) {
    value_set_int32(v, ascreensaver->time);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}


static bool_t screensaver_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
 
  screensaver_view_model_t* vm = (screensaver_view_model_t*)(obj);
  screensaver_t* ascreensaver = vm->ascreensaver;
  if (tk_str_ieq("apply", name)) {
    return screensaver_can_apply(ascreensaver);
  }
  return FALSE;
}

static ret_t screensaver_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
 
  screensaver_view_model_t* vm = (screensaver_view_model_t*)(obj);
  screensaver_t* ascreensaver = vm->ascreensaver;
  if (tk_str_ieq("apply", name)) {
    return screensaver_apply(ascreensaver);
  }
  return RET_NOT_FOUND;
}

static ret_t screensaver_view_model_on_destroy(tk_object_t* obj) {
  screensaver_view_model_t* vm = (screensaver_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  TKMEM_FREE(vm->ascreensaver);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_screensaver_view_model_vtable = {
  .type = "screensaver_view_model_t",
  .desc = "screensaver_view_model_t",
  .size = sizeof(screensaver_view_model_t),
  .is_collection = FALSE,
  .on_destroy = screensaver_view_model_on_destroy,
  .compare = NULL,
  .get_prop = screensaver_view_model_get_prop,
  .set_prop = screensaver_view_model_set_prop,
  .remove_prop = NULL,
  .foreach_prop = NULL,
  .clear_props = NULL,
  .find_prop = NULL,
  .find_props = NULL,
  .can_exec = screensaver_view_model_can_exec,
  .exec = screensaver_view_model_exec,
  .clone = NULL
};

view_model_t* screensaver_view_model_create_with(screensaver_t* ascreensaver) {
  tk_object_t* obj = tk_object_create(&s_screensaver_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  screensaver_view_model_t* screensaver_view_model = (screensaver_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  screensaver_view_model->ascreensaver = ascreensaver;
  

  return vm;
}

ret_t screensaver_view_model_attach(view_model_t* vm, screensaver_t* ascreensaver) {
  screensaver_view_model_t* screensaver_view_model = (screensaver_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  screensaver_view_model->ascreensaver = ascreensaver;

  return RET_OK;
}

view_model_t* screensaver_view_model_create(navigator_request_t* req) {
  screensaver_t* ascreensaver = TKMEM_ZALLOC(screensaver_t);
  return_value_if_fail(ascreensaver != NULL, NULL);

  return screensaver_view_model_create_with(ascreensaver);
}
