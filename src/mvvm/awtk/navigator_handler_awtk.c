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

extern ret_t awtk_open_window(navigator_request_t* req);

static const object_vtable_t s_navigator_handler_awtk_vtable = {
    .type = "navigator_handler_awtk",
    .desc = "navigator_handler_awtk",
    .size = sizeof(navigator_handler_awtk_t),
    .is_collection = FALSE,
};

static ret_t navigator_handler_awtk_on_request(navigator_handler_t* handler,
                                               navigator_request_t* req) {
  return awtk_open_window(req);
}

navigator_handler_t* navigator_handler_awtk_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_request;

  return handler;
}

static ret_t navigator_handler_awtk_on_toast(navigator_handler_t* handler,
                                               navigator_request_t* req) {
  const char* content = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT, NULL);
  int duration = object_get_prop_int(OBJECT(req), NAVIGATOR_ARG_DURATION, 3000);

  return dialog_toast(content, duration);
}

navigator_handler_t* navigator_handler_awtk_toast_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_toast;

  return handler;
}
