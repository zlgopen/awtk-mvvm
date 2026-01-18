/*
 * File:   jsobj.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript wrapper
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "mvvm/jerryscript/object_js_factory.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"

#ifndef WITH_JERRYSCRIPT_SNAPSHOT
#include "mvvm/jerryscript/mvvm_factory.res"
#else
#include "mvvm/jerryscript/mvvm_factory.snapshot"
#endif

ret_t jerry_script_register_mvvm_factory(void) {
#ifndef WITH_JERRYSCRIPT_SNAPSHOT
  return jerry_script_eval_buff(script_mvvm_factory, sizeof(script_mvvm_factory), "mvvm_factory.js",
                                TRUE);
#else
  return jerry_script_eval_snapshot(script_mvvm_factory, sizeof(script_mvvm_factory));
#endif
}

static jsvalue_t jsvalue_get_model_from_mvvm_factory(const char* model_name,
                                                     const char* func_name) {
  jsvalue_t model;
  jsvalue_t factory = jsvalue_get_global("MvvmFactory");

  if (jsvalue_check(factory) != RET_OK) {
    model = JS_UNDEFINED;
  } else {
    jsvalue_t func = jsobj_get_prop_value(factory, func_name);
    if (!jsvalue_is_function(func)) {
      model = JS_UNDEFINED;
    } else {
      jsvalue_t global_obj = jerry_get_global_object();
      jsvalue_t jsargs = jsvalue_from_utf8(model_name);

      model = jerry_call_function(func, global_obj, &jsargs, 1);
      if (jsvalue_check(model) != RET_OK) {
        model = JS_UNDEFINED;
      }

      jsvalue_unref(jsargs);
      jsvalue_unref(global_obj);
    }
    jsvalue_unref(func);
    jsvalue_unref(factory);
  }

  return model;
}

jsvalue_t jsvalue_get_view_model(const char* name) {
  return jsvalue_get_model_from_mvvm_factory(name, "createViewModel");
}

static void view_model_free_callback(void* native_p, struct jerry_object_native_info_t *info_p) {
}

static const jerry_object_native_info_t s_view_model_info = {.free_cb = view_model_free_callback};

ret_t js_view_model_set_native_ptr(jsvalue_t obj, void* ptr) {
  return_value_if_fail(ptr != NULL, RET_BAD_PARAMS);

  jerry_set_object_native_pointer(obj, ptr, &s_view_model_info);
  return RET_OK;
}

void* js_view_model_get_native_ptr(jsvalue_t obj) {
  void* p = NULL;
  if (jerry_get_object_native_pointer(obj, &p, &s_view_model_info)) {
    return VIEW_MODEL(p);
  }
  return NULL;
}

ret_t js_view_model_delete_native_ptr(jsvalue_t obj) {
  return jerry_delete_object_native_pointer(obj, &s_view_model_info) ? RET_OK : RET_FAIL;
}

jsvalue_t jsvalue_get_value_converter(const char* name) {
  return jsvalue_get_model_from_mvvm_factory(name, "getValueConverter");
}

ret_t js_value_converter_to_view(jsvalue_t converter, const value_t* from, value_t* to,
                                 str_t* temp) {
  ret_t ret = RET_OK;
  jsvalue_t func = jsobj_get_prop_value(converter, "toView");

  if (jsvalue_is_function(func)) {
    jsvalue_t jsfrom = jsvalue_from_value(from, temp);
    jsvalue_t jsret = jsfunc_call(func, converter, &jsfrom, 1);
    ret = jsvalue_to_value(jsret, to, temp);
    jsvalue_unref(jsfrom);
    jsvalue_unref(jsret);
  }
  jsvalue_unref(func);

  return ret;
}

ret_t js_value_converter_to_model(jsvalue_t converter, const value_t* from, value_t* to,
                                  str_t* temp) {
  ret_t ret = RET_OK;
  jsvalue_t func = jsobj_get_prop_value(converter, "toModel");

  if (jsvalue_is_function(func)) {
    jsvalue_t jsfrom = jsvalue_from_value(from, temp);
    jsvalue_t jsret = jsfunc_call(func, converter, &jsfrom, 1);
    ret = jsvalue_to_value(jsret, to, temp);
    jsvalue_unref(jsfrom);
    jsvalue_unref(jsret);
  }
  jsvalue_unref(func);

  return ret;
}

jsvalue_t jsvalue_get_value_validator(const char* name) {
  return jsvalue_get_model_from_mvvm_factory(name, "getValueValidator");
}

bool_t js_value_validator_is_valid(jsvalue_t validator, const value_t* v, str_t* msg) {
  bool_t ret = FALSE;
  jsvalue_t func = jsobj_get_prop_value(validator, "isValid");

  if (jsvalue_is_function(func)) {
    jsvalue_t jsvalue = jsvalue_from_value(v, msg);
    jsvalue_t jsret = jsfunc_call(func, validator, &jsvalue, 1);

    if (jsvalue_is_boolean(jsret)) {
      ret = jsvalue_to_boolean(jsret) ? TRUE : FALSE;
      str_set(msg, "");
    } else if (jsvalue_is_object(jsret)) {
      value_t r;
      value_set_int(&r, 0);

      if (jsobj_has_prop(jsret, "result")) {
        jsobj_get_prop(jsret, "result", &r, msg);
        ret = value_bool(&r) ? TRUE : FALSE;
      }

      if (jsobj_has_prop(jsret, "message")) {
        jsobj_get_prop(jsret, "message", &r, msg);
      }
    }
    jsvalue_unref(jsvalue);
    jsvalue_unref(jsret);
  }
  jsvalue_unref(func);

  return ret;
}

ret_t js_value_validator_fix(jsvalue_t validator, value_t* v) {
  ret_t ret = RET_OK;
  jsvalue_t func = jsobj_get_prop_value(validator, "fix");

  if (jsvalue_is_function(func)) {
    jsvalue_t jsvalue = jsvalue_from_value(v, NULL);
    jsvalue_t jsret = jsfunc_call(func, validator, &jsvalue, 1);
    ret = jsvalue_to_value(jsret, v, NULL);
    jsvalue_unref(jsvalue);
    jsvalue_unref(jsret);
  }
  jsvalue_unref(func);

  return ret;
}

static void navigator_request_free_callback(void* native_p, struct jerry_object_native_info_t *info_p) {
}

static const jerry_object_native_info_t s_navigater_request_info = {
    .free_cb = navigator_request_free_callback};

static ret_t js_navigater_request_set_native_ptr(jsvalue_t obj, void* ptr) {
  return_value_if_fail(ptr != NULL, RET_BAD_PARAMS);

  jerry_set_object_native_pointer(obj, ptr, &s_navigater_request_info);
  return RET_OK;
}

static void* js_navigater_request_get_native_ptr(jsvalue_t obj) {
  void* p = NULL;
  if (jerry_get_object_native_pointer(obj, &p, &s_navigater_request_info)) {
    return VIEW_MODEL(p);
  }
  return NULL;
}

ret_t js_navigater_request_delete_native_ptr(jsvalue_t obj) {
  return jerry_delete_object_native_pointer(obj, &s_navigater_request_info) ? RET_OK : RET_FAIL;
}

static ret_t js_navigater_request_on_destroy(void* ctx, event_t* e) {
  jsvalue_t jsreq = (jsvalue_t)((char*)ctx - (char*)NULL);
  js_navigater_request_delete_native_ptr(jsreq);
  return RET_OK;
}

static ret_t js_navigater_request_on_result(navigator_request_t* req, const value_t* result) {
  jsvalue_t func = (jsvalue_t)tk_object_get_prop_int(TK_OBJECT(req), JSOBJ_PROP_ON_RESULT, 0);

  if (jsvalue_is_function(func)) {
    str_t str;
    str_init(&str, 0);

    jsvalue_t thiz = JS_UNDEFINED;
    jsvalue_t jsargs = jsvalue_from_value(result, &str);
    jsvalue_t jsret = jsfunc_call(func, thiz, &jsargs, 1);
    jsvalue_unref(jsret);
    jsvalue_unref(jsargs);
    jsvalue_unref(thiz);

    str_reset(&str);
  } else {
    log_debug("no onResult\n");
  }

  jsvalue_unref(func);

  return RET_OK;
}

navigator_request_t* jsvalue_to_navigator_request(jsvalue_t obj) {
  navigator_request_t* req = NAVIGATOR_REQUEST(js_navigater_request_get_native_ptr(obj));

  if (req != NULL) {
    tk_object_ref(TK_OBJECT(req));
  } else {
    object_js_factory_t* factory = object_js_factory();
    tk_object_t* args = object_js_factory_create_object(factory, jsvalue_ref(obj), TRUE);

    if (args != NULL) {
      req = navigator_request_create("", js_navigater_request_on_result);
      if (req != NULL) {
        void* ctx = (char*)NULL + (int32_t)obj;
        js_navigater_request_set_native_ptr(obj, req);
        emitter_on(EMITTER(args), EVT_DESTROY, js_navigater_request_on_destroy, ctx);

        navigator_request_set_args(req, args);
      }

      tk_object_unref(args);
    }
  }

  return req;
}

jsvalue_t jsvalue_from_navigator_request(navigator_request_t* req) {
  return jsvalue_from_obj_copy(TK_OBJECT(req));
}

jsvalue_t jsvalue_get_application(void) {
  return jsvalue_get_model_from_mvvm_factory(NULL, "getApplication");
}

ret_t js_application_on_launch(void) {
  ret_t ret = RET_NOT_FOUND;
  jsvalue_t jsapp = jsvalue_get_application();

  if (jsvalue_check(jsapp) == RET_OK) {
    jsvalue_t jsargs = JS_UNDEFINED;
    ret = jsobj_exec_ex(jsapp, "onLaunch", jsargs);
    jsvalue_unref(jsargs);
    jsvalue_unref(jsapp);
  }

  return ret;
}

ret_t js_application_on_exit(void) {
  ret_t ret = RET_NOT_FOUND;
  jsvalue_t jsapp = jsvalue_get_application();

  if (jsvalue_check(jsapp) == RET_OK) {
    jsvalue_t jsargs = JS_UNDEFINED;
    ret = jsobj_exec_ex(jsapp, "onExit", jsargs);
    jsvalue_unref(jsargs);
    jsvalue_unref(jsapp);
  }

  return ret;
}
