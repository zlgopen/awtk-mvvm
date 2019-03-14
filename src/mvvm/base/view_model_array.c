/**
 * File:   view_model_array.c
 * Author: AWTK Develop Team
 * Brief:  view_model_array
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/utils.h"
#include "mvvm/base/view_model_array.h"

static ret_t view_model_array_on_destroy(object_t* obj) {
  view_model_t* vm = VIEW_MODEL(obj);
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(vm);

  view_model_deinit(vm);
  str_reset(&(vm_array->temp_expr));
  str_reset(&(vm_array->temp_prop));

  return RET_OK;
}

static int32_t view_model_array_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_array_set_prop(object_t* obj, const char* name, const value_t* v) {
  view_model_t* vm = VIEW_MODEL(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(name, MODEL_PROP_CURSOR)) {
    VIEW_MODEL_ARRAY(vm)->cursor = value_int(v);

    return RET_OK;
  }

  return object_set_prop(OBJECT(vm->model), name, v);
}

static ret_t view_model_array_get_prop(object_t* obj, const char* name, value_t* v) {
  view_model_t* vm = VIEW_MODEL(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(name, MODEL_PROP_CURSOR)) {
    value_set_int(v, VIEW_MODEL_ARRAY(vm)->cursor);

    return RET_OK;
  }

  return object_get_prop(OBJECT(vm->model), name, v);
}

static bool_t view_model_array_can_exec(object_t* obj, const char* name, const char* args) {
  view_model_t* vm = VIEW_MODEL(obj);
  return_value_if_fail(obj != NULL && name != NULL, FALSE);

  return object_can_exec(OBJECT(vm->model), name, args);
}

static ret_t view_model_array_exec(object_t* obj, const char* name, const char* args) {
  view_model_t* vm = VIEW_MODEL(obj);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return object_exec(OBJECT(vm->model), name, args);
}

static const char* view_model_array_preprocess_expr(view_model_t* vm, const char* expr) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(vm);
  return_value_if_fail(vm != NULL && expr != NULL, NULL);

  str_set(&(vm_array->temp_expr), expr);
  str_replace(&(vm_array->temp_expr), "item.", "$item_");
  str_replace(&(vm_array->temp_expr), "$$", "$");

  return vm_array->temp_expr.str;
}

static const char* view_model_array_preprocess_prop(view_model_t* vm, const char* prop) {
  char index[TK_NUM_MAX_LEN + 1];
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(vm);
  return_value_if_fail(vm != NULL && prop != NULL, NULL);

  tk_snprintf(index, TK_NUM_MAX_LEN, "[%d].", vm_array->cursor);

  str_set(&(vm_array->temp_prop), prop);
  str_replace(&(vm_array->temp_prop), "item_", index);

  return vm_array->temp_prop.str;
}

static const object_vtable_t s_view_model_array_vtable = {.type = "view_model_array",
                                                          .desc = "view_model_array",
                                                          .size = sizeof(view_model_array_t),
                                                          .is_collection = TRUE,
                                                          .on_destroy = view_model_array_on_destroy,

                                                          .compare = view_model_array_compare,
                                                          .get_prop = view_model_array_get_prop,
                                                          .set_prop = view_model_array_set_prop,
                                                          .can_exec = view_model_array_can_exec,
                                                          .exec = view_model_array_exec};

static ret_t view_model_array_on_items_changed(void* ctx, event_t* e) {
  (void)e;

  emitter_dispatch_simple_event(EMITTER(ctx), EVT_ITEMS_CHANGED);

  return RET_OK;
}

view_model_t* view_model_array_create(model_t* model) {
  object_t* obj = NULL;
  view_model_t* vm = NULL;
  view_model_array_t* vm_array = NULL;

  return_value_if_fail(object_is_collection(OBJECT(model)), NULL);

  obj = object_create(&s_view_model_array_vtable);
  vm = view_model_init(VIEW_MODEL(obj), VIEW_MODEL_ARRAY, model);
  vm_array = VIEW_MODEL_ARRAY(vm);
  return_value_if_fail(vm_array != NULL, NULL);

  str_init(&(vm_array->temp_expr), 0);
  str_init(&(vm_array->temp_prop), 0);
  vm->preprocess_expr = view_model_array_preprocess_expr;
  vm->preprocess_prop = view_model_array_preprocess_prop;

  emitter_on(EMITTER(model), EVT_ITEMS_CHANGED, view_model_array_on_items_changed, vm);

  return vm;
}

ret_t view_model_array_inc_cursor(view_model_t* vm) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(vm);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->cursor++;

  return RET_OK;
}

ret_t view_model_array_set_cursor(view_model_t* vm, uint32_t cursor) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(vm);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->cursor = cursor;

  return RET_OK;
}
