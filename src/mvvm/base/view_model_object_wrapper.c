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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_object_wrapper.h"
#include "mvvm/base/view_model_array_object_wrapper.h"

static ret_t view_model_object_wrapper_on_changed(void* ctx, event_t* e) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(ctx);

  emitter_dispatch(EMITTER(object_wrapper), e);

  return RET_OK;
}

static ret_t view_model_object_wrapper_on_destroy(object_t* obj) {
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);

  emitter_off_by_ctx(EMITTER(object_wrapper->obj), obj);
  TKMEM_FREE(object_wrapper->prop_prefix);
  OBJECT_UNREF(object_wrapper->obj);

  return view_model_deinit(VIEW_MODEL(obj));
}

static int32_t view_model_object_wrapper_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_object_wrapper_set_prop(object_t* obj, const char* name, const value_t* v) {
  char path[MAX_PATH + 1];
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (object_wrapper->prop_prefix != NULL) {
    tk_snprintf(path, MAX_PATH, "%s.%s", object_wrapper->prop_prefix, name);
    name = path;
  }

  return object_set_prop(OBJECT(object_wrapper->obj), name, v);
}

static ret_t view_model_object_wrapper_get_prop(object_t* obj, const char* name, value_t* v) {
  char path[MAX_PATH + 1];
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (object_wrapper->prop_prefix != NULL) {
    tk_snprintf(path, MAX_PATH, "%s.%s", object_wrapper->prop_prefix, name);
    name = path;
  }

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

static view_model_t* view_model_object_create_sub_view_model(view_model_t* view_model,
                                                             const char* name) {
  str_t str;
  view_model_t* sub = NULL;
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(view_model);

  if (object_wrapper->prop_prefix != NULL) {
    str_init(&str, 100);
    str_append_more(&str, object_wrapper->prop_prefix, ".", name, NULL);
    sub = view_model_object_wrapper_create_ex(object_wrapper->obj, name);
    str_reset(&str);
  } else {
    sub = view_model_object_wrapper_create_ex(object_wrapper->obj, name);
  }

  return sub;
}

static view_model_t* view_model_object_create_sub_view_model_array(view_model_t* view_model,
                                                                   const char* name) {
  str_t str;
  view_model_t* sub = NULL;
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(view_model);

  if (object_wrapper->prop_prefix != NULL) {
    str_init(&str, 100);
    str_append_more(&str, object_wrapper->prop_prefix, ".", name, NULL);
    sub = view_model_array_object_wrapper_create_ex(object_wrapper->obj, name);
    str_reset(&str);
  } else {
    sub = view_model_array_object_wrapper_create_ex(object_wrapper->obj, name);
  }

  return sub;
}

static ret_t view_model_object_wrapper_on_will_mount(view_model_t* view_model,
                                                     navigator_request_t* req) {
  const char* prefix = object_get_prop_str(OBJECT(req), STR_PATH_PREFIX);
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(view_model);

  if (prefix != NULL) {
    object_wrapper->prop_prefix = tk_str_copy(object_wrapper->prop_prefix, prefix);
  }

  return RET_OK;
}

static const view_model_vtable_t s_view_model_vtable = {
    .create_sub_view_model = view_model_object_create_sub_view_model,
    .create_sub_view_model_array = view_model_object_create_sub_view_model_array,
    .on_will_mount = view_model_object_wrapper_on_will_mount};

static const object_vtable_t s_object_vtable = {.type = "view_model_object_wrapper",
                                                .desc = "view_model_object_wrapper",
                                                .size = sizeof(view_model_object_wrapper_t),
                                                .is_collection = FALSE,
                                                .on_destroy = view_model_object_wrapper_on_destroy,
                                                .compare = view_model_object_wrapper_compare,
                                                .get_prop = view_model_object_wrapper_get_prop,
                                                .set_prop = view_model_object_wrapper_set_prop,
                                                .can_exec = view_model_object_wrapper_can_exec,
                                                .exec = view_model_object_wrapper_exec};

view_model_t* view_model_object_wrapper_create_ex(object_t* obj, const char* prop_prefix) {
  object_t* model = object_create(&s_object_vtable);
  view_model_object_wrapper_t* object_wrapper = VIEW_MODEL_OBJECT_WRAPPPER(model);
  view_model_t* view_model = view_model_init(VIEW_MODEL(model));
  return_value_if_fail(object_wrapper != NULL && obj != NULL, NULL);

  view_model->vt = &s_view_model_vtable;
  object_wrapper->obj = object_ref(obj);
  object_wrapper->prop_prefix = prop_prefix != NULL ? tk_strdup(prop_prefix) : NULL;
  emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, view_model_object_wrapper_on_changed, model);
  emitter_on(EMITTER(obj), EVT_PROP_CHANGED, view_model_object_wrapper_on_changed, model);

  return view_model;
}

view_model_t* view_model_object_wrapper_create(object_t* obj) {
  return view_model_object_wrapper_create_ex(obj, NULL);
}
