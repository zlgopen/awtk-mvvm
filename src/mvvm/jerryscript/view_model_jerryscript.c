/**
 * File:   view_model_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented view_model
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/view_model_jerryscript.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"

static ret_t view_model_jerryscript_on_destroy(object_t* obj) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(obj);

  str_reset(&(view_modeljs->temp));
  jerry_release_value(view_modeljs->jsobj);

  return RET_OK;
}

static int32_t view_model_jerryscript_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_jerryscript_set_prop(object_t* obj, const char* name, const value_t* v) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return jsobj_set_prop(view_modeljs->jsobj, name, v, &(view_modeljs->temp));
}

static ret_t view_model_jerryscript_get_prop(object_t* obj, const char* name, value_t* v) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  value_set_int(v, 0);
  if (jsobj_has_prop(view_modeljs->jsobj, name)) {
    return jsobj_get_prop(view_modeljs->jsobj, name, v, &(view_modeljs->temp));
  }

  return RET_NOT_FOUND;
}

static bool_t view_model_jerryscript_can_exec(object_t* obj, const char* name, const char* args) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return jsobj_can_exec(view_modeljs->jsobj, name, args);
}

static ret_t view_model_jerryscript_exec(object_t* obj, const char* name, const char* args) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return jsobj_exec(view_modeljs->jsobj, name, args);
}

#define STR_NATIVE_VIEW_MODEL "nativeModel"

jerry_value_t wrap_notify_props_changed(const jerry_value_t func_obj_val,
                                        const jerry_value_t this_p, const jerry_value_t args_p[],
                                        const jerry_length_t args_cnt) {
  object_t* obj = OBJECT(jsobj_get_prop_pointer(this_p, STR_NATIVE_VIEW_MODEL));

  return jerry_create_number(object_notify_changed(obj));
}

static const object_vtable_t s_model_jerryscript_obj_vtable = {
    .type = "view_model_jerryscript",
    .desc = "view_model_jerryscript",
    .size = sizeof(view_model_jerryscript_t),
    .is_collection = FALSE,
    .on_destroy = view_model_jerryscript_on_destroy,

    .compare = view_model_jerryscript_compare,
    .get_prop = view_model_jerryscript_get_prop,
    .set_prop = view_model_jerryscript_set_prop,
    .can_exec = view_model_jerryscript_can_exec,
    .exec = view_model_jerryscript_exec};

static ret_t view_model_jerryscript_on_will_mount(view_model_t* view_model,
                                                  navigator_request_t* req) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(view_model);
  jerry_value_t jsargs = jerry_value_from_navigator_request(req);

  ret_t ret = jsobj_exec_ex(view_modeljs->jsobj, "onWillMount", jsargs);
  jerry_release_value(jsargs);

  return ret;
}

static jerry_value_t jsobj_create_model_by_creator(const char* name, navigator_request_t* req) {
  jerry_value_t func = jsobj_get_model(name);
  if (jerry_value_is_function(func)) {
    jerry_value_t global_obj = jerry_get_global_object();
    jerry_value_t jsargs = jerry_value_from_navigator_request(req);
    jerry_value_t jsret = jerry_call_function(func, global_obj, &jsargs, 1);
    jerry_release_value(func);
    jerry_release_value(jsargs);
    jerry_release_value(global_obj);

    return jsret;
  } else {
    jerry_release_value(func);

    return jerry_create_undefined();
  }
}

static jerry_value_t jsobj_create_model(const char* name, navigator_request_t* req) {
  char camel_name[TK_NAME_LEN * 2 + 1];
  char underscore_name[TK_NAME_LEN * 2 + 1];

  /*try under score name: test_obj*/
  jerry_value_t view_model = jsobj_get_model(name);
  if (jerry_value_is_object(view_model)) {
    return view_model;
  } else {
    log_debug("js create view_model: try %s failed\n", name);
    jerry_release_value(view_model);
  }

  memset(camel_name, 0x00, sizeof(camel_name));
  memset(underscore_name, 0x00, sizeof(underscore_name));

  /*try camel name: testObj*/
  tk_under_score_to_camel(name, camel_name, sizeof(camel_name) - 1);
  view_model = jsobj_get_model(camel_name);
  if (jerry_value_is_object(view_model)) {
    return view_model;
  } else {
    log_debug("js create view_model: try %s failed\n", camel_name);
    jerry_release_value(view_model);
  }

  /*try under score creator: create_test_obj*/
  tk_snprintf(underscore_name, sizeof(underscore_name) - 1, "create_%s", name);
  view_model = jsobj_create_model_by_creator(underscore_name, req);
  if (jerry_value_is_object(view_model)) {
    return view_model;
  } else {
    log_debug("js create view_model: try %s failed\n", underscore_name);
    jerry_release_value(view_model);
  }

  /*try camel creator: createTestObj*/
  tk_under_score_to_camel(underscore_name, camel_name, sizeof(camel_name) - 1);
  view_model = jsobj_create_model_by_creator(camel_name, req);

  if (!jerry_value_is_object(view_model)) {
    log_debug("js create view_model: try %s failed\n", camel_name);
    log_warn("%s: not found valid view_model for %s\n", __FUNCTION__, name);
  }

  return view_model;
}

static ret_t view_model_jerryscript_on_mount(view_model_t* view_model) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(view_model);

  return jsobj_exec(view_modeljs->jsobj, "onMount", NULL);
}

static ret_t view_model_jerryscript_on_will_unmount(view_model_t* view_model) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(view_model);

  return jsobj_exec(view_modeljs->jsobj, "onWillUnmount", NULL);
}

static ret_t view_model_jerryscript_on_unmount(view_model_t* view_model) {
  view_model_jerryscript_t* view_modeljs = VIEW_MODEL_JERRYSCRIPT(view_model);

  return jsobj_exec(view_modeljs->jsobj, "onUnmount", NULL);
}

const static view_model_vtable_t s_model_jerryscript_model_vtable = {
    .on_will_mount = view_model_jerryscript_on_will_mount,
    .on_mount = view_model_jerryscript_on_mount,
    .on_will_unmount = view_model_jerryscript_on_will_unmount,
    .on_unmount = view_model_jerryscript_on_unmount};

view_model_t* view_model_jerryscript_create(const char* name, const char* code, uint32_t code_size,
                                            navigator_request_t* req) {
  object_t* obj = NULL;
  jerry_value_t jsret = 0;
  jerry_value_t jscode = 0;
  view_model_jerryscript_t* view_modeljs = NULL;
  return_value_if_fail(name != NULL && code != NULL && code_size > 0, NULL);

  obj = object_create(&s_model_jerryscript_obj_vtable);
  view_modeljs = VIEW_MODEL_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL, NULL);

  object_set_name(obj, name);
  jscode = jerry_parse((const jerry_char_t*)name, strlen(name), (const jerry_char_t*)code,
                       code_size, JERRY_PARSE_NO_OPTS);

  goto_error_if_fail(jerry_value_check(jscode) == RET_OK);

  jsret = jerry_run(jscode);
  jerry_release_value(jscode);
  goto_error_if_fail(jerry_value_check(jsret) == RET_OK);

  view_modeljs->jsobj = jsobj_create_model(name, req);
  goto_error_if_fail(jerry_value_check(view_modeljs->jsobj) == RET_OK);

  str_init(&(view_modeljs->temp), 0);
  jsobj_set_prop_pointer(view_modeljs->jsobj, STR_NATIVE_VIEW_MODEL, OBJECT(view_modeljs));
  jsobj_set_prop_func(view_modeljs->jsobj, "notifyPropsChanged", wrap_notify_props_changed);

  jerry_release_value(jsret);
  VIEW_MODEL(obj)->vt = &s_model_jerryscript_model_vtable;

  return VIEW_MODEL(obj);
error:
  object_unref(obj);

  return NULL;
}
