/**
 * File:   model_array.c
 * Author: AWTK Develop Team
 * Brief:  model_array
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

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/model_array.h"

static ret_t model_array_on_destroy(object_t* obj) {
  model_array_t* array = MODEL_ARRAY(obj);

  darray_deinit(&(array->array));

  return RET_OK;
}

static int32_t model_array_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t model_array_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  model_t* submodel = NULL;
  model_array_t* array = MODEL_ARRAY(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < array->array.size, RET_BAD_PARAMS);
  submodel = MODEL(array->array.elms[index]);

  return object_set_prop(OBJECT(submodel), name, v);
}

static ret_t model_array_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  model_t* submodel = NULL;
  model_array_t* array = MODEL_ARRAY(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(MODEL_PROP_ITEMS, name)) {
    value_set_int(v, array->array.size);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < array->array.size, RET_BAD_PARAMS);
  submodel = MODEL(array->array.elms[index]);

  return object_get_prop(OBJECT(submodel), name, v);
}

static bool_t model_array_can_exec(object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL, FALSE);

  return FALSE;
}

static ret_t model_array_exec(object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return RET_NOT_IMPL;
  ;
}

static const object_vtable_t s_model_array_vtable = {.type = "model_array",
                                                     .desc = "model_array",
                                                     .size = sizeof(model_array_t),
                                                     .is_collection = TRUE,
                                                     .on_destroy = model_array_on_destroy,

                                                     .compare = model_array_compare,
                                                     .get_prop = model_array_get_prop,
                                                     .set_prop = model_array_set_prop,
                                                     .can_exec = model_array_can_exec,
                                                     .exec = model_array_exec};

model_t* model_array_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_model_array_vtable);
  model_array_t* array = MODEL_ARRAY(obj);
  return_value_if_fail(array != NULL, NULL);

  darray_init(&(array->array), 10, (tk_destroy_t)(object_unref), NULL);

  return MODEL(obj);
}

ret_t model_array_clear(model_t* model) {
  model_array_t* array = MODEL_ARRAY(model);
  return_value_if_fail(array != NULL, RET_BAD_PARAMS);
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  darray_clear(&(array->array));
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return RET_OK;
}

ret_t model_array_add(model_t* model, model_t* submodel) {
  ret_t ret = RET_OK;
  model_array_t* array = MODEL_ARRAY(model);
  return_value_if_fail(array != NULL && submodel != NULL, RET_BAD_PARAMS);

  object_ref(OBJECT(submodel));
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  ret = darray_push(&(array->array), submodel);
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return ret;
}

ret_t model_array_remove(model_t* model, uint32_t index) {
  ret_t ret = RET_OK;
  model_array_t* array = MODEL_ARRAY(model);
  return_value_if_fail(array != NULL && index < array->array.size, RET_BAD_PARAMS);

  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  ret = darray_remove_index(&(array->array), index);
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return ret;
}

model_t* model_array_get(model_t* model, uint32_t index) {
  ret_t ret = RET_OK;
  model_array_t* array = MODEL_ARRAY(model);
  return_value_if_fail(array != NULL && index < array->array.size, RET_BAD_PARAMS);

  return MODEL(array->array.elms[index]);
}

int32_t model_array_size(model_t* model) {
  ret_t ret = RET_OK;
  model_array_t* array = MODEL_ARRAY(model);
  return_value_if_fail(array != NULL, 0);

  return MODEL(array->array.size);
}
