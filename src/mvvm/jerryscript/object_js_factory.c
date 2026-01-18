/**
 * File:   object_js_factory.c
 * Author: AWTK Develop Team
 * Brief:  object jerryscript factory
 *
 * Copyright (c) 2020 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-02-24 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/jerryscript/object_js_factory.h"
#include "mvvm/jerryscript/object_js_default.h"

static object_js_factory_t* object_js_factory_init(object_js_factory_t* factory);
static ret_t object_js_factory_deinit(object_js_factory_t* factory);

static object_js_factory_t* s_object_js_factory = NULL;

typedef struct _creator_item_t {
  jsobj_type_t type;
  object_js_create_t create;
} creator_item_t;

static int32_t creator_item_cmp(const creator_item_t* iter, jsobj_type_t type) {
  return iter->type - type;
}

object_js_factory_t* object_js_factory(void) {
  return s_object_js_factory;
}

object_js_factory_t* object_js_factory_create(void) {
  object_js_factory_t* factory = TKMEM_ZALLOC(object_js_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  return object_js_factory_init(factory);
}

static object_js_factory_t* object_js_factory_init(object_js_factory_t* factory) {
  return_value_if_fail(factory != NULL, NULL);

  darray_init(&(factory->creators), 0, default_destroy, (tk_compare_t)creator_item_cmp);

  return factory;
}

ret_t object_js_factory_register(object_js_factory_t* factory, jsobj_type_t type,
                                 object_js_create_t create) {
  creator_item_t* item = NULL;
  return_value_if_fail(factory != NULL && type != JERRY_OBJECT_TYPE_NONE && create != NULL,
                       RET_BAD_PARAMS);

  item = TKMEM_ZALLOC(creator_item_t);
  return_value_if_fail(item != NULL, RET_OOM);

  item->create = create;
  item->type = type;
  darray_push(&(factory->creators), item);

  return RET_OK;
}

tk_object_t* object_js_factory_create_object(object_js_factory_t* factory, jsvalue_t jsobj,
                                             bool_t free_handle) {
  jsobj_type_t type = jerry_object_get_type(jsobj);
  const creator_item_t* iter = NULL;
  return_value_if_fail(factory != NULL && type != JERRY_OBJECT_TYPE_NONE, NULL);

  iter = darray_find(&(factory->creators), (void*)type);
  if (iter != NULL) {
    return iter->create(jsobj, free_handle);
  } else {
    return object_js_default_create(jsobj, free_handle);
  }
}

ret_t object_js_factory_set(object_js_factory_t* factory) {
  s_object_js_factory = factory;

  return RET_OK;
}

static ret_t object_js_factory_deinit(object_js_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  darray_deinit(&(factory->creators));

  return RET_OK;
}

ret_t object_js_factory_destroy(object_js_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  object_js_factory_deinit(factory);
  TKMEM_FREE(factory);

  return RET_OK;
}
