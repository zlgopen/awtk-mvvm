/**
 * File:   custom_binder.c
 * Author: AWTK Develop Team
 * Brief:  custom_binder
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-07-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/custom_binder.h"

static custom_binder_t* s_custom_binder;

ret_t custom_binder_init(void) {
  if (s_custom_binder == NULL) {
    s_custom_binder = TKMEM_ZALLOC(custom_binder_t);
    return_value_if_fail(s_custom_binder != NULL, RET_OOM);

    s_custom_binder->binders = object_default_create();
    if (s_custom_binder->binders == NULL) {
      TKMEM_FREE(s_custom_binder);
      s_custom_binder = NULL;
    }
  }

  return s_custom_binder != NULL ? RET_OK : RET_FAIL;
}

bool_t custom_binder_exist(const char* type) {
  return_value_if_fail(s_custom_binder != NULL && type != NULL, FALSE);

  return tk_object_has_prop((s_custom_binder->binders), type);
}

ret_t custom_binder_unregister(const char* type) {
  return_value_if_fail(s_custom_binder != NULL && type != NULL, RET_BAD_PARAMS);

  return tk_object_remove_prop((s_custom_binder->binders), type);
}

ret_t custom_binder_register(const char* type, binding_context_bind_rule_t bind) {
  return_value_if_fail(s_custom_binder != NULL && type != NULL && bind != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop_pointer(s_custom_binder->binders, type, bind);
}

ret_t custom_binder_bind(const char* type, binding_context_t* ctx, binding_rule_t* rule) {
  binding_context_bind_rule_t bind = NULL;
  return_value_if_fail(s_custom_binder != NULL && type != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  bind = (binding_context_bind_rule_t)tk_object_get_prop_pointer(s_custom_binder->binders, type);
  if (bind != NULL) {
    return bind(ctx, rule);
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t custom_binder_deinit(void) {
  return_value_if_fail(s_custom_binder != NULL && s_custom_binder->binders != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(s_custom_binder->binders);
  TKMEM_FREE(s_custom_binder);

  s_custom_binder = NULL;

  return RET_OK;
}
