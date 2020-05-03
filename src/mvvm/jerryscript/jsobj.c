/*
 * File:   jsobj.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript wrapper
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "jsobj.h"
#include "tkc/utils.h"
#include "tkc/named_value.h"
#include "jerryscript-port.h"
#include "jerry_script_helper.h"
#include "tkc/object_default.h"
#include "jerryscript-ext/handler.h"

static ret_t jerry_value_to_obj(jerry_value_t value, object_t* obj);
static ret_t jerry_value_from_obj(jerry_value_t value, object_t* obj);

bool_t jsobj_has_prop(jerry_value_t obj, const char* name) {
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t has_prop_js = jerry_has_property(obj, prop_name);
  bool_t has_prop = (bool_t)jerry_get_boolean_value(has_prop_js);
  jerry_release_value(has_prop_js);
  jerry_release_value(prop_name);

  return has_prop;
}

jerry_value_t jsobj_get_global(const char* name) {
  jerry_value_t global_obj = jerry_get_global_object();
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t prop_value = jerry_get_property(global_obj, prop_name);
  jerry_release_value(prop_name);

  if (jerry_value_is_null(prop_value) || jerry_value_is_error(prop_value)) {
    jerry_release_value(prop_value);
    return global_obj;
  } else {
    jerry_release_value(global_obj);
    return prop_value;
  }
}

jerry_value_t jsobj_get_model(const char* name) {
  jerry_value_t view_model = jsobj_get_global(name);
  jerry_value_check(view_model);

  return view_model;
}

jerry_value_t jsobj_get_prop_value(jerry_value_t obj, const char* name) {
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t prop_value = jerry_get_property(obj, prop_name);
  jerry_release_value(prop_name);

  return prop_value;
}

ret_t jsobj_set_prop_value(jerry_value_t obj, const char* name, jerry_value_t prop_value) {
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);

  jerry_set_property(obj, prop_name, prop_value);

  jerry_release_value(prop_name);

  return RET_OK;
}

ret_t jsobj_set_prop(jerry_value_t obj, const char* name, const value_t* v, str_t* temp) {
  jerry_value_t prop_value = jerry_value_from_value(v, temp);
  ret_t ret = jsobj_set_prop_value(obj, name, prop_value);
  jerry_release_value(prop_value);

  return ret;
}

char* jerry_get_utf8_value(jerry_value_t value, str_t* temp) {
  jerry_size_t string_size = jerry_get_utf8_string_size(value);
  return_value_if_fail(str_extend(temp, string_size + 10) == RET_OK, NULL);

  jerry_string_to_utf8_char_buffer(value, (jerry_char_t*)temp->str, string_size);
  temp->size = string_size;
  temp->str[string_size] = '\0';

  return temp->str;
}

ret_t jsobj_get_prop(jerry_value_t obj, const char* name, value_t* v, str_t* temp) {
  ret_t ret = RET_FAIL;
  jerry_value_t prop_value = jsobj_get_prop_value(obj, name);

  if (jerry_value_check(prop_value) == RET_OK) {
    ret = jerry_value_to_value(prop_value, v, temp);
    jerry_release_value(prop_value);
  }

  return ret;
}

object_t* jsobj_get_prop_object(jerry_value_t obj, const char* name) {
  value_t v;
  return_value_if_fail(jsobj_get_prop(obj, name, &v, NULL) == RET_OK, NULL);
  return_value_if_fail(v.type == VALUE_TYPE_POINTER, NULL);

  return OBJECT(value_pointer(&v));
}

static jerry_value_t jerry_create_str(const char* str) {
  return str != NULL ? jerry_create_string((const jerry_char_t*)str) : jerry_create_null();
}

ret_t jsobj_exec(jerry_value_t obj, const char* name, const char* args) {
  jerry_value_t jsargs = jerry_create_str(args);
  ret_t ret = jsobj_exec_ex(obj, name, jsargs);
  jerry_release_value(jsargs);

  return ret;
}

ret_t jsobj_exec_ex(jerry_value_t obj, const char* name, jerry_value_t jsargs) {
  ret_t ret = RET_FAIL;

  if (jsobj_has_prop(obj, name)) {
    jerry_value_t func = jsobj_get_prop_value(obj, name);
    if (jerry_value_is_function(func)) {
      jerry_value_t jsret = jerry_call_function(func, obj, &jsargs, 1);
      ret = (ret_t)jerry_get_number_value(jsret);
      jerry_release_value(func);
      jerry_release_value(jsret);
    } else {
      ret = RET_NOT_IMPL;
      log_debug("not function %s\n", name);
    }
  } else {
    ret = RET_NOT_IMPL;
  }

  return ret;
}

bool_t jsobj_can_exec(jerry_value_t obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  char jsname[TK_NAME_LEN + 1];
  tk_snprintf(jsname, TK_NAME_LEN, "can%s", name);
  if (islower(jsname[3])) {
    jsname[3] = toupper(jsname[3]);
  }

  if (jsobj_has_prop(obj, jsname)) {
    jerry_value_t func = jsobj_get_prop_value(obj, jsname);

    if (jerry_value_is_function(func)) {
      jerry_value_t jsargs = jerry_create_str(args);
      jerry_value_t jsret = jerry_call_function(func, obj, &jsargs, 1);
      ret = jerry_get_boolean_value(jsret);
      jerry_release_value(func);
      jerry_release_value(jsret);
      jerry_release_value(jsargs);
    } else {
      ret = FALSE;
      log_debug("not function %s\n", name);
    }
  } else {
    ret = jsobj_has_prop_func(obj, name);
    if (!ret) {
      log_debug("not find function %s\n", name);
    }
  }

  return ret;
}

bool_t jsobj_has_prop_func(jerry_value_t obj, const char* name) {
  jerry_value_t value = jsobj_get_prop_value(obj, name);

  bool_t ret = jerry_value_is_function(value);

  jerry_release_value(value);

  return ret;
}

ret_t jerry_value_to_value(jerry_value_t value, value_t* v, str_t* temp) {
  ret_t ret = RET_NOT_IMPL;

  if (!jerry_value_is_error(value)) {
    if (jerry_value_is_string(value)) {
      char* str = jerry_get_utf8_value(value, temp);
      if (str != NULL) {
        value_set_str(v, str);
        ret = RET_OK;
      } else {
        ret = RET_OOM;
      }
    } else if (jerry_value_is_number(value)) {
      double raw_value = jerry_get_number_value(value);

      if (ceil(raw_value) == (int32_t)raw_value) {
        value_set_int32(v, (int32_t)raw_value);
      } else {
        value_set_double(v, raw_value);
      }
      ret = RET_OK;
    } else if (jerry_value_is_boolean(value)) {
      bool_t raw_value = jerry_get_boolean_value(value);
      value_set_bool(v, raw_value);
      ret = RET_OK;
    } else if (jerry_value_is_object(value)) {
      void* p = NULL;
      if (jerry_get_object_native_pointer(value, &p, NULL)) {
        value_set_pointer(v, p);
        ret = RET_OK;
      } else {
        object_t* obj = object_default_create();
        jerry_value_to_obj(value, obj);
        value_set_object(v, obj);
        v->free_handle = TRUE;
        ret = RET_OK;
      }
    } else {
      log_debug("not supported yet.\n");
    }
  }

  return ret;
}

jerry_value_t jerry_value_from_value(const value_t* v, str_t* temp) {
  jerry_value_t value;

  if (v->type == VALUE_TYPE_STRING) {
    value = jerry_create_string((const jerry_char_t*)value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    if (temp != NULL) {
      str_from_value(temp, v);
      value = jerry_create_string((const jerry_char_t*)temp->str);
    } else {
      value = jerry_create_string((const jerry_char_t*)"");
    }
  } else if (v->type == VALUE_TYPE_POINTER) {
    value = jerry_create_object();
    jerry_set_object_native_pointer(value, (void*)value_pointer(v), NULL);
  } else if (v->type == VALUE_TYPE_OBJECT) {
    object_t* obj = OBJECT(value_object(v));
    value = jerry_create_object();
    jerry_value_from_obj(value, obj);
  } else {
    value = jerry_create_number(value_float(v));
  }

  return value;
}

void* jerry_value_to_pointer(jerry_value_t value) {
  value_t v;
  value_set_pointer(&v, NULL);
  jerry_value_to_value(value, &v, NULL);

  return value_pointer(&v);
}

jerry_value_t jerry_value_from_pointer(void* ptr) {
  value_t v;
  value_set_pointer(&v, ptr);

  return jerry_value_from_value(&v, NULL);
}

void* jsobj_get_prop_pointer(jerry_value_t obj, const char* name) {
  void* p = NULL;
  jerry_value_t value = jsobj_get_prop_value(obj, name);
  jerry_get_object_native_pointer(value, &p, NULL);
  jerry_release_value(value);

  return p;
}

ret_t jsobj_set_prop_pointer(jerry_value_t obj, const char* name, void* ptr) {
  value_t v;
  value_set_pointer(&v, ptr);

  return jsobj_set_prop(obj, name, &v, NULL);
}

ret_t jsobj_set_prop_object(jerry_value_t obj, const char* name, object_t* ptr) {
  value_t v;
  value_set_object(&v, ptr);

  return jsobj_set_prop(obj, name, &v, NULL);
}

ret_t jsobj_set_prop_func(jerry_value_t obj, const char* name, jerry_external_handler_t handler_p) {
  jerry_value_t func = jerry_create_external_function(handler_p);
  ret_t ret = jsobj_set_prop_value(obj, name, func);
  jerry_release_value(func);

  return ret;
}

ret_t jsobj_set_prop_str(jerry_value_t obj, const char* name, const char* value) {
  value_t v;
  value_set_str(&v, value);

  return jsobj_set_prop(obj, name, &v, NULL);
}

ret_t jsobj_set_prop_int(jerry_value_t obj, const char* name, int32_t value) {
  value_t v;
  value_set_int(&v, value);

  return jsobj_set_prop(obj, name, &v, NULL);
}

static jerry_value_t jsobj_get_converter(const char* name) {
  jerry_value_t factory = jsobj_get_global(JSOBJ_VALUE_CONVERTERS);
  jerry_value_t converter = jsobj_get_prop_value(factory, name);
  jerry_release_value(factory);

  return converter;
}

bool_t jsvalue_converter_exist(const char* name) {
  jerry_value_t factory = jsobj_get_global(JSOBJ_VALUE_CONVERTERS);
  bool_t exist = jsobj_has_prop(factory, name);
  jerry_release_value(factory);

  return exist;
}

static ret_t value_convert(const char* converter_name, const char* func_name, const value_t* from,
                           value_t* to, str_t* temp) {
  ret_t ret = RET_OK;
  jerry_value_t converter = jsobj_get_converter(converter_name);
  jerry_value_t func = jsobj_get_prop_value(converter, func_name);

  if (jerry_value_is_function(func)) {
    jerry_value_t jsfrom = jerry_value_from_value(from, temp);
    jerry_value_t jsret = jerry_call_function(func, converter, &jsfrom, 1);
    ret = jerry_value_to_value(jsret, to, temp);
    jerry_release_value(jsret);
  }

  jerry_release_value(func);
  jerry_release_value(converter);

  return ret;
}

ret_t jsvalue_converter_to_view(const char* name, const value_t* from, value_t* to, str_t* temp) {
  return value_convert(name, JSOBJ_VALUE_CONVERTER_TO_VIEW, from, to, temp);
}

ret_t jsvalue_converter_to_model(const char* name, const value_t* from, value_t* to, str_t* temp) {
  return value_convert(name, JSOBJ_VALUE_CONVERTER_TO_VIEW_MODEL, from, to, temp);
}

static jerry_value_t jsobj_get_validator(const char* name) {
  jerry_value_t factory = jsobj_get_global(JSOBJ_VALUE_VALIDATORS);
  jerry_value_t validator = jsobj_get_prop_value(factory, name);
  jerry_release_value(factory);

  return validator;
}

bool_t jsvalue_validator_exist(const char* name) {
  jerry_value_t factory = jsobj_get_global(JSOBJ_VALUE_VALIDATORS);
  bool_t exist = jsobj_has_prop(factory, name);
  jerry_release_value(factory);

  return exist;
}

ret_t jsvalue_validator_is_valid(const char* name, const value_t* value, str_t* msg) {
  ret_t ret = RET_OK;
  jerry_value_t validator = jsobj_get_validator(name);
  jerry_value_t func = jsobj_get_prop_value(validator, JSOBJ_VALUE_VALIDATOR_IS_VALID);

  if (jerry_value_is_function(func)) {
    jerry_value_t jsvalue = jerry_value_from_value(value, msg);
    jerry_value_t jsret = jerry_call_function(func, validator, &jsvalue, 1);

    if (jerry_value_is_boolean(jsret)) {
      ret = jerry_value_to_boolean(jsret) ? RET_OK : RET_FAIL;
      str_set(msg, "");
    } else if (jerry_value_is_object(jsret)) {
      value_t r;
      value_set_int(&r, 0);
      if (jsobj_has_prop(jsret, JSOBJ_VALUE_VALIDATOR_RESULT)) {
        jsobj_get_prop(jsret, JSOBJ_VALUE_VALIDATOR_RESULT, &r, msg);
        ret = value_bool(&r) ? RET_OK : RET_FAIL;
      }

      if (jsobj_has_prop(jsret, JSOBJ_VALUE_VALIDATOR_MESSAGE)) {
        jsobj_get_prop(jsret, JSOBJ_VALUE_VALIDATOR_MESSAGE, &r, msg);
      }
    }
    jerry_release_value(jsret);
  }

  jerry_release_value(func);
  jerry_release_value(validator);

  return ret;
}

ret_t jsvalue_validator_fix(const char* name, value_t* v) {
  ret_t ret = RET_OK;
  jerry_value_t validator = jsobj_get_validator(name);
  jerry_value_t func = jsobj_get_prop_value(validator, JSOBJ_VALUE_VALIDATOR_FIX);

  if (jerry_value_is_function(func)) {
    jerry_value_t jsvalue = jerry_value_from_value(v, NULL);
    jerry_value_t jsret = jerry_call_function(func, validator, &jsvalue, 1);
    ret = jerry_value_to_value(jsret, v, NULL);
    jerry_release_value(jsret);
  } else {
    ret = RET_OK;
  }

  jerry_release_value(func);
  jerry_release_value(validator);

  return ret;
}

/*******************************************/

typedef struct _jerry_value_from_obj_ctx_t {
  str_t* str;
  jerry_value_t value;
} jerry_value_from_obj_ctx_t;

static ret_t visit_object_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  jerry_value_from_obj_ctx_t* info = (jerry_value_from_obj_ctx_t*)ctx;

  jsobj_set_prop(info->value, nv->name, &(nv->value), info->str);

  return RET_OK;
}

static ret_t jerry_value_from_obj(jerry_value_t value, object_t* obj) {
  str_t str;
  ret_t ret = RET_OK;
  jerry_value_from_obj_ctx_t info;

  str_init(&str, 0);

  info.str = &str;
  info.value = value;

  ret = object_foreach_prop(obj, visit_object_prop, &info);

  str_reset(&str);

  return ret;
}

/*******************************************/

#define STR_NATIVE_REQ "nativeRequest"

typedef struct _jerry_value_to_obj_ctx_t {
  str_t* name_str;
  str_t* value_str;
  object_t* obj;
} jerry_value_to_obj_ctx_t;

static bool visit_jerry_object_prop(const jerry_value_t property_name,
                                    const jerry_value_t property_value, void* user_data_p) {
  value_t name;
  value_t value;
  jerry_value_to_obj_ctx_t* info = (jerry_value_to_obj_ctx_t*)user_data_p;

  jerry_value_to_value(property_name, &name, info->name_str);
  if (tk_str_eq(value_str(&name), JSOBJ_ON_RESULT)) {
    jerry_acquire_value(property_value);
    value_set_int(&value, property_value);
  } else {
    jerry_value_to_value(property_value, &value, info->value_str);
  }

  object_set_prop(OBJECT(info->obj), value_str(&name), &value);

  value_reset(&name);
  value_reset(&value);

  return true;
}

static ret_t jerry_value_to_obj(jerry_value_t value, object_t* obj) {
  str_t str_name;
  str_t str_value;
  jerry_value_to_obj_ctx_t info;

  str_init(&str_name, 0);
  str_init(&str_value, 0);

  info.obj = obj;
  info.name_str = &str_name;
  info.value_str = &str_value;

  jerry_foreach_object_property(value, visit_jerry_object_prop, &info);

  str_reset(&str_name);
  str_reset(&str_value);

  return RET_OK;
}

/*******************************************/

static ret_t jerryscript_on_result(navigator_request_t* req, const value_t* result) {
  jerry_value_t func = (jerry_value_t)object_get_prop_int(OBJECT(req), JSOBJ_ON_RESULT, 0);
  if (func && jerry_value_is_function(func)) {
    str_t str;
    str_init(&str, 0);
    jerry_value_t thiz = jerry_create_null();
    jerry_value_t jsargs = jerry_value_from_value(result, &str);
    jerry_value_t jsret = jerry_call_function(func, thiz, &jsargs, 1);

    str_reset(&str);
    jerry_release_value(func);
    jerry_release_value(thiz);
    jerry_release_value(jsret);
    jerry_release_value(jsargs);
  } else {
    log_debug("no onResult\n");
  }

  return RET_OK;
}

navigator_request_t* jerry_value_to_navigator_request(jerry_value_t obj) {
  value_t v;
  str_t str;
  ret_t ret = RET_OK;
  const char* target = NULL;
  navigator_request_t* req = NULL;

  str_init(&str, 0);
  value_set_str(&v, NULL);

  ret = jsobj_get_prop(obj, NAVIGATOR_ARG_TARGET, &v, &str);
  goto_error_if_fail(ret == RET_OK);

  target = value_str(&v);
  goto_error_if_fail(target != NULL);

  req = navigator_request_create(target, jerryscript_on_result);
  goto_error_if_fail(req != NULL);

  jerry_value_to_obj(obj, OBJECT(req));
error:
  str_reset(&str);
  value_reset(&v);

  return req;
}

jerry_value_t js_return_result(const jerry_value_t func_obj_val, const jerry_value_t this_p,
                               const jerry_value_t args_p[], const jerry_length_t args_cnt) {
  if (args_cnt > 0) {
    value_t v;
    str_t str;
    jerry_value_t result = args_p[0];
    navigator_request_t* req = NAVIGATOR_REQUEST(jsobj_get_prop_pointer(this_p, STR_NATIVE_REQ));

    str_init(&str, 0);
    value_set_int(&v, 0);
    if (jerry_value_to_value(result, &v, &str) != RET_OK) {
      log_warn("js_return_result: jerry_value_to_value failed\n");
    }
    navigator_request_on_result(req, &v);
    str_reset(&str);
  } else {
    log_warn("wrap_notify_props_changed no nativeModel\n");
  }

  return jerry_create_number(0);
}

jerry_value_t jerry_value_from_navigator_request(navigator_request_t* req) {
  jerry_value_t obj = jerry_create_object();

  if (req != NULL) {
    jerry_value_from_obj(obj, OBJECT(req));
    ENSURE(jsobj_set_prop_str(obj, NAVIGATOR_ARG_TARGET, req->target) == RET_OK);
    ENSURE(jsobj_set_prop_pointer(obj, STR_NATIVE_REQ, OBJECT(req)) == RET_OK);
  }

  ENSURE(jsobj_set_prop_func(obj, JSOBJ_ON_RESULT, js_return_result) == RET_OK);

  return obj;
}
