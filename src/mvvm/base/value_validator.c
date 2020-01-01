/**
 * File:   value_validator.c
 * Author: AWTK Develop Team
 * Brief:  value validator
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
#include "mvvm/base/value_validator.h"

bool_t value_validator_is_valid(value_validator_t* validator, const value_t* value, str_t* msg) {
  return_value_if_fail(validator != NULL && validator->object.vt != NULL, FALSE);
  return_value_if_fail(validator->is_valid != NULL && validator->object.ref_count > 0, FALSE);
  return_value_if_fail(value != NULL && msg != NULL, FALSE);

  return validator->is_valid(validator, value, msg);
}

ret_t value_validator_fix(value_validator_t* validator, value_t* value) {
  return_value_if_fail(validator != NULL && validator->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL && validator->object.ref_count > 0, RET_BAD_PARAMS);

  if (validator->fix != NULL) {
    return validator->fix(validator, value);
  } else {
    return RET_OK;
  }
}

typedef struct _value_validator_factory_t {
  object_t* cache;
  object_t* creators;
  slist_t generic_creators;
} value_validator_factory_t;

static value_validator_factory_t* s_validator_factory;

static value_validator_factory_t* value_validator_factory_create(void) {
  value_validator_factory_t* factory = TKMEM_ZALLOC(value_validator_factory_t);
  return_value_if_fail(factory != NULL, NULL);

  factory->cache = object_default_create();
  factory->creators = object_default_create();
  slist_init(&(factory->generic_creators), NULL, NULL);

  return factory;
}

static ret_t value_validator_factory_destroy(value_validator_factory_t* factory) {
  return_value_if_fail(factory != NULL, RET_BAD_PARAMS);

  object_unref(factory->cache);
  object_unref(factory->creators);
  slist_deinit(&(factory->generic_creators));

  return RET_OK;
}

static value_validator_t* value_validator_generic_create(const char* name) {
  slist_node_t* iter = s_validator_factory->generic_creators.first;
  while (iter != NULL) {
    value_validator_create_t create = (value_validator_create_t)iter->data;
    value_validator_t* c = create(name);
    if (c != NULL) {
      return c;
    }
    iter = iter->next;
  }

  return NULL;
}

value_validator_t* value_validator_do_create(const char* name) {
  tk_create_t create = NULL;
  return_value_if_fail(name != NULL && s_validator_factory != NULL, NULL);

  create = (tk_create_t)object_get_prop_pointer(s_validator_factory->creators, name);
  if (create != NULL) {
    return (value_validator_t*)create();
  } else {
    return value_validator_generic_create(name);
  }
}

static value_validator_t* value_validator_get(const char* name) {
  object_t* obj = object_get_prop_object(s_validator_factory->cache, name);

  if (obj != NULL) {
    object_ref(obj);
  }

  return VALUE_VALIDATOR(obj);
}

static ret_t value_validator_put(const char* name, value_validator_t* c) {
  return_value_if_fail(name != NULL && c != NULL, RET_BAD_PARAMS);

  return object_set_prop_object(s_validator_factory->cache, name, OBJECT(c));
}

value_validator_t* value_validator_create(const char* name) {
  value_validator_t* c = NULL;
  return_value_if_fail(name != NULL, NULL);

  c = value_validator_get(name);
  if (c != NULL) {
    return c;
  }

  c = value_validator_do_create(name);
  if (c != NULL) {
    value_validator_put(name, c);
  }

  return c;
}

ret_t value_validator_set_context(value_validator_t* validator, object_t* context) {
  return_value_if_fail(validator != NULL, RET_BAD_PARAMS);
  if (validator->context != NULL) {
    object_unref(validator->context);
    validator->context = NULL;
  }

  if (context != NULL) {
    validator->context = context;
    object_ref(validator->context);
  }

  return RET_OK;
}

ret_t value_validator_register(const char* name, tk_create_t create) {
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(create != NULL && s_validator_factory != NULL, RET_BAD_PARAMS);

  return object_set_prop_pointer(s_validator_factory->creators, name, create);
}

ret_t value_validator_register_generic(value_validator_create_t create) {
  return_value_if_fail(create != NULL && s_validator_factory != NULL, RET_BAD_PARAMS);

  return slist_append(&(s_validator_factory->generic_creators), create);
}

ret_t value_validator_init(void) {
  if (s_validator_factory == NULL) {
    s_validator_factory = value_validator_factory_create();
  }

  return RET_OK;
}

ret_t value_validator_deinit(void) {
  return_value_if_fail(s_validator_factory != NULL, RET_BAD_PARAMS);

  value_validator_factory_destroy(s_validator_factory);
  s_validator_factory = NULL;

  return RET_OK;
}
