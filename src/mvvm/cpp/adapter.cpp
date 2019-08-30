/**
 * File:  view_model_adapter
 * Author: AWTK Develop Team
 * Brief:  adapters between c and cpp
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

/****************************view model*****************************/

typedef struct _view_model_adapter_t {
  view_model_t view_model;

  ViewModel* cpp;
} view_model_adapter_t;

static ret_t view_model_adapter_set_prop(object_t* obj, const char* name, const value_t* v) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->SetProp(name, v);
}

static ret_t view_model_adapter_get_prop(object_t* obj, const char* name, value_t* v) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->GetProp(name, v);
}

static bool_t view_model_adapter_can_exec(object_t* obj, const char* name, const char* args) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->CanExec(name, args);
}

static ret_t view_model_adapter_exec(object_t* obj, const char* name, const char* args) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(obj);
  return_value_if_fail(adapter->cpp != NULL, RET_BAD_PARAMS);

  return adapter->cpp->Exec(name, args);
}

static ret_t view_model_adapter_on_destroy(object_t* obj) {
  view_model_adapter_t* adapter = (view_model_adapter_t*)(obj);
  return_value_if_fail(adapter != NULL, RET_BAD_PARAMS);

  delete adapter->cpp;
  adapter->cpp = NULL;

  return view_model_deinit(VIEW_MODEL(obj));
}

static object_vtable_t s_view_model_adapter_vtable;

static ret_t view_model_adapter_init_vtable(object_vtable_t* vt) {
  vt->type = "view_model_adapter";
  vt->desc = "view_model_adapter for cpp";
  vt->size = sizeof(view_model_adapter_t);
  vt->exec = view_model_adapter_exec;
  vt->can_exec = view_model_adapter_can_exec;
  vt->get_prop = view_model_adapter_get_prop;
  vt->set_prop = view_model_adapter_set_prop;
  vt->on_destroy = view_model_adapter_on_destroy;

  return RET_OK;
}

static ret_t view_model_adapter_on_cpp_changed(void* ctx, event_t* event) {
  return emitter_dispatch(EMITTER(ctx), event);
}

view_model_t* view_model_cpp_create(ViewModel* cpp) {
  object_t* obj = NULL;
  view_model_t* view_model = NULL;
  view_model_adapter_t* adapter = NULL;
  return_value_if_fail(cpp != NULL, NULL);

  if (s_view_model_adapter_vtable.type == NULL) {
    view_model_adapter_init_vtable(&s_view_model_adapter_vtable);
  }

  obj = object_create(&s_view_model_adapter_vtable);
  return_value_if_fail(obj != NULL, NULL);

  view_model = view_model_init(VIEW_MODEL(obj));
  adapter = (view_model_adapter_t*)(view_model);

  adapter->cpp = cpp;
  cpp->On(EVT_PROP_CHANGED, view_model_adapter_on_cpp_changed, view_model);
  cpp->On(EVT_PROPS_CHANGED, view_model_adapter_on_cpp_changed, view_model);

  return view_model;
}

/****************************converter*****************************/

static object_vtable_t s_value_converter_adapter_vtable;
typedef struct _value_converter_adapter_t {
  value_converter_t value_converter;

  /*private*/
  ValueConverter* cpp;
} value_converter_adapter_t;

static ret_t value_converter_adapter_init_vtable(object_vtable_t* vt) {
  vt->type = "value_converter_adapter";
  vt->desc = "value_converter_adapter";
  vt->size = sizeof(value_converter_adapter_t);
  vt->is_collection = FALSE;

  return RET_OK;
}

#define VALUE_CONVERTER_ADAPTER(obj) ((value_converter_adapter_t*)(obj))

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

value_converter_t* value_converter_cpp_create(ValueConverter* cpp) {
  object_t* obj = NULL;
  value_converter_t* value_convert = NULL;
  value_converter_adapter_t* value_convert_adapter = NULL;
  return_value_if_fail(cpp != NULL, NULL);

  value_converter_adapter_init_vtable(&s_value_converter_adapter_vtable);
  obj = object_create(&s_value_converter_adapter_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_CONVERTER(obj);
  value_convert->to_view = value_converter_adapter_to_view;
  value_convert->to_model = value_converter_adapter_to_model;

  value_convert_adapter = VALUE_CONVERTER_ADAPTER(obj);
  value_convert_adapter->cpp = cpp;

  return value_convert;
}

/****************************validator*****************************/

static object_vtable_t s_value_validator_adapter_vtable;

typedef struct _value_validator_adapter_t {
  value_validator_t value_validator;

  /*private*/
  ValueValidator* cpp;
} value_validator_adapter_t;

static ret_t value_validator_adapter_init_vtable(object_vtable_t* vt) {
  vt->type = "value_validator_adapter";
  vt->desc = "value_validator_adapter";
  vt->size = sizeof(value_validator_adapter_t);
  vt->is_collection = FALSE;

  return RET_OK;
}

#define VALUE_VALIDATOR_ADAPTER(obj) ((value_validator_adapter_t*)(obj))

static bool_t value_validator_adapter_is_valid(value_validator_t* c, const value_t* value,
                                               str_t* msg) {
  value_validator_adapter_t* value_convert_adapter = VALUE_VALIDATOR_ADAPTER(c);

  return value_convert_adapter->cpp->IsValid(value, msg);
}

static ret_t value_validator_adapter_fix(value_validator_t* c, value_t* value) {
  value_validator_adapter_t* value_convert_adapter = VALUE_VALIDATOR_ADAPTER(c);

  return value_convert_adapter->cpp->Fix(value);
}

value_validator_t* value_validator_cpp_create(ValueValidator* cpp) {
  object_t* obj = NULL;
  value_validator_t* value_convert = NULL;
  return_value_if_fail(cpp != NULL, NULL);
  value_validator_adapter_t* value_convert_adapter = NULL;

  value_validator_adapter_init_vtable(&s_value_validator_adapter_vtable);
  obj = object_create(&s_value_validator_adapter_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_VALIDATOR(obj);
  value_convert->fix = value_validator_adapter_fix;
  value_convert->is_valid = value_validator_adapter_is_valid;

  value_convert_adapter = VALUE_VALIDATOR_ADAPTER(obj);
  value_convert_adapter->cpp = cpp;

  return value_convert;
}

/****************************view model array*****************************/

view_model_t* view_model_array_cpp_create(ViewModelArray* cpp) {
  /*TODO*/
  return NULL;
}
