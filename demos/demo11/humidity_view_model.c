﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "humidity_view_model.h"

static ret_t humidity_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  humidity_t* ahumidity = ((humidity_view_model_t*)(obj))->ahumidity;

  if (tk_str_ieq("value", name)) {
    ahumidity->value = value_double(v);

    return RET_OK;
  }
  
  return RET_NOT_FOUND;
}


static ret_t humidity_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  humidity_t* ahumidity = ((humidity_view_model_t*)(obj))->ahumidity;

  if (tk_str_ieq("value", name)) {
    value_set_double(v, ahumidity->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}


static bool_t humidity_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
 
  humidity_view_model_t* vm = (humidity_view_model_t*)(obj);
  humidity_t* ahumidity = vm->ahumidity;
  if (tk_str_ieq("apply", name)) {
    return humidity_can_apply(ahumidity);
  }
  return FALSE;
}

static ret_t humidity_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
 
  humidity_view_model_t* vm = (humidity_view_model_t*)(obj);
  humidity_t* ahumidity = vm->ahumidity;
  if (tk_str_ieq("apply", name)) {
    return humidity_apply(ahumidity);
  }
  return RET_NOT_FOUND;
}

static ret_t humidity_view_model_on_destroy(tk_object_t* obj) {
  humidity_view_model_t* vm = (humidity_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  humidity_destroy(vm->ahumidity);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_humidity_view_model_vtable = {
  .type = "humidity_view_model_t",
  .desc = "humidity_view_model_t",
  .size = sizeof(humidity_view_model_t),
  .is_collection = FALSE,
  .on_destroy = humidity_view_model_on_destroy,
  .compare = NULL,
  .get_prop = humidity_view_model_get_prop,
  .set_prop = humidity_view_model_set_prop,
  .remove_prop = NULL,
  .foreach_prop = NULL,
  .clear_props = NULL,
  .find_prop = NULL,
  .find_props = NULL,
  .can_exec = humidity_view_model_can_exec,
  .exec = humidity_view_model_exec,
  .clone = NULL
};

view_model_t* humidity_view_model_create_with(humidity_t* ahumidity) {
  tk_object_t* obj = tk_object_create(&s_humidity_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  humidity_view_model_t* humidity_view_model = (humidity_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  humidity_view_model->ahumidity = ahumidity;
  

  return vm;
}

ret_t humidity_view_model_attach(view_model_t* vm, humidity_t* ahumidity) {
  humidity_view_model_t* humidity_view_model = (humidity_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  humidity_view_model->ahumidity = ahumidity;

  return RET_OK;
}

view_model_t* humidity_view_model_create(navigator_request_t* req) {
  humidity_t* ahumidity = humidity_create();
  return_value_if_fail(ahumidity != NULL, NULL);

  return humidity_view_model_create_with(ahumidity);
}
