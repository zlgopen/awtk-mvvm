/**
 * File:   object_js_array.c
 * Author: AWTK Develop Team
 * Brief:  wrap jerry script array object to an tkc object.
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/value.h"
#include "tkc/types_def.h"
#include "mvvm/jerryscript/object_js_array.h"

#define JS_ARRAY_FUNC_SORT_COMPARE "sortCompare"
#define JS_ARRAY_PROP_SORT_ASCENDING "sortAscending"
#define JS_ARRAY_PROP_SORT_IGNORE_CASE "sortIgnoreCase"

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

static ret_t object_js_array_get(tk_object_t* obj, uint32_t i, value_t* v) {
  uint32_t len = 0;
  ret_t ret = RET_NOT_FOUND;
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  len = jerry_get_array_length(o->jsobj);
  if (i < len) {
    ret = jsobj_get_prop_by_index(o->jsobj, i, v, &(o->temp));
    if (ret == RET_OK && v->type == VALUE_TYPE_OBJECT) {
      tk_object_t* val = value_object(v);
      if (!object_js_base_is_listener_registered(val, obj)) {
        object_js_base_register_listener(val, obj);
      }
    }
  }

  return ret;
}

static ret_t object_js_array_exec_splice(tk_object_t* obj, const value_t* args_p, uint32_t args_c,
                                         value_t* result) {
  const char* func_name = "splice";
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return jsobj_exec_value(o->jsobj, func_name, args_p, args_c, result, &(o->temp));
}

static ret_t object_js_array_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  object_js_array_t* array_o = OBJECT_JS_ARRAY(obj);
  return_value_if_fail(o != NULL && array_o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, FSCRIPT_ARRAY_PROP_VTABEL_NAME)) {
    value_set_pointer(v, array_o->vt);
    return RET_OK;
  } else if (tk_str_eq(name, TK_OBJECT_ARRAY_PROP_LENGTH) ||
             tk_str_eq(name, TK_OBJECT_ARRAY_PROP_SIZE) || tk_str_eq(name, TK_OBJECT_PROP_SIZE)) {
    value_set_int(v, jerry_get_array_length(o->jsobj));
    return RET_OK;
  } else if (name[0] == '[') {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_get_prop(sub, name, v);
    }

    uint32_t index = tk_atoi(name + 1);
    return object_js_array_get(obj, index, v);
  }

  return object_js_base_get_prop(obj, name, v);
}

static ret_t object_js_array_remove_prop(tk_object_t* obj, const char* name) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (name[0] == '[') {
    value_t result;
    value_t args_p[2];
    uint32_t index = tk_atoi(name + 1);
    value_set_int(args_p, index);
    value_set_int(args_p + 1, 1);
    /* jsobj_remove_prop_by_index接口只会删除对应index的元素，其他元素的index和数组size不会变化 */
    /* 因此此处采用JS数组原生的splice函数实现 */
    return object_js_array_exec_splice(obj, args_p, 2, &result);
  }

  return object_js_base_remove_prop(obj, name);
}

static ret_t object_js_array_fscript_array_dup(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  log_debug("object_js_array_fscript_array_dup is not impl!\n");
  return RET_NOT_IMPL;
}

static ret_t object_js_array_fscript_array_push(fscript_t* fscript, fscript_args_t* args,
                                                value_t* v) {
  uint32_t i = 0;
  uint32_t size = 0;
  uint32_t index = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  index = size;
  for (i = 1; i < args->size; i++) {
    ret = jsobj_set_prop_by_index(o->jsobj, index, args->args + i, &(o->temp));
    if (ret != RET_OK) {
      break;
    }
    index++;
  }

  value_set_uint32(v, i - 1);

  return ret;
}

static ret_t object_js_array_fscript_array_pop(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  if (size > 0) {
    ret = jsobj_exec_value(o->jsobj, "pop", NULL, 0, v, &(o->temp));
  }

  if (ret != RET_OK) {
    v->type = VALUE_TYPE_INVALID;
  }

  return ret;
}

static ret_t object_js_array_fscript_array_shift(fscript_t* fscript, fscript_args_t* args,
                                                 value_t* v) {
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  if (size > 0) {
    ret = jsobj_exec_value(o->jsobj, "shift", NULL, 0, v, &(o->temp));
  }

  if (ret != RET_OK) {
    v->type = VALUE_TYPE_INVALID;
  }

  return ret;
}

static ret_t object_js_array_fscript_array_get(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  int32_t index = 0;
  uint32_t size = 0;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  index = value_int(args->args + 1);
  index = index < 0 ? index + size : index;
  return_value_if_fail(index >= 0 && index < size, RET_BAD_PARAMS);

  return object_js_array_get(obj, index, v);
}

static ret_t object_js_array_fscript_array_set(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  int32_t index = 0;
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  index = value_int(args->args + 1);
  index = index < 0 ? index + size : index;
  return_value_if_fail(index >= 0 && index < size, RET_BAD_PARAMS);

  ret = jsobj_set_prop_by_index(o->jsobj, index, args->args + 2, &(o->temp));
  value_set_bool(v, ret == RET_OK);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_insert(fscript_t* fscript, fscript_args_t* args,
                                                  value_t* v) {
  int32_t index = 0;
  uint32_t size = 0;
  value_t args_p[3];
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  index = value_int(args->args + 1);
  index = index < 0 ? index + size : index;
  return_value_if_fail(index >= 0, RET_BAD_PARAMS);

  value_set_int(args_p, index);
  value_set_int(args_p + 1, 0);
  value_copy(args_p + 2, args->args + 2);

  value_set_bool(v, object_js_array_exec_splice(obj, args_p, 3, v) == RET_OK);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_remove(fscript_t* fscript, fscript_args_t* args,
                                                  value_t* v) {
  int32_t index = 0;
  uint32_t size = 0;
  value_t args_p[2];
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  index = value_int(args->args + 1);
  index = index < 0 ? index + size : index;
  return_value_if_fail(index >= 0 && index < size, RET_BAD_PARAMS);

  value_set_int(args_p, index);
  value_set_int(args_p + 1, 1);

  value_set_bool(v, object_js_array_exec_splice(obj, args_p, 2, v) == RET_OK);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_get_and_remove(fscript_t* fscript, fscript_args_t* args,
                                                          value_t* v) {
  int32_t index = 0;
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  index = value_int(args->args + 1);
  index = index < 0 ? index + size : index;
  return_value_if_fail(index >= 0 && index < size, RET_BAD_PARAMS);

  ret = object_js_array_get(obj, index, v);
  if (ret == RET_OK) {
    value_t result;
    value_t args_p[2];
    value_set_int(args_p, index);
    value_set_int(args_p + 1, 1);
    ret = object_js_array_exec_splice(obj, args_p, 2, &result);
  }

  return ret;
}

static ret_t object_js_array_fscript_array_index_of(fscript_t* fscript, fscript_args_t* args,
                                                    value_t* v) {
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  if (size > 0) {
    ret = jsobj_exec_value(o->jsobj, "indexOf", args->args + 1, 1, v, &(o->temp));
  }

  return ret;
}

static ret_t object_js_array_fscript_array_last_index_of(fscript_t* fscript, fscript_args_t* args,
                                                         value_t* v) {
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  if (size > 0) {
    ret = jsobj_exec_value(o->jsobj, "lastIndexOf", args->args + 1, 1, v, &(o->temp));
  }

  return ret;
}

static ret_t object_js_array_fscript_array_clear(fscript_t* fscript, fscript_args_t* args,
                                                 value_t* v) {
  value_t args_p;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  value_set_int(&args_p, 0);
  value_set_bool(v, object_js_array_exec_splice(obj, &args_p, 1, v) == RET_OK);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_reverse(fscript_t* fscript, fscript_args_t* args,
                                                   value_t* v) {
  uint32_t size = 0;
  ret_t ret = RET_OK;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  if (size > 1) {
    ret = jsobj_exec_value(o->jsobj, "reverse", NULL, 0, v, &(o->temp));
  }

  value_set_bool(v, ret == RET_OK);

  return ret;
}

static ret_t object_js_array_fscript_array_join(fscript_t* fscript, fscript_args_t* args,
                                                value_t* v) {
  value_t result;
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  ret = jsobj_exec_value(o->jsobj, "join", args->args + 1, 1, &result, &(o->temp));
  if (ret == RET_OK) {
    value_dup_str(v, value_str(&result));
  }

  return ret;
}

static JSFUNC_DECL(object_js_array_sort_compare) {
  value_t a;
  value_t b;
  str_t temp;
  str_t temp_ex;
  double ret = 0;
  value_t ascending;
  value_t ignore_case;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  str_init(&temp, 0);
  str_init(&temp_ex, 0);
  value_set_bool(&ascending, TRUE);
  value_set_bool(&ignore_case, FALSE);
  obj = jsobj_get_native_ptr(call_info_p->function);
  o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, JS_UNDEFINED);

  jsobj_get_prop(o->jsobj, JS_ARRAY_PROP_SORT_ASCENDING, &ascending, &temp);
  jsobj_get_prop(o->jsobj, JS_ARRAY_PROP_SORT_IGNORE_CASE, &ignore_case, &temp);
  jsvalue_to_value(args_p[0], &a, &temp);
  jsvalue_to_value(args_p[1], &b, &temp_ex);

  if (a.type == VALUE_TYPE_STRING) {
    if (value_bool(&ignore_case)) {
      ret = tk_str_icmp(value_str(&a), value_str(&b));
      ret = value_bool(&ascending) ? ret : -ret;
    } else {
      ret = tk_str_cmp(value_str(&a), value_str(&b));
      ret = value_bool(&ascending) ? ret : -ret;
    }
  } else {
    ret = value_double(&a) - value_double(&b);
    ret = value_bool(&ascending) ? ret : -ret;
  }

  str_reset(&temp);
  str_reset(&temp_ex);

  return jsvalue_from_number(ret);
}

static ret_t object_js_array_fscript_array_sort(fscript_t* fscript, fscript_args_t* args,
                                                value_t* v) {
  uint32_t size = 0;
  value_t ascending;
  value_t ignore_case;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  size = jerry_get_array_length(o->jsobj);
  value_set_bool(&ascending, args->size > 1 ? value_bool(args->args + 1) : TRUE);
  value_set_bool(&ignore_case, args->size > 2 ? value_bool(args->args + 2) : FALSE);

  if (size > 1) {
    jsvalue_t jsret;
    jsvalue_t call_func;

    if (!jsobj_has_prop_func(o->jsobj, JS_ARRAY_FUNC_SORT_COMPARE)) {
      jsobj_set_prop_func(o->jsobj, JS_ARRAY_FUNC_SORT_COMPARE, object_js_array_sort_compare);
    }

    jsobj_set_prop(o->jsobj, JS_ARRAY_PROP_SORT_ASCENDING, &ascending, &(o->temp));
    jsobj_set_prop(o->jsobj, JS_ARRAY_PROP_SORT_IGNORE_CASE, &ignore_case, &(o->temp));

    call_func = jsobj_get_prop_value(o->jsobj, JS_ARRAY_FUNC_SORT_COMPARE);
    if (jsvalue_is_function(call_func)) {
      jsobj_set_native_ptr(call_func, obj);
      jsret = jsobj_exec_ex_value(o->jsobj, "sort", &call_func, 1);
      jsvalue_unref(jsret);
    }
    jsvalue_unref(call_func);
  }

  return RET_OK;
}

static ret_t object_js_array_fscript_array_clone_and_sort(fscript_t* fscript, fscript_args_t* args,
                                                          value_t* v) {
  log_debug("object_js_array_fscript_array_clone_and_sort is not impl!\n");
  return RET_NOT_IMPL;
}

static ret_t object_js_array_fscript_array_min(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  value_t item;
  int32_t i = 0;
  double value = 0;
  uint32_t size = 0;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);
  size = jerry_get_array_length(o->jsobj);
  return_value_if_fail(size > 0, RET_BAD_PARAMS);

  object_js_array_get(obj, 0, &item);
  value = value_double(&item);
  for (i = 1; i < size; i++) {
    object_js_array_get(obj, i, &item);
    double t = value_double(&item);
    if (value > t) {
      value = t;
    }
  }

  value_set_double(v, value);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_max(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  value_t item;
  int32_t i = 0;
  double value = 0;
  uint32_t size = 0;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);
  size = jerry_get_array_length(o->jsobj);
  return_value_if_fail(size > 0, RET_BAD_PARAMS);

  object_js_array_get(obj, 0, &item);
  value = value_double(&item);
  for (i = 1; i < size; i++) {
    object_js_array_get(obj, i, &item);
    double t = value_double(&item);
    if (value < t) {
      value = t;
    }
  }

  value_set_double(v, value);

  return RET_OK;
}

static double object_js_array_sum(tk_object_t* obj) {
  int32_t i = 0;
  double ret = 0;
  uint32_t size = 0;
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  size = jerry_get_array_length(o->jsobj);

  for (i = 0; i < size; i++) {
    value_t item;
    object_js_array_get(obj, i, &item);
    ret += value_double(&item);
  }

  return ret;
}

static ret_t object_js_array_fscript_array_avg(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  uint32_t size = 0;
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);
  size = jerry_get_array_length(o->jsobj);

  value_set_double(v, size > 0 ? object_js_array_sum(obj) / size : 0);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_sum(fscript_t* fscript, fscript_args_t* args,
                                               value_t* v) {
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  value_set_double(v, object_js_array_sum(obj));

  return RET_OK;
}

static ret_t object_js_array_fscript_array_is_empty(fscript_t* fscript, fscript_args_t* args,
                                                    value_t* v) {
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  value_set_bool(v, jerry_get_array_length(o->jsobj) == 0);

  return RET_OK;
}

static ret_t object_js_array_fscript_array_size(fscript_t* fscript, fscript_args_t* args,
                                                value_t* v) {
  tk_object_t* obj = NULL;
  object_js_base_t* o = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  o = OBJECT_JS_BASE(obj);
  FSCRIPT_FUNC_CHECK(o != NULL, RET_BAD_PARAMS);

  value_set_uint32(v, jerry_get_array_length(o->jsobj));

  return RET_OK;
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

static const fscript_array_vtable_t s_js_fscript_array_vtable = {
    .type = FSCRIPT_ARRAY_PROP_VTABEL_TYPE_STRING,
    .array_dup = object_js_array_fscript_array_dup,
    .array_push = object_js_array_fscript_array_push,
    .array_pop = object_js_array_fscript_array_pop,
    .array_shift = object_js_array_fscript_array_shift,
    .array_get = object_js_array_fscript_array_get,
    .array_set = object_js_array_fscript_array_set,
    .array_insert = object_js_array_fscript_array_insert,
    .array_remove = object_js_array_fscript_array_remove,
    .array_get_and_remove = object_js_array_fscript_array_get_and_remove,
    .array_index_of = object_js_array_fscript_array_index_of,
    .array_last_index_of = object_js_array_fscript_array_last_index_of,
    .array_clear = object_js_array_fscript_array_clear,
    .array_reverse = object_js_array_fscript_array_reverse,
    .array_join = object_js_array_fscript_array_join,
    .array_sort = object_js_array_fscript_array_sort,
    .array_clone_and_sort = object_js_array_fscript_array_clone_and_sort,
    .array_min = object_js_array_fscript_array_min,
    .array_max = object_js_array_fscript_array_max,
    .array_avg = object_js_array_fscript_array_avg,
    .array_sum = object_js_array_fscript_array_sum,
    .array_is_empty = object_js_array_fscript_array_is_empty,
    .array_size = object_js_array_fscript_array_size};

tk_object_t* object_js_array_create(jsvalue_t jsobj, bool_t free_handle) {
  tk_object_t* obj = tk_object_create(&s_object_js_array_vtable);
  object_js_array_t* o = OBJECT_JS_ARRAY(obj);
  return_value_if_fail(o != NULL, NULL);

  object_js_base_init(obj, jsobj, free_handle);
  o->vt = &s_js_fscript_array_vtable;

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
