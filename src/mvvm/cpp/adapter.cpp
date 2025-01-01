/**
 * File:  view_model_adapter
 * Author: AWTK Develop Team
 * Brief:  adapters between c and cpp
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/cpp/adapter.hpp"

namespace vm {

/****************************object*****************************/

typedef struct _object_adapter_t {
  tk_object_t obj;

  Object* cpp;
  bool_t auto_destroy_cpp;
} object_adapter_t;

static ret_t object_adapter_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_adapter_t* adapter = (object_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->SetProp(name, v);
}

static ret_t object_adapter_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_adapter_t* adapter = (object_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->GetProp(name, v);
}

static bool_t object_adapter_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_adapter_t* adapter = (object_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->CanExec(name, args);
}

static ret_t object_adapter_exec(tk_object_t* obj, const char* name, const char* args) {
  object_adapter_t* adapter = (object_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->Exec(name, args);
}

static ret_t object_adapter_on_destroy(tk_object_t* obj) {
  object_adapter_t* adapter = (object_adapter_t*)(obj);
  return_value_if_fail(adapter != NULL, RET_BAD_PARAMS);

  adapter->cpp->adapter = NULL;

  if (adapter->auto_destroy_cpp) {
    delete adapter->cpp;
    adapter->cpp = NULL;
  }

  return RET_OK;
}

static ret_t object_adapter_on_cpp_props_changed(void* ctx, event_t* event) {
  event_t e = *event;
  Object* obj = static_cast<Object*>(event->target);
  e.target = obj->adapter;
  return emitter_dispatch(EMITTER(ctx), &e);
}

static ret_t object_adapter_on_cpp_items_changed(void* ctx, event_t* event) {
  event_t e = *event;
  Object* obj = static_cast<Object*>(event->target);
  e.target = obj->adapter;
  return emitter_dispatch(EMITTER(ctx), &e);
}

static object_vtable_t s_object_adapter_vtable;
static ret_t object_adapter_init_vtable(object_vtable_t* vt) {
  vt->type = "object_adapter";
  vt->desc = "object_adapter for cpp";
  vt->size = sizeof(object_adapter_t);
  vt->exec = object_adapter_exec;
  vt->can_exec = object_adapter_can_exec;
  vt->get_prop = object_adapter_get_prop;
  vt->set_prop = object_adapter_set_prop;
  vt->on_destroy = object_adapter_on_destroy;

  return RET_OK;
}

static tk_object_t* object_cpp_create(Object* cpp, bool_t auto_destroy_cpp) {
  tk_object_t* obj = NULL;
  object_adapter_t* adapter = NULL;
  return_value_if_fail(cpp != NULL, NULL);

  if (cpp->adapter != NULL) {
    return TK_OBJECT_REF(cpp->adapter);
  }

  if (s_object_adapter_vtable.type == NULL) {
    object_adapter_init_vtable(&s_object_adapter_vtable);
  }

  obj = tk_object_create(&s_object_adapter_vtable);
  return_value_if_fail(obj != NULL, NULL);

  adapter = (object_adapter_t*)(obj);
  adapter->cpp = cpp;
  adapter->auto_destroy_cpp = auto_destroy_cpp;

  cpp->On(EVT_PROP_CHANGED, object_adapter_on_cpp_props_changed, obj);
  cpp->On(EVT_PROPS_CHANGED, object_adapter_on_cpp_props_changed, obj);
  cpp->On(EVT_ITEMS_CHANGED, object_adapter_on_cpp_items_changed, obj);
  cpp->adapter = TK_OBJECT(adapter);

  return obj;
}

/****************************view model*****************************/

typedef struct _view_model_adapter_t {
  view_model_t view_model;

  ViewModel* cpp;
  bool_t auto_destroy_cpp;
} view_model_adapter_t;

static ret_t view_model_adapter_on_will_mount(view_model_t* view_model, navigator_request_t* req) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(view_model);

  return adapter->cpp->OnWillMount(req);
}

static ret_t view_model_adapter_on_mount(view_model_t* view_model) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(view_model);

  return adapter->cpp->OnMount();
}

static ret_t view_model_adapter_on_will_unmount(view_model_t* view_model) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(view_model);

  return adapter->cpp->OnWillUnmount();
}

static ret_t view_model_adapter_on_unmount(view_model_t* view_model) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(view_model);

  return adapter->cpp->OnUnmount();
}

static ret_t view_model_adapter_on_destroy(tk_object_t* obj) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(obj);
  return_value_if_fail(adapter != NULL, RET_BAD_PARAMS);

  adapter->cpp->adapter = NULL;

  if (adapter->auto_destroy_cpp) {
    delete adapter->cpp;
    adapter->cpp = NULL;
  }

  return view_model_deinit(VIEW_MODEL(obj));
}

static object_vtable_t s_view_model_adapter_object_vtable;
static ret_t view_model_adapter_init_object_vtable(object_vtable_t* vt) {
  vt->type = "view_model_adapter";
  vt->desc = "view_model_adapter for cpp";
  vt->size = sizeof(view_model_adapter_t);
  vt->exec = object_adapter_exec;
  vt->can_exec = object_adapter_can_exec;
  vt->get_prop = object_adapter_get_prop;
  vt->set_prop = object_adapter_set_prop;
  vt->on_destroy = view_model_adapter_on_destroy;

  return RET_OK;
}

static view_model_vtable_t s_view_model_adapter_vtable;
static ret_t view_model_adapter_init_vtable(view_model_vtable_t* vt) {
  vt->on_will_mount = view_model_adapter_on_will_mount;
  vt->on_mount = view_model_adapter_on_mount;
  vt->on_will_unmount = view_model_adapter_on_will_unmount;
  vt->on_unmount = view_model_adapter_on_unmount;

  return RET_OK;
}

static view_model_t* view_model_cpp_create(ViewModel* cpp, bool_t auto_destroy_cpp) {
  tk_object_t* obj = NULL;
  view_model_t* view_model = NULL;
  view_model_adapter_t* adapter = NULL;
  return_value_if_fail(cpp != NULL, NULL);

  if (cpp->adapter != NULL) {
    return VIEW_MODEL(TK_OBJECT_REF(cpp->adapter));
  }

  if (s_view_model_adapter_object_vtable.type == NULL) {
    view_model_adapter_init_vtable(&s_view_model_adapter_vtable);
    view_model_adapter_init_object_vtable(&s_view_model_adapter_object_vtable);
  }

  obj = tk_object_create(&s_view_model_adapter_object_vtable);
  view_model = view_model_init(VIEW_MODEL(obj));
  return_value_if_fail(view_model != NULL, NULL);

  view_model->vt = &s_view_model_adapter_vtable;

  adapter = (view_model_adapter_t*)(view_model);
  adapter->cpp = cpp;
  adapter->auto_destroy_cpp = auto_destroy_cpp;

  cpp->On(EVT_PROP_CHANGED, object_adapter_on_cpp_props_changed, view_model);
  cpp->On(EVT_PROPS_CHANGED, object_adapter_on_cpp_props_changed, view_model);
  cpp->On(EVT_ITEMS_CHANGED, object_adapter_on_cpp_items_changed, view_model);
  cpp->adapter = TK_OBJECT(adapter);

  return view_model;
}

/****************************view model array*****************************/

typedef struct _view_model_array_adapter_t {
  view_model_array_t view_model_array;

  ViewModelArray* cpp;
  bool_t auto_destroy_cpp;
} view_model_array_adapter_t;

static ret_t view_model_array_adapter_set_prop(tk_object_t* obj, const char* name,
                                               const value_t* v) {
  uint32_t index = 0;
  const char* subname = NULL;
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_adapter_t* adapter = (view_model_array_adapter_t*)(view_model);

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(view_model, value_int(v));

    return RET_OK;
  }

  subname = tk_destruct_array_prop_name(name, &index);
  return_value_if_fail(subname != NULL && subname != name, RET_BAD_PARAMS);

  return adapter->cpp->SetProp(index, subname, v);
}

static ret_t view_model_array_adapter_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  const char* subname = NULL;
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_adapter_t* adapter = (view_model_array_adapter_t*)(view_model);

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, adapter->cpp->GetSize());
    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);
    return RET_OK;
  }

  subname = tk_destruct_array_prop_name(name, &index);
  return_value_if_fail(subname != NULL && subname != name, RET_BAD_PARAMS);

  return adapter->cpp->GetProp(index, subname, v);
}

static bool_t view_model_array_adapter_can_exec(tk_object_t* obj, const char* name,
                                                const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_adapter_t* adapter = (view_model_array_adapter_t*)(view_model);

  if (tk_str_ieq(name, "add")) {
    return TRUE;
  } else if (tk_str_ieq(name, "clear")) {
    return adapter->cpp->GetSize() > 0;
  }

  return adapter->cpp->CanExec(index, name);
}

static ret_t view_model_array_adapter_exec(tk_object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_adapter_t* adapter = (view_model_array_adapter_t*)(view_model);

  if (tk_str_ieq(name, "clear")) {
    adapter->cpp->Clear();
    return RET_ITEMS_CHANGED;
  }

  if (tk_str_ieq(name, "remove")) {
    adapter->cpp->Remove(index);
    return RET_ITEMS_CHANGED;
  }

  return adapter->cpp->Exec(index, name);
}

static ret_t view_model_array_adapter_on_destroy(tk_object_t* obj) {
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_adapter_t* adapter = (view_model_array_adapter_t*)(view_model);

  adapter->cpp->adapter = NULL;

  if (adapter->auto_destroy_cpp) {
    delete adapter->cpp;
    adapter->cpp = NULL;
  }

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static object_vtable_t s_view_model_array_adapter_object_vtable;
static ret_t view_model_array_adapter_init_object_vtable(object_vtable_t* vt) {
  vt->type = "view_model_array_adapter";
  vt->desc = "view_model_array_adapter for cpp";
  vt->is_collection = TRUE;
  vt->size = sizeof(view_model_array_adapter_t);
  vt->exec = view_model_array_adapter_exec;
  vt->can_exec = view_model_array_adapter_can_exec;
  vt->get_prop = view_model_array_adapter_get_prop;
  vt->set_prop = view_model_array_adapter_set_prop;
  vt->on_destroy = view_model_array_adapter_on_destroy;

  return RET_OK;
}

static view_model_vtable_t s_view_model_array_adapter_vtable;
static ret_t view_model_array_adapter_init_vtable(view_model_vtable_t* vt) {
  vt->on_will_mount = view_model_adapter_on_will_mount;
  vt->on_mount = view_model_adapter_on_mount;
  vt->on_will_unmount = view_model_adapter_on_will_unmount;
  vt->on_unmount = view_model_adapter_on_unmount;

  return RET_OK;
}

static view_model_t* view_model_array_cpp_create(ViewModelArray* cpp, bool_t auto_destroy_cpp) {
  tk_object_t* obj = NULL;
  view_model_t* view_model = NULL;
  view_model_array_adapter_t* adapter = NULL;
  return_value_if_fail(cpp != NULL, NULL);

  if (cpp->adapter != NULL) {
    return VIEW_MODEL(TK_OBJECT_REF(cpp->adapter));
  }

  if (s_view_model_array_adapter_object_vtable.type == NULL) {
    view_model_array_adapter_init_vtable(&s_view_model_array_adapter_vtable);
    view_model_array_adapter_init_object_vtable(&s_view_model_array_adapter_object_vtable);
  }

  obj = tk_object_create(&s_view_model_array_adapter_object_vtable);
  view_model = view_model_array_init(VIEW_MODEL(obj));
  return_value_if_fail(view_model != NULL, NULL);

  view_model->vt = &s_view_model_array_adapter_vtable;

  adapter = (view_model_array_adapter_t*)(view_model);
  adapter->cpp = cpp;
  adapter->auto_destroy_cpp = auto_destroy_cpp;

  cpp->On(EVT_PROP_CHANGED, object_adapter_on_cpp_props_changed, view_model);
  cpp->On(EVT_PROPS_CHANGED, object_adapter_on_cpp_props_changed, view_model);
  cpp->On(EVT_ITEMS_CHANGED, object_adapter_on_cpp_items_changed, view_model);
  cpp->adapter = TK_OBJECT(adapter);

  return view_model;
}

/****************************converter*****************************/

#define VALUE_CONVERTER_ADAPTER(obj) ((value_converter_adapter_t*)(obj))

typedef struct _value_converter_adapter_t {
  value_converter_t value_converter;

  ValueConverter* cpp;
  bool_t auto_destroy_cpp;
} value_converter_adapter_t;

static ret_t value_converter_adapter_on_destroy(tk_object_t* obj) {
  value_converter_adapter_t* adapter = (value_converter_adapter_t*)(obj);
  return_value_if_fail(adapter != NULL, RET_BAD_PARAMS);

  adapter->cpp->adapter = NULL;

  if (adapter->auto_destroy_cpp) {
    delete adapter->cpp;
    adapter->cpp = NULL;
  }

  return RET_OK;
}

static object_vtable_t s_value_converter_adapter_vtable;
static ret_t value_converter_adapter_init_vtable(object_vtable_t* vt) {
  vt->type = "value_converter_adapter";
  vt->desc = "value_converter_adapter for cpp";
  vt->size = sizeof(value_converter_adapter_t);
  vt->is_collection = FALSE;
  vt->on_destroy = value_converter_adapter_on_destroy;

  return RET_OK;
}

static ret_t value_converter_adapter_to_view(value_converter_t* c, const value_t* from,
                                             value_t* to) {
  value_converter_adapter_t* value_convert_adapter = VALUE_CONVERTER_ADAPTER(c);

  return value_convert_adapter->cpp->ToView(from, to);
}

static ret_t value_converter_adapter_to_model(value_converter_t* c, const value_t* from,
                                              value_t* to) {
  value_converter_adapter_t* value_convert_adapter = VALUE_CONVERTER_ADAPTER(c);

  return value_convert_adapter->cpp->ToModel(from, to);
}

static value_converter_t* value_converter_cpp_create(ValueConverter* cpp, bool_t auto_destroy_cpp) {
  tk_object_t* obj = NULL;
  value_converter_t* value_convert = NULL;
  value_converter_adapter_t* adapter = NULL;
  return_value_if_fail(cpp != NULL, NULL);

  if (cpp->adapter != NULL) {
    return VALUE_CONVERTER(TK_OBJECT_REF(cpp->adapter));
  }

  if (s_value_converter_adapter_vtable.type == NULL) {
    value_converter_adapter_init_vtable(&s_value_converter_adapter_vtable);
  }

  obj = tk_object_create(&s_value_converter_adapter_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_CONVERTER(obj);
  value_convert->to_view = value_converter_adapter_to_view;
  value_convert->to_model = value_converter_adapter_to_model;

  adapter = VALUE_CONVERTER_ADAPTER(obj);
  adapter->cpp = cpp;
  adapter->auto_destroy_cpp = auto_destroy_cpp;

  cpp->adapter = TK_OBJECT(adapter);

  return value_convert;
}

/****************************validator*****************************/

#define VALUE_VALIDATOR_ADAPTER(obj) ((value_validator_adapter_t*)(obj))

typedef struct _value_validator_adapter_t {
  value_validator_t value_validator;

  ValueValidator* cpp;
  bool_t auto_destroy_cpp;
} value_validator_adapter_t;

static ret_t value_validator_adapter_on_destroy(tk_object_t* obj) {
  value_validator_adapter_t* adapter = (value_validator_adapter_t*)(obj);
  return_value_if_fail(adapter != NULL, RET_BAD_PARAMS);

  adapter->cpp->adapter = NULL;

  if (adapter->auto_destroy_cpp) {
    delete adapter->cpp;
    adapter->cpp = NULL;
  }

  return RET_OK;
}

static object_vtable_t s_value_validator_adapter_vtable;
static ret_t value_validator_adapter_init_vtable(object_vtable_t* vt) {
  vt->type = "value_validator_adapter";
  vt->desc = "value_validator_adapter";
  vt->size = sizeof(value_validator_adapter_t);
  vt->is_collection = FALSE;
  vt->on_destroy = value_validator_adapter_on_destroy;

  return RET_OK;
}

static bool_t value_validator_adapter_is_valid(value_validator_t* c, const value_t* value,
                                               str_t* msg) {
  value_validator_adapter_t* value_convert_adapter = VALUE_VALIDATOR_ADAPTER(c);

  return value_convert_adapter->cpp->IsValid(value, msg);
}

static ret_t value_validator_adapter_fix(value_validator_t* c, value_t* value) {
  value_validator_adapter_t* value_convert_adapter = VALUE_VALIDATOR_ADAPTER(c);

  return value_convert_adapter->cpp->Fix(value);
}

static value_validator_t* value_validator_cpp_create(ValueValidator* cpp, bool_t auto_destroy_cpp) {
  tk_object_t* obj = NULL;
  value_validator_t* value_convert = NULL;
  return_value_if_fail(cpp != NULL, NULL);
  value_validator_adapter_t* adapter = NULL;

  if (cpp->adapter != NULL) {
    return VALUE_VALIDATOR(TK_OBJECT_REF(cpp->adapter));
  }

  if (s_value_validator_adapter_vtable.type == NULL) {
    value_validator_adapter_init_vtable(&s_value_validator_adapter_vtable);
  }

  obj = tk_object_create(&s_value_validator_adapter_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_VALIDATOR(obj);
  value_convert->fix = value_validator_adapter_fix;
  value_convert->is_valid = value_validator_adapter_is_valid;

  adapter = VALUE_VALIDATOR_ADAPTER(obj);
  adapter->cpp = cpp;
  adapter->auto_destroy_cpp = auto_destroy_cpp;

  cpp->adapter = TK_OBJECT(adapter);

  return value_convert;
}

tk_object_t* To(Object* cpp, bool_t auto_destroy_cpp) {
  return object_cpp_create(cpp, auto_destroy_cpp);
}

view_model_t* To(ViewModel* cpp, bool_t auto_destroy_cpp) {
  return view_model_cpp_create(cpp, auto_destroy_cpp);
}

view_model_t* To(ViewModelArray* cpp, bool_t auto_destroy_cpp) {
  return view_model_array_cpp_create(cpp, auto_destroy_cpp);
}

value_converter_t* To(ValueConverter* cpp, bool_t auto_destroy_cpp) {
  return value_converter_cpp_create(cpp, auto_destroy_cpp);
}

value_validator_t* To(ValueValidator* cpp, bool_t auto_destroy_cpp) {
  return value_validator_cpp_create(cpp, auto_destroy_cpp);
}

}  // namespace vm
