/**
 * File:   view_model_array_object_wrapper.c
 * Author: AWTK Develop Team
 * Brief:  view_model_array_object_wrapper
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_object_wrapper.h"
#include "mvvm/base/view_model_array_object_wrapper.h"

static ret_t view_model_array_object_wrapper_on_changed(void* ctx, event_t* e) {
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(ctx);

  emitter_dispatch(EMITTER(object_wrapper), e);

  return RET_OK;
}

static ret_t view_model_array_object_wrapper_on_destroy(object_t* obj) {
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(obj);

  emitter_off_by_ctx(EMITTER(object_wrapper->obj), obj);
  TKMEM_FREE(object_wrapper->prop_prefix);
  OBJECT_UNREF(object_wrapper->obj);

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static int32_t view_model_array_object_wrapper_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_array_object_wrapper_set_prop(object_t* obj, const char* name,
                                                      const value_t* v) {
  char path[MAX_PATH + 1];
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (view_model_array_default_set_prop(view_model, name, v) == RET_OK) {
    return RET_OK;
  }

  if (object_wrapper->prop_prefix != NULL) {
    tk_snprintf(path, MAX_PATH, "%s.%s", object_wrapper->prop_prefix, name);
    name = path;
  }

  return object_set_prop(OBJECT(object_wrapper->obj), name, v);
}

static ret_t view_model_array_object_wrapper_get_prop(object_t* obj, const char* name, value_t* v) {
  char path[MAX_PATH + 1];
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (view_model_array_default_get_prop(view_model, name, v) == RET_OK) {
    return RET_OK;
  }

  if (tk_str_ieq(VIEW_MODEL_PROP_ITEMS, name)) {
    name = "#size";
  }

  if (object_wrapper->prop_prefix != NULL) {
    tk_snprintf(path, MAX_PATH, "%s.%s", object_wrapper->prop_prefix, name);
    name = path;
  }

  return object_get_prop(OBJECT(object_wrapper->obj), name, v);
}

static bool_t view_model_array_object_wrapper_can_exec(object_t* obj, const char* name,
                                                       const char* args) {
  char path[MAX_PATH + 1];
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(obj);
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(obj);

  return_value_if_fail(obj != NULL && name != NULL, FALSE);

  if (view_model_array_default_can_exec(VIEW_MODEL(obj), name, args)) {
    return TRUE;
  }
  
  if (tk_str_eq(name, OBJECT_CMD_EDIT)) {
    return TRUE;
  }

  if (tk_str_eq(name, OBJECT_CMD_ADD)) {
    args = object_wrapper->prop_prefix;
  } else {
    if (object_wrapper->prop_prefix != NULL) {
      tk_snprintf(path, MAX_PATH, "%s.[%u]", object_wrapper->prop_prefix, vm_array->selected_index);
    } else {
      tk_snprintf(path, MAX_PATH, "[%u]", vm_array->selected_index);
    }

    args = path;
  }

  return object_can_exec(OBJECT(object_wrapper->obj), name, args);
}

static const char* view_model_array_object_wrapper_get_target(char target[TK_NAME_LEN + 1],
                                                              const char* prefix,
                                                              const char* action) {
  const char* p = NULL;
  if (prefix != NULL) {
    p = strrchr(prefix, '.');
    if (p == NULL) {
      p = prefix;
    } else {
      p++;
    }
    tk_snprintf(target, TK_NAME_LEN, "%s_%s", p, action);
  } else {
    tk_snprintf(target, TK_NAME_LEN, "%s", action);
  }

  return target;
}

static ret_t view_model_array_object_wrapper_cmd_add(const char* prefix, const char* path) {
  char target[TK_NAME_LEN + 1];

  view_model_array_object_wrapper_get_target(target, prefix, "add");

  return navigator_to_with_key_value(target, STR_PATH_PREFIX, path);
}

static ret_t view_model_array_object_wrapper_cmd_edit(const char* prefix, const char* path) {
  char target[TK_NAME_LEN + 1];

  view_model_array_object_wrapper_get_target(target, prefix, "edit");

  return navigator_to_with_key_value(target, STR_PATH_PREFIX, path);
}

static ret_t view_model_array_object_wrapper_exec(object_t* obj, const char* name,
                                                  const char* args) {
  ret_t ret = RET_OK;
  char path[MAX_PATH + 1];
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(obj);
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  ret = view_model_array_default_exec(VIEW_MODEL(obj), name, args);
  if (ret != RET_NOT_FOUND) {
    return ret;
  }

  if (tk_str_eq(name, OBJECT_CMD_CLEAR) || tk_str_eq(name, OBJECT_CMD_ADD)) {
    args = object_wrapper->prop_prefix;
  } else {
    if (object_wrapper->prop_prefix != NULL) {
      tk_snprintf(path, MAX_PATH, "%s.[%u]", object_wrapper->prop_prefix, vm_array->selected_index);
    } else {
      tk_snprintf(path, MAX_PATH, "[%u]", vm_array->selected_index);
    }

    args = path;
  }
  
  if (tk_str_eq(name, OBJECT_CMD_ADD) || tk_str_eq(name, OBJECT_CMD_EDIT)) {
    object_exec(object_wrapper->obj, OBJECT_CMD_SAVE, NULL);
  }

  ret = object_exec(OBJECT(object_wrapper->obj), name, args);
  if (tk_str_eq(name, OBJECT_CMD_ADD)) {
    uint32_t nr = object_get_prop_int(OBJECT(obj), VIEW_MODEL_PROP_ITEMS, 0);
    int32_t last_index = nr - 1;
    return_value_if_fail(ret == RET_OK, RET_FAIL);
    return_value_if_fail(last_index >= 0, RET_FAIL);

    if (object_wrapper->prop_prefix != NULL) {
      tk_snprintf(path, MAX_PATH, "%s.[%d]", object_wrapper->prop_prefix, last_index);
    } else {
      tk_snprintf(path, MAX_PATH, "[%d]", last_index);
    }

    return view_model_array_object_wrapper_cmd_add(object_wrapper->prop_prefix, path);
  }

  if (ret == RET_NOT_FOUND || ret == RET_NOT_IMPL) {
    if (tk_str_eq(name, OBJECT_CMD_EDIT)) {
      return view_model_array_object_wrapper_cmd_edit(object_wrapper->prop_prefix, path);
    }
  }

  if (tk_str_eq(name, OBJECT_CMD_MOVE_UP)) {
    if (ret == RET_OK) {
      if (vm_array->selected_index > 0) {
        vm_array->selected_index--;
      }
    }
  } else if (tk_str_eq(name, OBJECT_CMD_MOVE_DOWN)) {
    if (ret == RET_OK) {
      uint32_t nr = object_get_prop_int(OBJECT(obj), VIEW_MODEL_PROP_ITEMS, 0);
      if ((vm_array->selected_index + 1) < nr) {
        vm_array->selected_index++;
      }
    }
  } else if (tk_str_eq(name, OBJECT_CMD_CLEAR)) {
    vm_array->selected_index = 0;
  } else if (tk_str_eq(name, OBJECT_CMD_REMOVE)) {
    uint32_t nr = object_get_prop_int(OBJECT(obj), VIEW_MODEL_PROP_ITEMS, 0);

    if (vm_array->selected_index >= nr) {
      if (nr > 0) {
        vm_array->selected_index = nr - 1;
      } else {
        vm_array->selected_index = 0;
      }
    }
  }

  return ret;
}

view_model_t* view_model_array_object_wrapper_create_ex(object_t* obj, const char* prop_prefix);

static view_model_t* view_model_object_create_sub_view_model(view_model_t* view_model,
                                                             const char* name) {
  str_t str;
  view_model_t* sub = NULL;
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(view_model);

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
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(view_model);

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

static ret_t view_model_array_object_wrapper_on_will_mount(view_model_t* view_model,
                                                           navigator_request_t* req) {
  const char* prefix = object_get_prop_str(OBJECT(req), STR_PATH_PREFIX);
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(view_model);

  if (prefix != NULL) {
    object_wrapper->prop_prefix = tk_str_copy(object_wrapper->prop_prefix, prefix);
  }

  return RET_OK;
}

static const view_model_vtable_t s_view_model_vtable = {
    .create_sub_view_model = view_model_object_create_sub_view_model,
    .create_sub_view_model_array = view_model_object_create_sub_view_model_array,
    .on_will_mount = view_model_array_object_wrapper_on_will_mount};

static const object_vtable_t s_object_vtable = {
    .type = "view_model_array_object_wrapper",
    .desc = "view_model_array_object_wrapper",
    .size = sizeof(view_model_array_object_wrapper_t),
    .is_collection = TRUE,
    .on_destroy = view_model_array_object_wrapper_on_destroy,
    .compare = view_model_array_object_wrapper_compare,
    .get_prop = view_model_array_object_wrapper_get_prop,
    .set_prop = view_model_array_object_wrapper_set_prop,
    .can_exec = view_model_array_object_wrapper_can_exec,
    .exec = view_model_array_object_wrapper_exec};

view_model_t* view_model_array_object_wrapper_create_ex(object_t* obj, const char* prop_prefix) {
  object_t* model = object_create(&s_object_vtable);
  view_model_t* view_model = VIEW_MODEL(model);
  view_model_t* view_model_array = view_model_array_init(VIEW_MODEL(model));
  view_model_array_object_wrapper_t* object_wrapper = VIEW_MODEL_ARRAY_OBJECT_WRAPPPER(model);
  return_value_if_fail(object_wrapper != NULL && obj != NULL, NULL);

  view_model->vt = &s_view_model_vtable;
  object_wrapper->obj = object_ref(obj);
  object_wrapper->prop_prefix = tk_strdup(prop_prefix);
  emitter_on(EMITTER(obj), EVT_PROP_CHANGED, view_model_array_object_wrapper_on_changed, model);
  emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, view_model_array_object_wrapper_on_changed, model);
  emitter_on(EMITTER(obj), EVT_ITEMS_CHANGED, view_model_array_object_wrapper_on_changed, model);

  return view_model;
}

view_model_t* view_model_array_object_wrapper_create(object_t* obj) {
  return view_model_array_object_wrapper_create_ex(obj, NULL);
}
