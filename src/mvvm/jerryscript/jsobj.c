/*
 * File:   jsobj.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript wrapper
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/named_value.h"
#include "mvvm/base/utils.h"
#include "mvvm/jerryscript/object_js_factory.h"
#include "mvvm/jerryscript/jsobj.h"

ret_t js_gc() {
  jsvalue_t jsarg = JS_UNDEFINED;
  jsvalue_t jsret = jerryx_handler_gc(NULL, &jsarg, 1);
  jsvalue_unref(jsarg);
  jsvalue_unref(jsret);

  return RET_OK;
}

jsvalue_t jsvalue_get_global(const char* name) {
  jsvalue_t global_obj = jsvalue_get_global_object();
  jsvalue_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jsvalue_t prop_value = jerry_get_property(global_obj, prop_name);

  jsvalue_unref(global_obj);
  jsvalue_unref(prop_name);

  if (jerry_value_is_error(prop_value)) {
    jsvalue_unref(prop_value);
    return JS_UNDEFINED;
  }

  return prop_value;
}

int32_t jsvalue_compare(jsvalue_t value, jsvalue_t other) {
  if (jerry_binary_operation(JERRY_BIN_OP_EQUAL, value, other)) {
    return 0;
  } else if (jerry_binary_operation(JERRY_BIN_OP_LESS, value, other)) {
    return -1;
  } else {
    return 1;
  }
}

jsvalue_t jsvalue_from_utf8(const char* str) {
  return str != NULL ? jerry_create_string((const jerry_char_t*)str) : JS_UNDEFINED;
}

char* jsvalue_to_utf8(jsvalue_t value, str_t* temp) {
  jerry_size_t string_size = jerry_get_utf8_string_size(value);
  return_value_if_fail(str_extend(temp, string_size + 1) == RET_OK, NULL);

  jerry_string_to_utf8_char_buffer(value, (jerry_char_t*)temp->str, string_size);
  temp->size = string_size;
  temp->str[string_size] = '\0';

  return temp->str;
}

static JSFUNC_DECL(wrap_object_t_get_prop) {
  jsvalue_t ret;

  if (args_count >= 1) {
    tk_object_t* obj = jsobj_get_native_ptr(call_info_p->this_value);
    if (obj != NULL) {
      const char* name = NULL;
      str_t temp;
      value_t v;

      str_init(&temp, 0);
      name = jsvalue_to_utf8(args_p[0], &temp);
      if (tk_object_get_prop(obj, name, &v) == RET_OK) {
        ret = jsvalue_from_value(&v, &temp);
      } else {
        ret = JS_UNDEFINED;
      }
      str_reset(&temp);

      return ret;
    }
  }

  return JS_UNDEFINED;
}

static JSFUNC_DECL(wrap_object_t_set_prop) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 2) {
    tk_object_t* obj = jsobj_get_native_ptr(call_info_p->this_value);
    if (obj != NULL) {
      const char* name = NULL;
      str_t temp_key;
      str_t temp_val;
      value_t v;

      str_init(&temp_key, 0);
      str_init(&temp_val, 0);

      name = jsvalue_to_utf8(args_p[0], &temp_key);
      if (jsvalue_to_value(args_p[1], &v, &temp_val) == RET_OK) {
        ret = tk_object_set_prop(obj, name, &v);
      }

      str_reset(&temp_key);
      str_reset(&temp_val);
    }
  }

  return jsvalue_from_number(ret);
}

static const char* jsvalue_to_args_string(jsvalue_t value, str_t* temp) {
  const char* args = NULL;

  if (jerry_value_is_object(value)) {
    tk_object_t* obj = jsvalue_to_obj(value);
    if (obj != NULL) {
      str_set(temp, COMMAND_ARGS_STRING_PREFIX);
      if (tk_command_arguments_from_object(obj, temp) == RET_OK) {
        args = temp->str;
      }
    }
  } else {
    args = jsvalue_to_utf8(value, temp);
  }

  return args;
}

static JSFUNC_DECL(wrap_object_t_can_exec) {
  bool_t ret = TRUE;

  if (args_count >= 1) {
    tk_object_t* obj = jsobj_get_native_ptr(call_info_p->this_value);
    if (obj != NULL) {
      const char* name = NULL;
      const char* args = NULL;
      str_t temp_name;
      str_t temp_args;

      str_init(&temp_name, 0);
      str_init(&temp_args, 0);

      name = jsvalue_to_utf8(args_p[0], &temp_name);
      if (args_count >= 2) {
        args = jsvalue_to_args_string(args_p[1], &temp_args);
      }

      ret = tk_object_can_exec(obj, name, args);

      str_reset(&temp_name);
      str_reset(&temp_args);
    }
  }

  return jsvalue_from_boolean(ret);
}

static JSFUNC_DECL(wrap_object_t_exec) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 1) {
    tk_object_t* obj = jsobj_get_native_ptr(call_info_p->this_value);
    if (obj != NULL) {
      const char* name = NULL;
      const char* args = NULL;
      str_t temp_name;
      str_t temp_args;

      str_init(&temp_name, 0);
      str_init(&temp_args, 0);

      name = jsvalue_to_utf8(args_p[0], &temp_name);
      if (args_count >= 2) {
        args = jsvalue_to_args_string(args_p[1], &temp_args);
      }

      ret = tk_object_exec(obj, name, args);

      str_reset(&temp_name);
      str_reset(&temp_args);
    }
  }

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_object_t_forward_event) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 2) {
    tk_object_t* obj = jsobj_get_native_ptr(call_info_p->this_value);
    if (obj != NULL) {
      tk_object_t* listener = jsvalue_to_obj(args_p[0]);
      uint32_t type = jsvalue_to_number(args_p[1]);
      emitter_on(EMITTER(obj), type, emitter_forward, listener);
      ret = RET_OK;
    }
  }

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_object_t_unforward_event) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 2) {
    tk_object_t* obj = jsobj_get_native_ptr(call_info_p->this_value);
    if (obj != NULL) {
      tk_object_t* listener = jsvalue_to_obj(args_p[0]);
      uint32_t type = jsvalue_to_number(args_p[1]);
      emitter_off_by_func(EMITTER(obj), type, emitter_forward, listener);
      ret = RET_OK;
    }
  }

  return jsvalue_from_number(ret);
}

jsvalue_t jsvalue_from_obj(tk_object_t* obj) {
  value_t v;
  return_value_if_fail(obj != NULL, JS_UNDEFINED);

  if (tk_object_get_prop(obj, JSOBJ_PROP_NATIVE_OBJ, &v) == RET_OK) {
    return jsvalue_ref(value_uint32(&v));
  } else {
    jsvalue_t jsobj = jerry_create_object();

    jsobj_set_native_ptr(jsobj, tk_object_ref(obj));
    jsobj_set_prop_func(jsobj, "getProp", wrap_object_t_get_prop);
    jsobj_set_prop_func(jsobj, "setProp", wrap_object_t_set_prop);
    jsobj_set_prop_func(jsobj, "canExec", wrap_object_t_can_exec);
    jsobj_set_prop_func(jsobj, "exec", wrap_object_t_exec);
    jsobj_set_prop_func(jsobj, "forwardEvent", wrap_object_t_forward_event);
    jsobj_set_prop_func(jsobj, "unforwardEvent", wrap_object_t_unforward_event);

    return jsobj;
  }
}

typedef struct _jsvalue_from_obj_ctx_t {
  str_t* str;
  jsvalue_t value;
} jsvalue_from_obj_ctx_t;

static ret_t visit_object_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  jsvalue_from_obj_ctx_t* info = (jsvalue_from_obj_ctx_t*)ctx;

  jsobj_set_prop(info->value, nv->name, &(nv->value), info->str);
  return RET_OK;
}

jsvalue_t jsvalue_from_obj_copy(tk_object_t* obj) {
  value_t v;
  return_value_if_fail(obj != NULL, JS_UNDEFINED);

  if (tk_object_get_prop(obj, JSOBJ_PROP_NATIVE_OBJ, &v) == RET_OK) {
    return jsvalue_ref(value_uint32(&v));
  } else {
    str_t str;
    jsvalue_from_obj_ctx_t info;
    jsvalue_t jsobj = jerry_create_object();

    info.str = &str;
    info.value = jsobj;
    str_init(&str, 0);
    tk_object_foreach_prop(obj, visit_object_prop, &info);
    str_reset(&str);

    return jsobj;
  }
}

tk_object_t* jsvalue_to_obj(jsvalue_t value) {
  tk_object_t* obj = jsobj_get_native_ptr(value);

  if (obj == NULL) {
    void* p = NULL;
    jerry_get_object_native_pointer(value, &p, NULL);

    if (p == NULL) {
      obj = object_js_factory_create_object(object_js_factory(), value, FALSE);
      jsobj_set_native_ptr(value, obj);
    }
  }

  return obj;
}

jsvalue_t jsvalue_from_value(const value_t* v, str_t* temp) {
  jsvalue_t value;

  if (v == NULL) {
    return JS_UNDEFINED;
  }

  if (v->type == VALUE_TYPE_STRING) {
    value = jsvalue_from_utf8(value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    if (temp != NULL && str_from_value(temp, v) == RET_OK) {
      value = jsvalue_from_utf8(temp->str);
    } else {
      value = jsvalue_from_utf8("");
    }
  } else if (v->type == VALUE_TYPE_BOOL) {
    value = jerry_create_boolean(value_bool(v));
  } else if (v->type == VALUE_TYPE_OBJECT) {
    return jsvalue_from_obj(value_object(v));
  } else if (v->type == VALUE_TYPE_POINTER) {
    value = jerry_create_object();
    jerry_set_object_native_pointer(value, (void*)value_pointer(v), NULL);
  } else if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    uint8_t* buffer = (uint8_t*)bin->data;
    uint32_t size = bin->size;

    jsvalue_t jbuffer = jerry_create_arraybuffer_external(size, buffer, NULL);
    value = jerry_create_typedarray_for_arraybuffer(JERRY_TYPEDARRAY_UINT8, jbuffer);
    jsvalue_unref(jbuffer);
  } else {
    value = jsvalue_from_number(value_double(v));
  }

  return value;
}

ret_t jsvalue_to_value(jsvalue_t value, value_t* v, str_t* temp) {
  ret_t ret = RET_NOT_IMPL;

  if (jerry_value_is_string(value)) {
    char* str = jsvalue_to_utf8(value, temp);
    if (str != NULL) {
      value_set_str(v, str);
      ret = RET_OK;
    } else {
      ret = RET_OOM;
    }
  } else if (jerry_value_is_number(value)) {
    double raw_value = jsvalue_to_number(value);
    value_set_double(v, raw_value);
    ret = RET_OK;
  } else if (jerry_value_is_boolean(value)) {
    bool_t raw_value = jsvalue_to_boolean(value);
    value_set_bool(v, raw_value);
    ret = RET_OK;
  } else if (jsvalue_is_object(value)) {
    tk_object_t* obj = jsvalue_to_obj(value);
    if (obj != NULL) {
      value_set_object(v, obj);
      ret = RET_OK;
    } else {
      void* p = NULL;
      if (jerry_get_object_native_pointer(value, &p, NULL)) {
        value_set_pointer(v, p);
        ret = RET_OK;
      } else {
        log_debug("not supported yet.\n");
      }
    }
  } else if (jerry_value_is_typedarray(value)) {
    jerry_length_t byte_offset = 0;
    jerry_length_t byte_length = 0;

    jerry_typedarray_type_t type = jerry_get_typedarray_type(value);
    jerry_length_t length = jerry_get_typedarray_length(value);
    jsvalue_t jbuffer = jerry_get_typedarray_buffer(value, &byte_offset, &byte_length);

    uint8_t* buff = jerry_get_arraybuffer_pointer(jbuffer);
    uint32_t size = jerry_get_arraybuffer_byte_length(jbuffer);
    jsvalue_t jdetachable = jerry_is_arraybuffer_detachable(jbuffer);
    if (jsvalue_to_boolean(jdetachable)) {
      jsvalue_t jdetach = jerry_detach_arraybuffer(jbuffer);
      if (jerry_value_is_null(jdetach)) {
        value_set_binary_data(v, buff, size);
      } else {
        value_dup_binary_data(v, buff, size);
      }
      jsvalue_unref(jdetach);
    } else {
      value_dup_binary_data(v, buff, size);
    }
    jsvalue_unref(jdetachable);
    jsvalue_unref(jbuffer);
  } else {
    log_debug("not supported yet.\n");
  }

  return ret;
}

ret_t jsobj_register_global(const char* name, tk_object_t* obj) {
  jsvalue_t global_obj;
  jsvalue_t jsobj;
  return_value_if_fail(name != NULL && obj != NULL, RET_BAD_PARAMS);

  jsobj = jsvalue_from_obj(obj);
  global_obj = jsvalue_get_global_object();
  jsobj_set_prop_value(global_obj, name, jsobj);
  jsvalue_unref(global_obj);
  jsvalue_unref(jsobj);

  return RET_OK;
}

ret_t jsobj_unregister_global(const char* name) {
  jsvalue_t global_obj;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  global_obj = jsvalue_get_global_object();
  jsobj_remove_prop(global_obj, name);
  jsvalue_unref(global_obj);

  js_gc();

  return RET_OK;
}

static void tkc_object_free_callback(void* native_p, struct jerry_object_native_info_t* info_p) {
  tk_object_t* obj = TK_OBJECT(native_p);
  tk_object_unref(obj);
}

static const jerry_object_native_info_t s_tkc_object_info = {.free_cb = tkc_object_free_callback};

ret_t jsobj_set_native_ptr(jsvalue_t obj, tk_object_t* ptr) {
  return_value_if_fail(ptr != NULL, RET_BAD_PARAMS);

  jerry_set_object_native_pointer(obj, (void*)ptr, &s_tkc_object_info);
  return RET_OK;
}

tk_object_t* jsobj_get_native_ptr(jsvalue_t obj) {
  void* p = NULL;
  if (jerry_get_object_native_pointer(obj, &p, &s_tkc_object_info)) {
    return TK_OBJECT(p);
  }
  return NULL;
}

ret_t jsobj_delete_native_ptr(jsvalue_t obj) {
  return jerry_delete_object_native_pointer(obj, &s_tkc_object_info) ? RET_OK : RET_FAIL;
}

jsvalue_t jsobj_get_keys(jsvalue_t obj) {
  return jerry_get_object_keys(obj);
}

uint32_t jsobj_get_prop_count(jsvalue_t obj) {
  if (jerry_value_is_array(obj)) {
    return jerry_get_array_length(obj);
  } else {
    uint32_t count = 0;
    jsvalue_t keys = jerry_get_object_keys(obj);

    if (jsvalue_check(keys) == RET_OK) {
      count = jerry_get_array_length(keys);
      jsvalue_unref(keys);
    }

    return count;
  }
}

jsvalue_t jsobj_get_prop_value_by_index(jsvalue_t obj, uint32_t index) {
  if (jerry_value_is_array(obj)) {
    return jerry_get_property_by_index(obj, index);
  } else {
    jsvalue_t prop_name;
    jsvalue_t prop_value;
    jsvalue_t keys = jerry_get_object_keys(obj);

    if (jsvalue_check(keys) != RET_OK) {
      prop_value = jerry_create_undefined();
    } else {
      prop_name = jerry_get_property_by_index(keys, index);

      if (jsvalue_check(prop_name) != RET_OK) {
        prop_value = jerry_create_undefined();
      } else {
        prop_value = jerry_get_property(obj, prop_name);
        jsvalue_unref(prop_name);
      }

      jsvalue_unref(keys);
    }

    return prop_value;
  }
}

ret_t jsobj_set_prop_value_by_index(jsvalue_t obj, uint32_t index, jsvalue_t prop_value) {
  ret_t ret = RET_FAIL;

  if (jerry_value_is_array(obj)) {
    ret = jsvalue_check(jerry_set_property_by_index(obj, index, prop_value));
  } else {
    jsvalue_t keys = jerry_get_object_keys(obj);

    if (jsvalue_check(keys) == RET_OK) {
      jsvalue_t prop_name = jerry_get_property_by_index(keys, index);

      if (jsvalue_check(prop_name) == RET_OK) {
        ret = jsvalue_check(jerry_set_property(obj, prop_name, prop_value));
        jsvalue_unref(prop_name);
      }

      jsvalue_unref(keys);
    }
  }

  return ret;
}

ret_t jsobj_get_prop_by_index(jsvalue_t obj, uint32_t index, value_t* v, str_t* temp) {
  ret_t ret = RET_FAIL;
  jsvalue_t value = jsobj_get_prop_value_by_index(obj, index);

  if (jsvalue_check(value) == RET_OK) {
    ret = jsvalue_to_value(value, v, temp);
    jsvalue_unref(value);
  }

  return ret;
}

ret_t jsobj_set_prop_by_index(jsvalue_t obj, uint32_t index, const value_t* v, str_t* temp) {
  jsvalue_t value = jsvalue_from_value(v, temp);
  ret_t ret = jsobj_set_prop_value_by_index(obj, index, value);
  jsvalue_unref(value);

  return ret;
}

ret_t jsobj_remove_prop_by_index(jsvalue_t obj, uint32_t index) {
  ret_t ret = RET_FAIL;

  if (jerry_value_is_array(obj)) {
    ret = jerry_delete_property_by_index(obj, index) ? RET_OK : RET_FAIL;
  } else {
    jsvalue_t keys = jerry_get_object_keys(obj);

    if (jsvalue_check(keys) == RET_OK) {
      jsvalue_t prop_name = jerry_get_property_by_index(keys, index);

      if (jsvalue_check(prop_name) == RET_OK) {
        ret = jerry_delete_property(obj, prop_name) ? RET_OK : RET_FAIL;
        jsvalue_unref(prop_name);
      }

      jsvalue_unref(keys);
    }
  }

  return ret;
}

jsvalue_t jsobj_get_prop_value(jsvalue_t obj, const char* name) {
  jsvalue_t prop_name = jsvalue_from_utf8(name);
  jsvalue_t prop_value = jerry_get_property(obj, prop_name);
  jsvalue_unref(prop_name);

  return prop_value;
}

ret_t jsobj_set_prop_value(jsvalue_t obj, const char* name, jsvalue_t prop_value) {
  jsvalue_t prop_name = jsvalue_from_utf8(name);
  jerry_set_property(obj, prop_name, prop_value);
  jsvalue_unref(prop_name);

  return RET_OK;
}

bool_t jsobj_has_prop(jsvalue_t obj, const char* name) {
  jsvalue_t prop_name = jsvalue_from_utf8(name);
  jsvalue_t has_prop_js = jerry_has_property(obj, prop_name);
  bool_t has_prop = (bool_t)jsvalue_to_boolean(has_prop_js);
  jsvalue_unref(has_prop_js);
  jsvalue_unref(prop_name);

  return has_prop;
}

bool_t jsobj_has_prop_func(jsvalue_t obj, const char* name) {
  jsvalue_t value = jsobj_get_prop_value(obj, name);
  bool_t ret = jsvalue_is_function(value);
  jsvalue_unref(value);

  return ret;
}

ret_t jsobj_get_prop(jsvalue_t obj, const char* name, value_t* v, str_t* temp) {
  ret_t ret = RET_FAIL;
  jsvalue_t value = jsobj_get_prop_value(obj, name);

  if (jsvalue_check(value) == RET_OK) {
    if (jerry_value_is_undefined(value)) {
      ret = RET_NOT_FOUND;
    } else {
      ret = jsvalue_to_value(value, v, temp);
    }

    jsvalue_unref(value);
  }

  return ret;
}

ret_t jsobj_set_prop(jsvalue_t obj, const char* name, const value_t* v, str_t* temp) {
  jsvalue_t value = jsvalue_from_value(v, temp);
  ret_t ret = jsobj_set_prop_value(obj, name, value);
  jsvalue_unref(value);

  return ret;
}

ret_t jsobj_set_prop_int(jsvalue_t obj, const char* name, int v) {
  jsvalue_t value = jsvalue_from_number(v);
  ret_t ret = jsobj_set_prop_value(obj, name, value);
  jsvalue_unref(value);

  return ret;
}

ret_t jsobj_set_prop_str(jsvalue_t obj, const char* name, const char* v) {
  jsvalue_t value = jsvalue_from_utf8(v);
  ret_t ret = jsobj_set_prop_value(obj, name, value);
  jsvalue_unref(value);

  return ret;
}

ret_t jsobj_set_prop_func(jsvalue_t obj, const char* name, jsfunc_t handler_p) {
  jsvalue_t func = jerry_create_external_function(handler_p);
  ret_t ret = jsobj_set_prop_value(obj, name, func);
  jsvalue_unref(func);

  return ret;
}

ret_t jsobj_remove_prop(jsvalue_t obj, const char* name) {
  jsvalue_t prop_name = jsvalue_from_utf8(name);
  ret_t ret = jerry_delete_property(obj, prop_name) ? RET_OK : RET_FAIL;
  jsvalue_unref(prop_name);

  return ret;
}

typedef struct _jerry_object_foreach_data_t {
  tk_visit_t on_prop;
  void* ctx;
  str_t* temp_name;
  str_t* temp_value;
} jerry_object_foreach_data_t;

static bool jerry_object_prop_visit(const jsvalue_t name, const jsvalue_t val, void* data) {
  jerry_object_foreach_data_t* d = (jerry_object_foreach_data_t*)data;
  named_value_t prop;

  prop.name = jsvalue_to_utf8(name, d->temp_name);
  if (jsvalue_to_value(val, &(prop.value), d->temp_value) == RET_OK) {
    if (d->on_prop(d->ctx, &prop) == RET_OK) {
      return true;
    }
  }

  return false;
}

ret_t jsobj_foreach_prop(jsvalue_t obj, tk_visit_t on_prop, void* ctx, str_t* temp_name,
                         str_t* temp_value) {
  ret_t ret = RET_FAIL;
  jerry_object_foreach_data_t data;
  return_value_if_fail(on_prop != NULL && temp_name != NULL && temp_value != NULL, RET_BAD_PARAMS);

  data.on_prop = on_prop;
  data.ctx = ctx;
  data.temp_name = temp_name;
  data.temp_value = temp_value;
  if (jerry_foreach_object_property(obj, jerry_object_prop_visit, &data)) {
    ret = RET_OK;
  }

  return ret;
}

ret_t jsobj_exec_ex(jsvalue_t obj, const char* name, jsvalue_t jsargs) {
  ret_t ret = RET_FAIL;

  if (jsobj_has_prop(obj, name)) {
    jsvalue_t func = jsobj_get_prop_value(obj, name);
    if (jsvalue_is_function(func)) {
      jsvalue_t jsret = jerry_call_function(func, obj, &jsargs, 1);
      jerry_value_check(jsret);
      ret = (ret_t)jsvalue_to_number(jsret);
      jsvalue_unref(func);
      jsvalue_unref(jsret);
    } else {
      ret = RET_NOT_IMPL;
      log_debug("not function %s\n", name);
    }
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

ret_t jsobj_exec(jsvalue_t obj, const char* name, const value_t* args, str_t* temp) {
  jsvalue_t jsargs = jsvalue_from_value(args, temp);
  ret_t ret = jsobj_exec_ex(obj, name, jsargs);
  jsvalue_unref(jsargs);

  return ret;
}

bool_t jsobj_can_exec(jsvalue_t obj, const char* name, const value_t* args, str_t* temp) {
  bool_t ret = FALSE;
  return_value_if_fail(name != NULL && name[0] != '\0' && temp != NULL, FALSE);

  str_set(temp, "can");
  str_append_char(temp, islower(name[0]) ? toupper(name[0]) : name[0]);
  str_append(temp, &name[1]);

  if (jsobj_has_prop(obj, temp->str)) {
    jsvalue_t func = jsobj_get_prop_value(obj, temp->str);

    if (jsvalue_is_function(func)) {
      jsvalue_t jsargs = jsvalue_from_value(args, temp);
      jsvalue_t jsret = jerry_call_function(func, obj, &jsargs, 1);
      ret = jsvalue_to_boolean(jsret);
      jsvalue_unref(jsret);
      jsvalue_unref(jsargs);
    } else {
      ret = FALSE;
    }

    jsvalue_unref(func);
  } else {
    ret = jsobj_has_prop_func(obj, name);
  }

  return ret;
}
