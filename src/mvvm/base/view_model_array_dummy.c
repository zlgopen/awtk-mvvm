/**
 * File:   view_model_array_dummy.c
 * Author: AWTK Develop Team
 * Brief:  view_model_array_dummy
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
#include "tkc/object_array.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/view_model_dummy.h"
#include "mvvm/base/view_model_array_dummy.h"

static ret_t view_model_array_dummy_on_destroy(tk_object_t* obj) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);

  TK_OBJECT_UNREF(dummy->array);
  view_model_array_deinit(VIEW_MODEL(dummy));

  return RET_OK;
}

static int32_t view_model_array_dummy_compare(tk_object_t* obj, tk_object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_array_dummy_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t view_model_array_dummy_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  view_model_array_dummy_t* vm = VIEW_MODEL_ARRAY_DUMMY(obj);

  tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
  if (sub != NULL) {
    return tk_object_get_prop(sub, name, v);
  }

  if (tk_str_ieq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_object(v, vm->array);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t view_model_array_dummy_can_exec(tk_object_t* obj, const char* name,
                                              const char* args) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);
  return_value_if_fail(obj != NULL && name != NULL, FALSE);
  if (tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_CLEAR)) {
    return OBJECT_ARRAY(dummy->array)->size > 0; 
  }

  return FALSE;
}

static ret_t view_model_array_dummy_exec(tk_object_t* obj, const char* name, const char* args) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);
  return_value_if_fail(dummy != NULL && name != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    view_model_t* item = view_model_dummy_create(NULL);
    return_value_if_fail(item != NULL, RET_OOM);
    view_model_array_dummy_add(VIEW_MODEL(obj), item);
    TK_OBJECT_UNREF(item);

    return RET_OK;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_CLEAR)) {
    view_model_array_dummy_clear(VIEW_MODEL(obj));
    return RET_OK;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE)) {
    uint32_t index = 0;
    if (args != NULL) {
      tk_object_t* a = object_default_create();
      tk_command_arguments_to_object(args, a);
      index = tk_object_get_prop_int32(a, "index", -1);
      TK_OBJECT_UNREF(a);
    }

    view_model_array_dummy_remove(VIEW_MODEL(obj), index);
    return RET_OK;
  }
  return RET_NOT_IMPL;
}

static const object_vtable_t s_model_array_vtable = {
    .type = "view_model_array_dummy",
    .desc = "view_model_array_dummy",
    .size = sizeof(view_model_array_dummy_t),
    .is_collection = TRUE,
    .on_destroy = view_model_array_dummy_on_destroy,

    .compare = view_model_array_dummy_compare,
    .get_prop = view_model_array_dummy_get_prop,
    .set_prop = view_model_array_dummy_set_prop,
    .can_exec = view_model_array_dummy_can_exec,
    .exec = view_model_array_dummy_exec};

view_model_t* view_model_array_dummy_create(navigator_request_t* req) {
  tk_object_t* obj = tk_object_create(&s_model_array_vtable);
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);
  return_value_if_fail(dummy != NULL, NULL);

  view_model_array_init(VIEW_MODEL(obj));
  dummy->array = object_array_create();

  return VIEW_MODEL(obj);
}

ret_t view_model_array_dummy_clear(view_model_t* view_model) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL, RET_BAD_PARAMS);

  object_array_clear_props(dummy->array);
  view_model_notify_items_changed(VIEW_MODEL(dummy), dummy->array);

  return RET_OK;
}

ret_t view_model_array_dummy_add(view_model_t* view_model, view_model_t* submodel) {
  value_t v;
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL, RET_BAD_PARAMS);

  value_set_object(&v, TK_OBJECT(submodel));
  emitter_on(EMITTER(submodel), EVT_PROP_CHANGED, emitter_forward, TK_OBJECT(dummy));

  if (object_array_push(dummy->array, &v) == RET_OK) {
    view_model_notify_items_changed(VIEW_MODEL(dummy), dummy->array);
  }

  return RET_OK;
}

ret_t view_model_array_dummy_remove(view_model_t* view_model, uint32_t index) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  uint32_t size = OBJECT_ARRAY(dummy->array)->size;
  return_value_if_fail(dummy != NULL && index < size, RET_BAD_PARAMS);

  object_array_remove(dummy->array, index);
  view_model_notify_items_changed(VIEW_MODEL(dummy), dummy->array);

  return RET_OK;
}

view_model_t* view_model_array_dummy_get(view_model_t* view_model, uint32_t index) {
  value_t v;
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  uint32_t size = OBJECT_ARRAY(dummy->array)->size;
  return_value_if_fail(dummy != NULL && index < size, NULL);

  object_array_get(dummy->array, index, &v);
  return VIEW_MODEL(value_object(&v));
}

int32_t view_model_array_dummy_size(view_model_t* view_model) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  uint32_t size = OBJECT_ARRAY(dummy->array)->size;
  return_value_if_fail(dummy != NULL, 0);

  return size;
}
