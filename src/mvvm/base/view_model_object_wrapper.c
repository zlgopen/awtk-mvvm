/**
 * File:   view_model_object_wrapper.c
 * Author: AWTK Develop Team
 * Brief:  view_model_object_wrapper
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
 * 2019-04-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_object_wrapper.h"

static ret_t view_model_object_wrapper_on_changed(void* ctx, event_t* e) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(ctx);

  emitter_dispatch(EMITTER(object_wrapper->obj), e);

  return RET_OK;
}

static ret_t view_model_object_wrapper_on_destroy(object_t* obj) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);

  emitter_off_by_ctx(EMITTER(object_wrapper->obj), obj);
  OBJECT_UNREF(object_wrapper->obj);

  return RET_OK;
}

static int32_t view_model_object_wrapper_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_object_wrapper_set_prop(object_t* obj, const char* name, const value_t* v) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return object_set_prop(OBJECT(object_wrapper->obj), name, v);
}

static ret_t view_model_object_wrapper_get_prop(object_t* obj, const char* name, value_t* v) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return object_get_prop(OBJECT(object_wrapper->obj), name, v);
}

static bool_t view_model_object_wrapper_can_exec(object_t* obj, const char* name,
                                                 const char* args) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL, FALSE);

  return object_can_exec(OBJECT(object_wrapper->obj), name, args);
}

static ret_t view_model_object_wrapper_exec(object_t* obj, const char* name, const char* args) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return object_exec(OBJECT(object_wrapper->obj), name, args);
}

static const object_vtable_t s_model_object_wrapper_vtable = {
    .type = "view_model_object_wrapper",
    .desc = "view_model_object_wrapper",
    .size = sizeof(view_model_object_wrapper_t),
    .is_collection = FALSE,
    .on_destroy = view_model_object_wrapper_on_destroy,
    .compare = view_model_object_wrapper_compare,
    .get_prop = view_model_object_wrapper_get_prop,
    .set_prop = view_model_object_wrapper_set_prop,
    .can_exec = view_model_object_wrapper_can_exec,
    .exec = view_model_object_wrapper_exec};

view_model_t* view_model_object_wrapper_create(object_t* obj) {
  object_t* model = object_create(&s_model_object_wrapper_vtable);
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(object_wrapper != NULL && obj != NULL, NULL);

  object_ref(obj);
  object_wrapper->obj = obj;
  emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, view_model_object_wrapper_on_changed, model);

  return VIEW_MODEL(model);
}
