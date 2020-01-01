/**
 * File:   device_factory.c
 * Author: AWTK Develop Team
 * Brief:  device_factory
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
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "device_factory.h"
#include "tkc/object_default.h"

static object_t* g_creators = NULL;

ret_t device_factory_init(void) {
  if (g_creators == NULL) {
    g_creators = object_default_create();
  } else {
    object_unref(g_creators);
  }

  return RET_OK;
}

bool_t device_factory_has(const char* type) {
  return_value_if_fail(type != NULL, RET_BAD_PARAMS);
  return_value_if_fail(g_creators != NULL, RET_BAD_PARAMS);

  return object_get_prop_pointer(g_creators, type) != NULL;
}

ret_t device_factory_unregister(const char* type) {
  return_value_if_fail(type != NULL, RET_BAD_PARAMS);
  return_value_if_fail(g_creators != NULL, RET_BAD_PARAMS);

  return object_remove_prop(g_creators, type);
}

object_t* device_factory_create_device(const char* type, const char* args) {
  device_object_create_t create = NULL;
  return_value_if_fail(type != NULL, NULL);
  return_value_if_fail(g_creators != NULL, NULL);

  create = (device_object_create_t)object_get_prop_pointer(g_creators, type);
  return_value_if_fail(create != NULL, NULL);

  return create(args);
}

ret_t device_factory_register(const char* type, device_object_create_t create) {
  return_value_if_fail(type != NULL, RET_BAD_PARAMS);
  return_value_if_fail(create != NULL, RET_BAD_PARAMS);
  return_value_if_fail(g_creators != NULL, RET_BAD_PARAMS);

  return object_set_prop_pointer(g_creators, type, create);
}

ret_t device_factory_deinit(void) {
  return_value_if_fail(g_creators != NULL, RET_BAD_PARAMS);

  if (g_creators->ref_count > 1) {
    object_unref(g_creators);
  } else if (g_creators->ref_count == 1) {
    object_unref(g_creators);
    g_creators = NULL;
  } else {
    g_creators = NULL;
  }

  return RET_OK;
}
