/**
 * File:   view_model_dummy.c
 * Author: AWTK Develop Team
 * Brief:  view_model_dummy
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/func_call_parser.h"
#include "mvvm/base/view_model_dummy.h"

static ret_t view_model_dummy_on_destroy(tk_object_t* obj) {
  view_model_dummy_t* dummy = VIEW_MODEL_DUMMY(obj);

  tk_object_unref(TK_OBJECT(dummy->props));

  return view_model_deinit(VIEW_MODEL(obj));
}

static int32_t view_model_dummy_compare(tk_object_t* obj, tk_object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_dummy_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  view_model_dummy_t* dummy = VIEW_MODEL_DUMMY(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop(TK_OBJECT(dummy->props), name, v);
}

static ret_t view_model_dummy_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  view_model_dummy_t* dummy = VIEW_MODEL_DUMMY(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_object_get_prop(TK_OBJECT(dummy->props), name, v) != RET_OK) {
    value_set_int(v, 0);
  }

  return RET_OK;
}

static bool_t view_model_dummy_can_exec(tk_object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL, FALSE);

  return FALSE;
}

static ret_t view_model_dummy_exec(tk_object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static const object_vtable_t s_model_dummy_vtable = {.type = "view_model_dummy",
                                                     .desc = "view_model_dummy",
                                                     .size = sizeof(view_model_dummy_t),
                                                     .is_collection = FALSE,
                                                     .on_destroy = view_model_dummy_on_destroy,
                                                     .compare = view_model_dummy_compare,
                                                     .get_prop = view_model_dummy_get_prop,
                                                     .set_prop = view_model_dummy_set_prop,
                                                     .can_exec = view_model_dummy_can_exec,
                                                     .exec = view_model_dummy_exec};

view_model_t* view_model_dummy_create(navigator_request_t* req) {
  tk_object_t* obj = tk_object_create(&s_model_dummy_vtable);
  view_model_dummy_t* dummy = VIEW_MODEL_DUMMY(obj);
  const char* type_and_args = NULL;
  return_value_if_fail(dummy != NULL, NULL);

  type_and_args = tk_object_get_prop_str(req->args, NAVIGATOR_ARG_VIEW_MODEL_TYPE);
  if (type_and_args != NULL && strchr(type_and_args, '(') != NULL) {
    dummy->props = func_call_parse(type_and_args, tk_strlen(type_and_args));
  }

  if (dummy->props == NULL) {
    dummy->props = object_default_create();
  }

  return view_model_init(VIEW_MODEL(obj));
}
