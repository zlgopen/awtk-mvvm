#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/object_array.h"
#include "mvvm/base/utils.h"
#include "devices_view_model.h"

typedef enum _device_type_t {
  DEV_TYPE_PACK_SKP_1000 = 0,
  DEV_TYPE_PACK_SKP_2000,
  DEV_TYPE_PACK_SKP_3042,
  DEV_TYPE_PACK_SKP_3132,
  DEV_TYPE_PACK_SKP_3142,
  DEV_TYPE_PACK_SKP_5002,

  DEV_TYPE_MAX_COUNT
} device_type_t;

typedef enum _device_params_t {
  DEV_PARAMS_NONE = 0,
  DEV_PARAMS_NTC_2252K,
  DEV_PARAMS_NTC_5K,
  DEV_PARAMS_NTC_10K,

  DEV_PARAMS_MAX_COUNT
} device_params_t;

static object_t* device_view_model_create_device(device_type_t type) {
  object_t* obj = object_default_create();
  return_value_if_fail(obj != NULL, NULL);

  object_set_prop_int32(obj, "pack_type", type);
  object_set_prop_int32(obj, "pack_params", random() % DEV_PARAMS_MAX_COUNT);

  if (type == DEV_TYPE_PACK_SKP_1000) {
    object_set_prop_bool(obj, "io1", random() % 2 == 0);
    object_set_prop_bool(obj, "io2", random() % 2 == 0);
  } else if (type == DEV_TYPE_PACK_SKP_2000 || type == DEV_TYPE_PACK_SKP_3042) {
    object_set_prop_double(obj, "temp", random());
  } else if (type == DEV_TYPE_PACK_SKP_3132 || type == DEV_TYPE_PACK_SKP_3142) {
    object_set_prop_int32(obj, "a1", random());
    object_set_prop_int32(obj, "a2", random());
  } else {
    object_set_prop_double(obj, "tps", random());
  }

  return obj;
}

static ret_t device_view_model_reset_devices(devices_view_model_t* vm) {
  uint32_t i;
  object_t* obj;

  for (i = 0; i < 300; i++) {
    obj = device_view_model_create_device(random() % DEV_TYPE_MAX_COUNT);
    object_set_prop_object(vm->devices, "-1", obj);
    emitter_on(EMITTER(obj), EVT_PROP_CHANGED, (event_func_t)emitter_dispatch, vm);
    OBJECT_UNREF(obj);
  }

  return RET_OK;
}

static ret_t devices_view_model_on_create(view_model_t* view_model, navigator_request_t* req) {
  devices_view_model_t* vm = (devices_view_model_t*)(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  vm->unlocked = FALSE;
  vm->current_index = -1;
  vm->devices = object_array_create();
  device_view_model_reset_devices(vm);

  return RET_OK;
}

static ret_t devices_view_model_on_destroy(object_t* obj) {
  devices_view_model_t* vm = (devices_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  OBJECT_UNREF(vm->devices);

  return view_model_deinit(VIEW_MODEL(obj));
}

static ret_t devices_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  devices_view_model_t* vm = ((devices_view_model_t*)(obj));

  if (tk_str_ieq("unlocked", name)) {
    vm->unlocked = value_bool(v);
    return RET_OK;
  } else if (tk_str_ieq("currentIndex", name)) {
    vm->current_index = value_int32(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t devices_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  devices_view_model_t* vm = ((devices_view_model_t*)(obj));

  if (tk_str_ieq("items", name)) {
    value_set_object(v, vm->devices);
    return RET_OK;
  } else if (tk_str_ieq("unlocked", name)) {
    value_set_bool(v, vm->unlocked);
    return RET_OK;
  } else if (tk_str_ieq("currentIndex", name)) {
    value_set_int32(v, vm->current_index);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t devices_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  int32_t len;
  devices_view_model_t* vm = (devices_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, FALSE);

  len = object_get_prop_int32(vm->devices, "length", -1);
  if (tk_str_ieq("remove", name)) {
    int32_t index = vm->current_index;
    return index >= 0 && index < len;
  } else if (tk_str_ieq("insert", name)) {
    int32_t index = vm->current_index;
    return index >= 0 && index < len;
  } else if (tk_str_ieq("clear", name)) {
    return len > 0;
  } else if (tk_str_ieq("reset", name)) {
    return len == 0;
  } else if (tk_str_ieq("setCurrent", name)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t devices_view_model_exec(object_t* obj, const char* name, const char* args) {
  devices_view_model_t* vm = (devices_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq("remove", name)) {
    if (object_array_remove(vm->devices, vm->current_index) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->devices);
    }

    return RET_OK;
  } else if (tk_str_ieq("insert", name)) {
    value_t v;
    object_t* o = device_view_model_create_device(random() % DEV_TYPE_MAX_COUNT);
    return_value_if_fail(o != NULL, RET_FAIL);

    value_set_object(&v, o);
    emitter_on(EMITTER(o), EVT_PROP_CHANGED, (event_func_t)emitter_dispatch, vm);

    if (object_array_insert(vm->devices, vm->current_index, &v) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->devices);
    }

    OBJECT_UNREF(o);
    return RET_OK;
  } else if (tk_str_ieq("clear", name)) {
    if (object_array_clear_props(vm->devices) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->devices);
    }
    return RET_OK;
  } else if (tk_str_ieq("reset", name)) {
    if (device_view_model_reset_devices(vm) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->devices);
    }
    return RET_OK;
  } else if (tk_str_ieq("setCurrent", name)) {
    object_t* a = object_default_create();
    tk_command_arguments_to_object(args, a);
    vm->current_index = object_get_prop_int32(a, "index", -1);
    OBJECT_UNREF(a);
    return RET_OBJECT_CHANGED;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_devices_view_model_vtable = {"devices_view_model_t",
                                                            "devices_view_model_t",
                                                            sizeof(devices_view_model_t),
                                                            FALSE,
                                                            devices_view_model_on_destroy,
                                                            NULL,
                                                            devices_view_model_get_prop,
                                                            devices_view_model_set_prop,
                                                            NULL,
                                                            NULL,
                                                            devices_view_model_can_exec,
                                                            devices_view_model_exec};

view_model_t* devices_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_devices_view_model_vtable);
  view_model_t* view_model = view_model_init(VIEW_MODEL(obj));
  return_value_if_fail(view_model != NULL, NULL);

  devices_view_model_on_create(view_model, req);

  return view_model;
}
