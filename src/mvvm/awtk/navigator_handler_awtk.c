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

#include "base/dialog.h"
#include "base/window_manager.h"
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
  const char* content = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT);
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

static ret_t navigator_handler_awtk_on_home(navigator_handler_t* handler,
                                            navigator_request_t* req) {
  window_manager_back_to_home(window_manager());
  return RET_OK;
}

navigator_handler_t* navigator_handler_awtk_home_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_home;

  return handler;
}

static ret_t navigator_handler_awtk_on_back(navigator_handler_t* handler,
                                            navigator_request_t* req) {
  window_manager_back(window_manager());
  return RET_OK;
}

navigator_handler_t* navigator_handler_awtk_back_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_back;

  return handler;
}

#ifdef WITH_NATIVE_FILE_DIALOG

#include <nfd.h>

static ret_t navigator_handler_awtk_on_pick_file(navigator_handler_t* handler,
                                                 navigator_request_t* req) {
  nfdchar_t* outPath = NULL;
  nfdresult_t result = NFD_OKAY;
  const char* defpath = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT);
  const char* mimetype = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_MINE_TYPES);
  bool_t for_save = object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_FOR_SAVE, FALSE);

  if (for_save) {
    result = NFD_SaveDialog(NULL, defpath, &outPath);
  } else {
    result = NFD_OpenDialog(NULL, defpath, &outPath);
  }

  if (result == NFD_OKAY) {
    value_t v;
    value_set_str(&v, outPath);
    navigator_request_on_result(req, &v);
    free(outPath);
    return RET_OK;
  } else if (result == NFD_CANCEL) {
    log_debug("User pressed cancel.");
    return RET_FAIL;
  } else {
    log_debug("Error: %s\n", NFD_GetError());
    return RET_FAIL;
  }
}

navigator_handler_t* navigator_handler_awtk_pick_file_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_pick_file;

  return handler;
}

static ret_t navigator_handler_awtk_on_pick_dir(navigator_handler_t* handler,
                                                navigator_request_t* req) {
  nfdchar_t* outPath = NULL;
  const char* defpath = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT);
  nfdresult_t result = NFD_PickFolder(defpath, &outPath);

  if (result == NFD_OKAY) {
    value_t v;
    value_set_str(&v, outPath);
    navigator_request_on_result(req, &v);
    free(outPath);
    return RET_OK;
  } else if (result == NFD_CANCEL) {
    log_debug("User pressed cancel.");
    return RET_FAIL;
  } else {
    log_debug("Error: %s\n", NFD_GetError());
    return RET_FAIL;
  }

  return RET_OK;
}

navigator_handler_t* navigator_handler_awtk_pick_dir_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_pick_dir;

  return handler;
}
#endif /*WITH_NATIVE_FILE_DIALOG*/
