/**
 * File:   custom_binder.c
 * Author: AWTK Develop Team
 * Brief:  custom_binder
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
 * 2020-07-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/darray.h"
#include "mvvm/base/custom_binder.h"

static custom_binder_t* s_custom_binder;
static darray_t* s_wildcard_binder;

typedef struct _wildcard_binder_t {
  char* pattern;
  binding_context_bind_rule_t bind;
} wildcard_binder_t;

static int wildcard_binder_compare(const void* a, const void* b) {
  wildcard_binder_t* wb = (wildcard_binder_t*)a;
  const char* pattern = (const char*)b;
  return_value_if_fail(wb != NULL && wb->pattern != NULL && pattern != NULL, -1);

  return tk_str_cmp(wb->pattern, pattern);
}

static ret_t wildcard_binder_destroy(void* data) {
  wildcard_binder_t* wb = (wildcard_binder_t*)data;
  if (wb != NULL) {
    TKMEM_FREE(wb->pattern);
    TKMEM_FREE(wb);
  }

  return RET_OK;
}

static bool_t custom_binder_is_wildcard(const char* type) {
  return type != NULL && (strchr(type, '*') != NULL || strchr(type, '?') != NULL);
}

static ret_t custom_binder_register_wildcard(const char* pattern,
                                             binding_context_bind_rule_t bind) {
  wildcard_binder_t* wb = NULL;
  if (s_wildcard_binder == NULL) {
    s_wildcard_binder = darray_create(5, wildcard_binder_destroy, wildcard_binder_compare);
  }
  return_value_if_fail(s_wildcard_binder != NULL, RET_OOM);

  wb = (wildcard_binder_t*)darray_find(s_wildcard_binder, (void*)pattern);
  if (wb != NULL) {
    wb->bind = bind;
    return RET_OK;
  }

  wb = TKMEM_ZALLOC(wildcard_binder_t);
  return_value_if_fail(wb != NULL, RET_OOM);

  wb->pattern = tk_str_copy(NULL, pattern);
  if (wb->pattern == NULL) {
    TKMEM_FREE(wb);
    return RET_OOM;
  }
  wb->bind = bind;

  if (darray_push(s_wildcard_binder, wb) != RET_OK) {
    TKMEM_FREE(wb->pattern);
    TKMEM_FREE(wb);
    return RET_OOM;
  }

  return RET_OK;
}

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

  if (tk_object_has_prop((s_custom_binder->binders), type)) {
    return TRUE;
  }

  if (s_wildcard_binder != NULL) {
    uint32_t i = 0;
    for (i = 0; i < s_wildcard_binder->size; i++) {
      wildcard_binder_t* wb = (wildcard_binder_t*)darray_get(s_wildcard_binder, i);
      if (wb != NULL && tk_wild_card_match(wb->pattern, type)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

ret_t custom_binder_unregister(const char* type) {
  return_value_if_fail(s_custom_binder != NULL && type != NULL, RET_BAD_PARAMS);

  if (custom_binder_is_wildcard(type)) {
    if (s_wildcard_binder != NULL) {
      return darray_remove(s_wildcard_binder, (void*)type);
    }
    return RET_NOT_FOUND;
  }

  return tk_object_remove_prop((s_custom_binder->binders), type);
}

ret_t custom_binder_register(const char* type, binding_context_bind_rule_t bind) {
  return_value_if_fail(s_custom_binder != NULL && type != NULL && bind != NULL, RET_BAD_PARAMS);

  if (custom_binder_is_wildcard(type)) {
    return custom_binder_register_wildcard(type, bind);
  }

  return tk_object_set_prop_pointer(s_custom_binder->binders, type, bind);
}

ret_t custom_binder_bind(const char* type, binding_context_t* ctx, binding_rule_t* rule) {
  binding_context_bind_rule_t bind = NULL;
  return_value_if_fail(s_custom_binder != NULL && type != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  bind = (binding_context_bind_rule_t)tk_object_get_prop_pointer(s_custom_binder->binders, type);
  if (bind != NULL) {
    return bind(ctx, rule);
  }

  if (s_wildcard_binder != NULL) {
    uint32_t i = 0;
    for (i = 0; i < s_wildcard_binder->size; i++) {
      wildcard_binder_t* wb = (wildcard_binder_t*)darray_get(s_wildcard_binder, i);
      if (wb != NULL && tk_wild_card_match(wb->pattern, type)) {
        return wb->bind(ctx, rule);
      }
    }
  }

  return RET_NOT_FOUND;
}

ret_t custom_binder_deinit(void) {
  return_value_if_fail(s_custom_binder != NULL && s_custom_binder->binders != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(s_custom_binder->binders);
  TKMEM_FREE(s_custom_binder);
  s_custom_binder = NULL;

  if (s_wildcard_binder != NULL) {
    darray_destroy(s_wildcard_binder);
    s_wildcard_binder = NULL;
  }

  return RET_OK;
}
