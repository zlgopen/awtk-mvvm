/**
 * File:   object_js_base.c
 * Author: AWTK Develop Team
 * Brief:  object_js_base
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
#include "mvvm/base/utils.h"
#include "mvvm/jerryscript/object_js_base.h"
#include "mvvm/jerryscript/object_js_default.h"

ret_t object_js_base_init(object_t* obj, jsvalue_t jsobj, bool_t free_handle) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->jsobj = jsobj;
  o->free_handle = free_handle;
  str_init(&(o->temp), 0);

  return RET_OK;
}

ret_t object_js_base_deinit(object_t* obj) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  str_reset(&(o->temp));

  if (o->free_handle) {
    jsvalue_unref(o->jsobj);
  }

  return RET_OK;
}

int32_t object_js_base_compare(object_t* obj, object_t* other) {
  if (obj == other) {
    return 0;
  } else {
    jsvalue_t jsobj1 = (jsvalue_t)object_get_prop_uint32(obj, JSOBJ_PROP_NATIVE_OBJ, 0);
    jsvalue_t jsobj2 = (jsvalue_t)object_get_prop_uint32(other, JSOBJ_PROP_NATIVE_OBJ, 0);
    if (jsobj1 && jsobj2) {
      return jsvalue_compare(jsobj1, jsobj2);
    }

    return -1;
  }
}

ret_t object_js_base_set_prop(object_t* obj, const char* name, const value_t* v) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, JSOBJ_PROP_NATIVE_OBJ)) {
    return RET_BAD_PARAMS;
  } else {
    if (strstr(name, ".") != NULL) {
      return RET_NOT_FOUND;
    }
    return jsobj_set_prop(o->jsobj, name, v, &(o->temp));
  }

  return RET_OK;
}

ret_t object_js_base_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, JSOBJ_PROP_NATIVE_OBJ)) {
    value_set_uint32(v, o->jsobj);
  } else {
    ret = jsobj_get_prop(o->jsobj, name, v, &(o->temp));
    if (ret == RET_OK && v->type == VALUE_TYPE_OBJECT) {
      object_t* val = value_object(v);
      if (!object_js_base_is_listener_registered(val, obj)) {
        object_js_base_register_listener(val, obj);
      }
    }
  }

  return ret;
}

ret_t object_js_base_remove_prop(object_t* obj, const char* name) {
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, JSOBJ_PROP_NATIVE_OBJ)) {
    return RET_BAD_PARAMS;
  } else {
    return jsobj_remove_prop(o->jsobj, name);
  }
}

ret_t object_js_base_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_FAIL;
  str_t str;
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  str_init(&str, 0);
  ret = jsobj_foreach_prop(o->jsobj, on_prop, ctx, &str, &(o->temp));
  str_reset(&str);

  return ret;
}

bool_t object_js_base_can_exec(object_t* obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  value_t v;
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, FALSE);

  if (args == NULL || !tk_str_start_with(args, COMMAND_ARGS_STRING_PREFIX)) {
    value_set_str(&v, args);
    ret = jsobj_can_exec(o->jsobj, name, &v, &(o->temp));
  } else {
    jsvalue_t jsargs = JS_EMPTY_OBJ;
    object_t* a = object_js_default_create(jsargs, TRUE);

    if (a != NULL) {
      if (tk_command_arguments_to_object(args, a) == RET_OK) {
        value_set_object(&v, a);
        ret = jsobj_can_exec(o->jsobj, name, &v, &(o->temp));
      }

      OBJECT_UNREF(a);
    }
  }

  return ret;
}

ret_t object_js_base_exec(object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_FAIL;
  value_t v;
  object_js_base_t* o = OBJECT_JS_BASE(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (args == NULL || !tk_str_start_with(args, COMMAND_ARGS_STRING_PREFIX)) {
    value_set_str(&v, args);
    ret = jsobj_exec(o->jsobj, name, &v, &(o->temp));
  } else {
    jsvalue_t jsargs = JS_EMPTY_OBJ;
    object_t* a = object_js_default_create(jsargs, TRUE);

    if (a != NULL) {
      if (tk_command_arguments_to_object(args, a) == RET_OK) {
        value_set_object(&v, a);
        ret = jsobj_exec(o->jsobj, name, &v, &(o->temp));
      }

      OBJECT_UNREF(a);
    }
  }

  return ret;
}

bool_t object_js_base_is_listener_registered(object_t* obj, object_t* listener) {
  return emitter_exist(EMITTER(obj), EVT_PROP_CHANGED, (event_func_t)emitter_dispatch, listener);
}

static ret_t object_js_base_on_unregister_listener(void* ctx, event_t* e) {
  object_t* obj = OBJECT(ctx);
  object_t* target = OBJECT(e->target);

  emitter_off_by_ctx(EMITTER(obj), target);

  return RET_OK;
}

ret_t object_js_base_register_listener(object_t* obj, object_t* listener) {
  return_value_if_fail(obj != NULL && listener != NULL, RET_BAD_PARAMS);

  emitter_on(EMITTER(listener), EVT_DESTROY, object_js_base_on_unregister_listener, obj);
  emitter_on(EMITTER(obj), EVT_DESTROY, object_js_base_on_unregister_listener, listener);
  emitter_on(EMITTER(obj), EVT_ITEMS_CHANGED, (event_func_t)emitter_dispatch, listener);
  emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, (event_func_t)emitter_dispatch, listener);
  emitter_on(EMITTER(obj), EVT_PROP_CHANGED, (event_func_t)emitter_dispatch, listener);

  return RET_OK;
}

bool_t object_is_object_js(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, FALSE);
  return tk_str_start_with(obj->vt->type, "object_js_");
}
