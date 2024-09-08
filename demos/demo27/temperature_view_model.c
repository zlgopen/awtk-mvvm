﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "temperature_view_model.h"

static ret_t temperature_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  temperature_t* atemperature = ((temperature_view_model_t*)(obj))->atemperature;

  if (tk_str_ieq("temp", name)) {
    atemperature->temp = value_double(v);

    return RET_OK;
  }
  
  return RET_NOT_FOUND;
}


static ret_t temperature_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  temperature_t* atemperature = ((temperature_view_model_t*)(obj))->atemperature;

  if (tk_str_ieq("temp", name)) {
    value_set_double(v, atemperature->temp);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}


static bool_t temperature_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {

  return FALSE;
}

static ret_t temperature_view_model_exec(tk_object_t* obj, const char* name, const char* args) {

  return RET_NOT_FOUND;
}

static ret_t temperature_view_model_on_destroy(tk_object_t* obj) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  TKMEM_FREE(vm->atemperature);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_temperature_view_model_vtable = {
  .type = "temperature_view_model_t",
  .desc = "temperature_view_model_t",
  .size = sizeof(temperature_view_model_t),
  .is_collection = FALSE,
  .on_destroy = temperature_view_model_on_destroy,
  .compare = NULL,
  .get_prop = temperature_view_model_get_prop,
  .set_prop = temperature_view_model_set_prop,
  .remove_prop = NULL,
  .foreach_prop = NULL,
  .clear_props = NULL,
  .find_prop = NULL,
  .find_props = NULL,
  .can_exec = temperature_view_model_can_exec,
  .exec = temperature_view_model_exec,
  .clone = NULL
};

view_model_t* temperature_view_model_create_with(temperature_t* atemperature) {
  tk_object_t* obj = tk_object_create(&s_temperature_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  temperature_view_model_t* temperature_view_model = (temperature_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  temperature_view_model->atemperature = atemperature;
  

  return vm;
}

ret_t temperature_view_model_attach(view_model_t* vm, temperature_t* atemperature) {
  temperature_view_model_t* temperature_view_model = (temperature_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  temperature_view_model->atemperature = atemperature;

  return RET_OK;
}

view_model_t* temperature_view_model_create(navigator_request_t* req) {
  temperature_t* atemperature = TKMEM_ZALLOC(temperature_t);
  return_value_if_fail(atemperature != NULL, NULL);

  return temperature_view_model_create_with(atemperature);
}
