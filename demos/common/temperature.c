#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "temperature.h"


/***************temperature***************/;

temperature_t* temperature_create(void) {
  temperature_t* temperature = TKMEM_ZALLOC(temperature_t);
  return_value_if_fail(temperature != NULL, NULL);



  return temperature;
} 

static ret_t temperature_destroy(temperature_t* temperature) {
  return_value_if_fail(temperature != NULL, RET_BAD_PARAMS);



  TKMEM_FREE(temperature);

  return RET_OK;
}

static bool_t temperature_can_exec_apply(temperature_t* temperature, const char* args) {
  return temperature->old_value != temperature->value;
}

static ret_t temperature_apply(temperature_t* temperature, const char* args) {
  temperature->old_value = temperature->value;
  return RET_OBJECT_CHANGED;
}

/***************temperature_view_model***************/

static ret_t temperature_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  temperature_t* temperature = vm->temperature;

  if (tk_str_eq("value", name)) {
    temperature->value =  value_double(v);
  
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static ret_t temperature_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  temperature_t* temperature = vm->temperature;

  if (tk_str_eq("value", name)) {
    value_set_double(v, temperature->value);
  
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static bool_t temperature_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  temperature_t* temperature = vm->temperature;

  if (tk_str_eq("apply", name)) {
    return temperature_can_exec_apply(temperature, args);
  } else {
    return FALSE;
  }
}

static ret_t temperature_view_model_exec(object_t* obj, const char* name, const char* args) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  temperature_t* temperature = vm->temperature;

  if (tk_str_eq("apply", name)) {
    return temperature_apply(temperature, args);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

static ret_t temperature_view_model_on_destroy(object_t* obj) {
  temperature_view_model_t* vm = (temperature_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  temperature_destroy(vm->temperature);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_temperature_view_model_vtable = {
  .type = "temperature",
  .desc = "temperature controller",
  .size = sizeof(temperature_view_model_t),
  .exec = temperature_view_model_exec,
  .can_exec = temperature_view_model_can_exec,
  .get_prop = temperature_view_model_get_prop,
  .set_prop = temperature_view_model_set_prop,
  .on_destroy = temperature_view_model_on_destroy
};

view_model_t* temperature_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_temperature_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  temperature_view_model_t* temperature_view_model = (temperature_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  temperature_view_model->temperature = temperature_create();
  ENSURE(temperature_view_model->temperature != NULL);

  return vm;
}
