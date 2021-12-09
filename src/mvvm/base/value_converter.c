/**
 * File:   value_converter.c
 * Author: AWTK Develop Team
 * Brief:  value converter
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
 * 2019-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/slist.h"
#include "tkc/object_default.h"
#include "mvvm/base/value_converter.h"

ret_t value_converter_to_view(value_converter_t* converter, const value_t* from, value_t* to) {
  return_value_if_fail(converter != NULL && converter->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(converter->to_view != NULL && converter->object.ref_count > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(from != NULL && to != NULL, RET_BAD_PARAMS);

  return converter->to_view(converter, from, to);
}

ret_t value_converter_to_model(value_converter_t* converter, const value_t* from, value_t* to) {
  return_value_if_fail(converter != NULL && converter->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(converter->to_model != NULL && converter->object.ref_count > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(from != NULL && to != NULL, RET_BAD_PARAMS);

  return converter->to_model(converter, from, to);
}

typedef struct _value_converter_factory_t {
  tk_object_t* cache;
  tk_object_t* creators;
  slist_t generic_creators;
} value_converter_factory_t;

static value_converter_factory_t* s_converter_factory;

static value_converter_factory_t* value_converter_factory_create(void) {
  value_converter_factory_t* factory = TKMEM_ZALLOC(value_converter_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  factory->cache = object_default_create();
  factory->creators = object_default_create();
  slist_init(&(factory->generic_creators), NULL, NULL);

  return factory;
}

static ret_t value_converter_factory_destroy(value_converter_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  tk_object_unref(factory->cache);
  tk_object_unref(factory->creators);
  slist_deinit(&(factory->generic_creators));
  TKMEM_FREE(factory);

  return RET_OK;
}

static value_converter_t* value_converter_generic_create(const char* name) {
  slist_node_t* iter = s_converter_factory->generic_creators.first;
  while (iter != NULL) {
    value_converter_create_t create = (value_converter_create_t)iter->data;
    value_converter_t* c = create(name);
    if (c != NULL) {
      return c;
    }
    iter = iter->next;
  }

  return NULL;
}

value_converter_t* value_converter_do_create(const char* name) {
  tk_create_t create = NULL;
  return_value_if_fail(name != NULL && s_converter_factory != NULL, NULL);

  create = (tk_create_t)tk_object_get_prop_pointer(s_converter_factory->creators, name);
  if (create != NULL) {
    return (value_converter_t*)create();
  } else {
    return value_converter_generic_create(name);
  }
}

static value_converter_t* value_converter_get(const char* name) {
  tk_object_t* obj = tk_object_get_prop_object(s_converter_factory->cache, name);

  if (obj != NULL) {
    tk_object_ref(obj);
  }

  return VALUE_CONVERTER(obj);
}

static ret_t value_converter_put(const char* name, value_converter_t* c) {
  return_value_if_fail(name != NULL && c != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop_object(s_converter_factory->cache, name, TK_OBJECT(c));
}

value_converter_t* value_converter_create(const char* name) {
  value_converter_t* c = NULL;
  return_value_if_fail(name != NULL, NULL);

  c = value_converter_get(name);
  if (c != NULL) {
    return c;
  }

  c = value_converter_do_create(name);
  if (c != NULL) {
    value_converter_put(name, c);
  }

  return c;
}

ret_t value_converter_register(const char* name, tk_create_t create) {
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(create != NULL && s_converter_factory != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop_pointer(s_converter_factory->creators, name, create);
}

ret_t value_converter_register_generic(value_converter_create_t create) {
  return_value_if_fail(create != NULL && s_converter_factory != NULL, RET_BAD_PARAMS);

  return slist_append(&(s_converter_factory->generic_creators), create);
}

ret_t value_converter_init(void) {
  if (s_converter_factory == NULL) {
    s_converter_factory = value_converter_factory_create();
  }

  return RET_OK;
}

ret_t value_converter_deinit(void) {
  return_value_if_fail(s_converter_factory != NULL, RET_BAD_PARAMS);

  value_converter_factory_destroy(s_converter_factory);
  s_converter_factory = NULL;

  return RET_OK;
}
