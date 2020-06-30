/**
 * File:   view_model_factory.c
 * Author: AWTK Develop Team
 * Brief:  view_model factory
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_compositor.h"

static view_model_factory_t* s_model_factory;

ret_t view_model_factory_init(void) {
  if (s_model_factory == NULL) {
    s_model_factory = TKMEM_ZALLOC(view_model_factory_t);
    return_value_if_fail(s_model_factory != NULL, RET_OOM);

    s_model_factory->creators = object_default_create();
    if (s_model_factory->creators == NULL) {
      TKMEM_FREE(s_model_factory);
      s_model_factory = NULL;
    }
  }

  return s_model_factory != NULL ? RET_OK : RET_FAIL;
}

bool_t view_model_factory_exist(const char* type) {
  return_value_if_fail(s_model_factory != NULL && type != NULL, FALSE);

  return object_has_prop((s_model_factory->creators), type);
}

ret_t view_model_factory_unregister(const char* type) {
  return_value_if_fail(s_model_factory != NULL && type != NULL, RET_BAD_PARAMS);

  return object_remove_prop((s_model_factory->creators), type);
}

ret_t view_model_factory_register(const char* type, view_model_create_t create) {
  return_value_if_fail(s_model_factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  return object_set_prop_pointer(s_model_factory->creators, type, create);
}

view_model_t* view_model_factory_create_model_one(const char* type, navigator_request_t* req) {
  view_model_create_t create = NULL;
  return_value_if_fail(s_model_factory != NULL && type != NULL && req != NULL, NULL);
  create = (view_model_create_t)object_get_prop_pointer(s_model_factory->creators, type);
  if (create != NULL) {
    return create(req);
  } else {
    return NULL;
  }
}

#define IS_COMPOSITOR_VIEW_MODEL(type) strchr(type, '+') != NULL

view_model_t* view_model_factory_create_model(const char* type, navigator_request_t* req) {
  return_value_if_fail(s_model_factory != NULL && type != NULL && req != NULL, NULL);

  if (IS_COMPOSITOR_VIEW_MODEL(type)) {
    tokenizer_t t;
    view_model_t* compositor = view_model_compositor_create(req);
    return_value_if_fail(compositor != NULL, NULL);

    tokenizer_init(&t, type, strlen(type), "+");
    while (tokenizer_has_more(&t)) {
      const char* type1 = tokenizer_next(&t);
      view_model_t* vm = view_model_factory_create_model_one(type1, req);
      if (vm != NULL) {
        if (view_model_compositor_add(compositor, vm) != RET_OK) {
          log_warn("view_model_compositor_add failed\n");
          OBJECT_UNREF(vm);
        }
      } else {
        log_warn("create %s view_model failed\n", type1);
      }
    }
    tokenizer_deinit(&t);

    return compositor;
  } else {
    return view_model_factory_create_model_one(type, req);
  }
}

ret_t view_model_factory_deinit(void) {
  return_value_if_fail(s_model_factory != NULL && s_model_factory->creators != NULL,
                       RET_BAD_PARAMS);

  object_unref(s_model_factory->creators);
  TKMEM_FREE(s_model_factory);

  s_model_factory = NULL;

  return RET_OK;
}
