﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "temperature_view_model.h"

static ret_t temperature_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  Temperature* aTemperature = ((temperature_view_model_t*)(obj))->aTemperature;

  if (tk_str_eq("value", name)) {
     aTemperature->value = value_double(v);

     return RET_OK;
  }
  
  return RET_NOT_FOUND;
}


static ret_t temperature_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  Temperature* aTemperature = ((temperature_view_model_t*)(obj))->aTemperature;

  if (tk_str_eq("value", name)) {
     value_set_double(v, aTemperature->value);
     return RET_OK;
  }

  return RET_NOT_FOUND;
}


static bool_t temperature_view_model_can_exec(object_t* obj, const char* name, const char* args) {
 
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  Temperature* aTemperature = vm->aTemperature;
  if (tk_str_eq("Apply", name)) {
    return aTemperature->CanApply();
  }
  return FALSE;
}

static ret_t temperature_view_model_exec(object_t* obj, const char* name, const char* args) {
 
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  Temperature* aTemperature = vm->aTemperature;
  if (tk_str_eq("Apply", name)) {
    return aTemperature->Apply();
  }
  return RET_NOT_FOUND;
}

static ret_t temperature_view_model_on_destroy(object_t* obj) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  delete (vm->aTemperature);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_temperature_view_model_vtable = {
  .type = "temperature_view_model_t",
  .desc = "temperature_view_model_t",
  .size = sizeof(temperature_view_model_t),
  .exec = temperature_view_model_exec,
  .can_exec = temperature_view_model_can_exec,
  .get_prop = temperature_view_model_get_prop,
  .set_prop = temperature_view_model_set_prop,
  .on_destroy = temperature_view_model_on_destroy
};

view_model_t* temperature_view_model_create_with(Temperature* aTemperature) {
  object_t* obj = object_create(&s_temperature_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  temperature_view_model_t* temperature_view_model = (temperature_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  temperature_view_model->aTemperature = aTemperature;
  

  return vm;
}

ret_t temperature_view_model_attach(view_model_t* vm, Temperature* aTemperature) {
  temperature_view_model_t* temperature_view_model = (temperature_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  temperature_view_model->aTemperature = aTemperature;

  return RET_OK;
}

view_model_t* temperature_view_model_create(navigator_request_t* req) {
  Temperature* aTemperature = new Temperature();
  return_value_if_fail(aTemperature != NULL, NULL);

  return temperature_view_model_create_with(aTemperature);
}
