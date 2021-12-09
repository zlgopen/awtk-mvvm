/**
 * File:   object_js_array.c
 * Author: AWTK Develop Team
 * Brief:  wrap jerry script array object to an tkc object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-31 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/value.h"
#include "mvvm/jerryscript/object_js_array.h"

static ret_t object_js_array_on_destroy(tk_object_t* obj) {
  object_js_array_t* o = OBJECT_JS_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return object_js_base_deinit(obj);
}

static int32_t object_array_parse_index(const char* name) {
  if (isdigit(*name)) {
    return tk_atoi(name);
  } else if (*name == '[') {
    return tk_atoi(name + 1);
  } else {
    return -1;
  }
}

static ret_t object_js_array_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
  if (sub != NULL) {
    return tk_object_set_prop(sub, name, v);
  }

  if (name[0] == '[') {
    uint32_t index = tk_atoi(name + 1);
    return jsobj_set_prop_by_index(o->jsobj, index, v, &(o->temp));
  }

  return object_js_base_set_prop(obj, name, v);
}

static ret_t object_js_array_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "length") || tk_str_eq(name, "size") ||
      tk_str_eq(name, TK_OBJECT_PROP_SIZE)) {
    value_set_int(v, jerry_get_array_length(o->jsobj));
    return RET_OK;
  } else if (name[0] == '[') {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_get_prop(sub, name, v);
    }

    uint32_t index = tk_atoi(name + 1);
    uint32_t len = jerry_get_array_length(o->jsobj);
    if (index >= len) {
      return RET_NOT_FOUND;
    } else {
      ret_t ret = jsobj_get_prop_by_index(o->jsobj, index, v, &(o->temp));
      if (ret == RET_OK && v->type == VALUE_TYPE_OBJECT) {
        tk_object_t* val = value_object(v);
        if (!object_js_base_is_listener_registered(val, obj)) {
          object_js_base_register_listener(val, obj);
        }
      }
      return ret;
    }
  }

  return object_js_base_get_prop(obj, name, v);
}

static ret_t object_js_array_remove_prop(tk_object_t* obj, const char* name) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (name[0] == '[') {
    uint32_t index = tk_atoi(name + 1);
    return jsobj_remove_prop_by_index(o->jsobj, index);
  }

  return object_js_base_remove_prop(obj, name);
}

static const object_vtable_t s_object_js_array_vtable = {
    .type = "object_js_array",
    .desc = "object_js_array",
    .size = sizeof(object_js_array_t),
    .is_collection = TRUE,
    .on_destroy = object_js_array_on_destroy,
    .compare = object_js_base_compare,
    .get_prop = object_js_array_get_prop,
    .set_prop = object_js_array_set_prop,
    .remove_prop = object_js_array_remove_prop,
    .foreach_prop = object_js_base_foreach_prop,
    .can_exec = object_js_base_can_exec,
    .exec = object_js_base_exec};

tk_object_t* object_js_array_create(jsvalue_t jsobj, bool_t free_handle) {
  tk_object_t* obj = tk_object_create(&s_object_js_array_vtable);
  object_js_array_t* o = OBJECT_JS_ARRAY(obj);
  return_value_if_fail(o != NULL, NULL);

  object_js_base_init(obj, jsobj, free_handle);

  return obj;
}

bool_t object_is_object_js_array(tk_object_t* obj) {
  return_value_if_fail(obj != NULL, FALSE);
  return obj->vt == &s_object_js_array_vtable;
}

object_js_array_t* object_js_array_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_js_array_vtable, NULL);

  return (object_js_array_t*)(obj);
}
