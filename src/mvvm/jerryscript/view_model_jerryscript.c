/**
 * File:   view_model_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented view_model
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/jerry_script_helper.h"
#include "mvvm/jerryscript/view_model_jerryscript.h"
#include "mvvm/jerryscript/view_model_array_jerryscript.h"
#include "mvvm/jerryscript/view_model_normal_jerryscript.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"

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

ret_t view_model_jerryscript_load(const char* name, const char* code, uint32_t code_size) {
  return jerry_script_eval_buff(code, code_size, name, TRUE);
}

#define STR_NATIVE_MODEL "nativeModel"

jerry_value_t wrap_notify_props_changed(const jerry_value_t func_obj_val,
                                        const jerry_value_t this_p, const jerry_value_t args_p[],
                                        const jerry_length_t args_cnt) {
  object_t* obj = OBJECT(jsobj_get_prop_pointer(this_p, STR_NATIVE_MODEL));

  return jerry_create_number(object_notify_changed(obj));
}

jerry_value_t wrap_notify_items_changed(const jerry_value_t func_obj_val,
                                        const jerry_value_t this_p, const jerry_value_t args_p[],
                                        const jerry_length_t args_cnt) {
  object_t* obj = OBJECT(jsobj_get_prop_pointer(this_p, STR_NATIVE_MODEL));

  return jerry_create_number(view_model_array_notify_items_changed(VIEW_MODEL(obj)));
}

view_model_t* view_model_jerryscript_create(const char* name, const char* code, uint32_t code_size,
                                            navigator_request_t* req) {
  jerry_value_t jsobj = 0;
  view_model_t* view_model = NULL;
  return_value_if_fail(name != NULL && code != NULL && code_size > 0, NULL);

  return_value_if_fail(view_model_jerryscript_load(name, code, code_size) == RET_OK, NULL);

  jsobj = jsobj_create_model(name, req);
  return_value_if_fail(jerry_value_is_object(jsobj), NULL);
  return_value_if_fail(jerry_value_check(jsobj) == RET_OK, NULL);

  if (jerry_value_is_array(jsobj)) {
    view_model = view_model_array_jerryscript_create(jsobj);
    jsobj_set_prop_func(jsobj, "notifyPropsChanged", wrap_notify_props_changed);
    jsobj_set_prop_func(jsobj, "notifyItemsChanged", wrap_notify_items_changed);
  } else {
    view_model = view_model_normal_jerryscript_create(jsobj);
    jsobj_set_prop_func(jsobj, "notifyPropsChanged", wrap_notify_props_changed);
  }

  if (view_model != NULL) {
    object_set_name(OBJECT(view_model), name);
    jsobj_set_prop_pointer(jsobj, STR_NATIVE_MODEL, OBJECT(view_model));
  } else {
    jerry_release_value(jsobj);
    return NULL;
  }

  return VIEW_MODEL(view_model);
}
