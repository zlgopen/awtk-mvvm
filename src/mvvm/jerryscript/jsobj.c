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

jerry_value_t jerry_create_str(const char* str) {
  return str != NULL ? jerry_create_string((const jerry_char_t*)str) : jerry_create_null();
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
      } else if (jerry_value_is_typedarray(value)) {
        jerry_length_t byte_offset = 0;
        jerry_length_t byte_length = 0;

        jerry_typedarray_type_t type = jerry_get_typedarray_type(value);
        jerry_length_t length = jerry_get_typedarray_length(value);
        jerry_value_t jbuffer = jerry_get_typedarray_buffer(value, &byte_offset, &byte_length);

        uint8_t* buff = jerry_get_arraybuffer_pointer(jbuffer);
        uint32_t size = jerry_get_arraybuffer_byte_length(jbuffer);
        jerry_value_t jdetachable = jerry_is_arraybuffer_detachable(jbuffer);
        if (jerry_value_to_boolean(jdetachable)) {
          jerry_value_t jdetach = jerry_detach_arraybuffer(jbuffer);
          if (jerry_value_is_null(jdetach)) {
            value_set_binary_data(v, buff, size);
          } else {
            value_dup_binary_data(v, buff, size);
          }
          jerry_release_value(jdetach);
        } else {
          value_dup_binary_data(v, buff, size);
        }
        jerry_release_value(jdetachable);
        jerry_release_value(jbuffer);
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
  } else if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    uint8_t* buffer = (uint8_t*)bin->data;
    uint32_t size = bin->size;

    jerry_value_t jbuffer = jerry_create_arraybuffer_external(size, buffer, NULL);
    value = jerry_create_typedarray_for_arraybuffer(JERRY_TYPEDARRAY_UINT8, jbuffer);
    jerry_release_value(jbuffer);

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

ret_t jerry_value_from_obj(jerry_value_t value, object_t* obj) {
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

typedef struct _jerry_value_to_obj_ctx_t {
  str_t* name_str;
  str_t* value_str;
  object_t* obj;
} jerry_value_to_obj_ctx_t;

static bool visit_jerry_object_prop(const jerry_value_t property_name, const jerry_value_t property_value, void* user_data_p) {
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

ret_t jerry_value_to_obj(jerry_value_t value, object_t* obj) {
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
