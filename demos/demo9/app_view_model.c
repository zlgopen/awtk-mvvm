﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "app_view_model.h"

static ret_t app_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t app_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  app_view_model_t* vm = (app_view_model_t*)(obj);

  if (tk_str_ieq("running", name)) {
    value_set_bool(v, vm->running);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t app_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
  app_view_model_t* vm = (app_view_model_t*)(obj);
  if (tk_str_ieq("start", name)) {
    return !vm->running;
  } else if (tk_str_ieq("stop", name)) {
    return vm->running;
  }
  return FALSE;
}

static ret_t app_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
  app_view_model_t* vm = (app_view_model_t*)(obj);
  if (tk_str_ieq("start", name)) {
    vm->running = TRUE;
    log_debug("start\n");
    return RET_OBJECT_CHANGED;
  } else if (tk_str_ieq("stop", name)) {
    vm->running = FALSE;
    log_debug("stop\n");
    return RET_OBJECT_CHANGED;
  }
  return RET_NOT_FOUND;
}

static ret_t app_view_model_on_destroy(tk_object_t* obj) {
  app_view_model_t* vm = (app_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_app_view_model_vtable = {"app_view_model_t",
                                                             "app_view_model_t",
                                                             sizeof(app_view_model_t),
                                                             FALSE,
                                                             app_view_model_on_destroy,
                                                             NULL,
                                                             app_view_model_get_prop,
                                                             app_view_model_set_prop,
                                                             NULL,
                                                             NULL,
                                                             app_view_model_can_exec,
                                                             app_view_model_exec};

view_model_t* app_view_model_create(navigator_request_t* req) {
  tk_object_t* obj = tk_object_create(&s_app_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));

  return_value_if_fail(vm != NULL, NULL);

  return vm;
}

