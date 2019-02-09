/**
 * File:   navigator_handler.h
 * Author: AWTK Develop Team
 * Brief:  navigator_handler awtk global functions.
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/base/binding_context.h"
#include "mvvm/awtk/navigator_handler_awtk.h"

static const object_vtable_t s_navigator_handler_awtk_vtable = {
    .type = "navigator_handler_awtk",
    .desc = "navigator_handler_awtk",
    .size = sizeof(navigator_handler_awtk_t),
    .is_collection = FALSE,
};

static ret_t navigator_handler_awtk_on_request(navigator_handler_t* handler,
                                               navigator_request_t* req) {
  model_t* model = NULL;
  navigator_handler_awtk_t* handler_awtk = NAVIGATOR_HANDLER_AWTK(handler);

  if (handler_awtk->model_create != NULL) {
    model = handler_awtk->model_create(handler_awtk->model_create_args);
    return_value_if_fail(model != NULL, RET_FAIL);
  }

  return vm_open_window(req->target, model, req);
}

navigator_handler_t* navigator_handler_awtk_create(model_create_t model_create,
                                                   void* model_create_args) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;
  navigator_handler_awtk_t* handler_awtk = NULL;
  return_value_if_fail(model_create != NULL, NULL);

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  handler_awtk = NAVIGATOR_HANDLER_AWTK(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler_awtk->model_create = model_create;
  handler_awtk->model_create_args = model_create_args;
  handler->on_request = navigator_handler_awtk_on_request;

  return handler;
}

navigator_handler_t* navigator_handler_awtk_default_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_request;

  return handler;
}
