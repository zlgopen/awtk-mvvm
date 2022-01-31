/**
 * File:   navigator_request.c
 * Author: AWTK Develop Team
 * Brief:  navigator request
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/mem.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/navigator_request.h"

static ret_t navigator_request_on_destroy(tk_object_t* obj) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);

  value_reset(&(req->result));
  TK_OBJECT_UNREF(req->args);

  return RET_OK;
}

static int32_t navigator_request_compare(tk_object_t* obj, tk_object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t navigator_request_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (req->args == NULL) {
    req->args = object_default_create();
  }

  return tk_object_set_prop(req->args, name, v);
}

static ret_t navigator_request_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return tk_object_get_prop(req->args, name, v);
}

static ret_t object_default_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);
  return_value_if_fail(obj != NULL && on_prop != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(req->args, on_prop, ctx);
}

static const object_vtable_t s_navigator_request_vtable = {
    .type = "navigator_request",
    .desc = "navigator_request",
    .size = sizeof(navigator_request_t),
    .is_collection = FALSE,
    .on_destroy = navigator_request_on_destroy,
    .compare = navigator_request_compare,
    .get_prop = navigator_request_get_prop,
    .set_prop = navigator_request_set_prop,
    .foreach_prop = object_default_foreach_prop};

navigator_request_t* navigator_request_create(const char* args,
                                              navigator_request_on_result_t on_result) {
  tk_object_t* obj = tk_object_create(&s_navigator_request_vtable);
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);

  req->on_result = on_result;

  if (args != NULL && *args != '\0') {
    req->args = object_default_create();
    if (req->args != NULL) {
      if (tk_str_start_with(args, COMMAND_ARGS_STRING_PREFIX)) {
        tk_command_arguments_to_object(args, req->args);
      } else {
        tk_object_set_prop_str(req->args, NAVIGATOR_ARG_TARGET, args);
      }
    } else {
      TK_OBJECT_UNREF(obj);
      req = NULL;
    }
  }

  return req;
}

ret_t navigator_request_on_result(navigator_request_t* req, const value_t* result) {
  return_value_if_fail(req != NULL && result != NULL, RET_BAD_PARAMS);

  if (req->on_result != NULL) {
    req->on_result(req, result);
  }

  return RET_OK;
}

ret_t navigator_request_set_args(navigator_request_t* req, tk_object_t* args) {
  return_value_if_fail(req != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(req->args);

  if (args != NULL) {
    req->args = TK_OBJECT_REF(args);
  }

  return RET_OK;
}
