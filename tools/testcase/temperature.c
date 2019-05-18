#include "tkc/mem.h"
#include "tkc/utils.h"
#include "temperature.h"

static ret_t temperature_set_prop(object_t* obj, const char* name, const value_t* v) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("value", name)) {
    vm->value = value_double(v);
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static ret_t temperature_get_prop(object_t* obj, const char* name, value_t* v) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("value", name)) {
    double value = vm->value;
    value_set_double(v, value);
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}

static bool_t temperature_can_exec_apply(temperature_t* vm, const char* args) {
  return vm->old_value != vm->value;;
}

static bool_t temperature_apply(temperature_t* vm, const char* args) {
  /*TODO: really apply the value*/
  vm->old_value = vm->value;;
  return RET_OK;
}


static bool_t temperature_can_exec(object_t* obj, const char* name, const char* args) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("apply", name)) {
    return temperature_can_exec_apply(vm, args);
  } else {
    return FALSE;
  }
}

static ret_t temperature_exec(object_t* obj, const char* name, const char* args) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("apply", name)) {
    return temperature_apply(vm, args);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t temperature_on_destroy(object_t* obj) {
  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_temperature_vtable = {
  .type = "temperature",
  .desc = "temperature controller",
  .size = sizeof(temperature_t),
  .exec = temperature_exec,
  .can_exec = temperature_can_exec,
  .get_prop = temperature_get_prop,
  .set_prop = temperature_set_prop,
  .on_destroy = temperature_on_destroy
};

view_model_t* temperature_create(navigator_request_t* req) {
  return view_model_init(VIEW_MODEL(object_create(&s_temperature_vtable)));
}
