/**
 * File:   view_model_array_jerryscript.c
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
 * 2019-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/view_model_array_jerryscript.h"

static ret_t view_model_array_jerryscript_on_destroy(object_t* obj) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(obj);

  str_reset(&(view_modeljs->temp));
  view_model_array_deinit(VIEW_MODEL(obj));
  ;
  jerry_release_value(view_modeljs->jsobj);

  return RET_OK;
}

static int32_t view_model_array_jerryscript_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_array_jerryscript_set_prop(object_t* obj, const char* name,
                                                   const value_t* v) {
  uint32_t index = 0;
  jerry_value_t jsprop = 0;
  ret_t ret = RET_NOT_FOUND;
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(VIEW_MODEL(obj), value_int(v));

    return RET_OK;
  }

  name = tk_destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < jerry_get_array_length(view_modeljs->jsobj), RET_BAD_PARAMS);

  jsprop = jerry_get_property_by_index(view_modeljs->jsobj, index);
  ret = jsobj_set_prop(jsprop, name, v, &(view_modeljs->temp));
  jerry_release_value(jsprop);

  return ret;
}

static ret_t view_model_array_jerryscript_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  jerry_value_t jsprop = 0;
  ret_t ret = RET_NOT_FOUND;
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, jerry_get_array_length(view_modeljs->jsobj));

    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);

    return RET_OK;
  }

  name = tk_destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < jerry_get_array_length(view_modeljs->jsobj), RET_BAD_PARAMS);

  value_set_int(v, 0);
  jsprop = jerry_get_property_by_index(view_modeljs->jsobj, index);
  if (jsobj_has_prop(jsprop, name)) {
    ret = jsobj_get_prop(jsprop, name, v, &(view_modeljs->temp));
  }
  jerry_release_value(jsprop);

  return ret;
}

static bool_t view_model_array_jerryscript_can_exec(object_t* obj, const char* name,
                                                    const char* args) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return jsobj_can_exec(view_modeljs->jsobj, name, args);
}

static ret_t view_model_array_jerryscript_exec(object_t* obj, const char* name, const char* args) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return jsobj_exec(view_modeljs->jsobj, name, args);
}

static const object_vtable_t s_obj_view_model_array_jerryscript_vtable = {
    .type = "view_model_array_jerryscript",
    .desc = "view_model_array_jerryscript",
    .size = sizeof(view_model_array_jerryscript_t),
    .is_collection = TRUE,
    .on_destroy = view_model_array_jerryscript_on_destroy,

    .compare = view_model_array_jerryscript_compare,
    .get_prop = view_model_array_jerryscript_get_prop,
    .set_prop = view_model_array_jerryscript_set_prop,
    .can_exec = view_model_array_jerryscript_can_exec,
    .exec = view_model_array_jerryscript_exec};

static ret_t view_model_jerryscript_on_will_mount(view_model_t* view_model,
                                                  navigator_request_t* req) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(view_model);
  jerry_value_t jsargs = jerry_value_from_navigator_request(req);

  ret_t ret = jsobj_exec_ex(view_modeljs->jsobj, "onWillMount", jsargs);
  jerry_release_value(jsargs);

  return ret;
}

static ret_t view_model_jerryscript_on_mount(view_model_t* view_model) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(view_model);

  return jsobj_exec(view_modeljs->jsobj, "onMount", NULL);
}

static ret_t view_model_jerryscript_on_will_unmount(view_model_t* view_model) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(view_model);

  return jsobj_exec(view_modeljs->jsobj, "onWillUnmount", NULL);
}

static ret_t view_model_jerryscript_on_unmount(view_model_t* view_model) {
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(view_model);

  return jsobj_exec(view_modeljs->jsobj, "onUnmount", NULL);
}

const static view_model_vtable_t s_view_model_jerryscript_vtable = {
    .on_will_mount = view_model_jerryscript_on_will_mount,
    .on_mount = view_model_jerryscript_on_mount,
    .on_will_unmount = view_model_jerryscript_on_will_unmount,
    .on_unmount = view_model_jerryscript_on_unmount};

view_model_t* view_model_array_jerryscript_create(jerry_value_t jsobj) {
  object_t* obj = object_create(&s_obj_view_model_array_jerryscript_vtable);
  view_model_array_jerryscript_t* view_modeljs = VIEW_MODEL_ARRAY_JERRYSCRIPT(obj);
  return_value_if_fail(view_modeljs != NULL, NULL);

  view_modeljs->jsobj = jsobj;
  str_init(&(view_modeljs->temp), 0);
  view_model_array_init(VIEW_MODEL(obj));
  VIEW_MODEL(view_modeljs)->vt = &s_view_model_jerryscript_vtable;

  return VIEW_MODEL(obj);
}
