/**
 * File:   navigator_handler.h
 * Author: AWTK Develop Team
 * Brief:  navigator_handler awtk global functions.
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

#include "base/dialog.h"
#include "base/window_manager.h"
#include "file_browser/file_chooser.h"
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
  if (!(req->open_new)) {
    widget_t* wm = window_manager();
    widget_t* target_win = widget_child(wm, req->target);

    if (target_win != NULL) {
      widget_t* curr_win = window_manager_get_top_window(wm);
      return window_manager_switch_to(wm, curr_win, target_win, req->close_current);
    }
  }

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

static ret_t navigator_handler_awtk_on_info(navigator_handler_t* handler,
                                            navigator_request_t* req) {
  const char* title = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE);
  const char* content = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT);

  return dialog_info(title, content);
}

navigator_handler_t* navigator_handler_awtk_info_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_info;

  return handler;
}

static ret_t navigator_handler_awtk_on_warn(navigator_handler_t* handler,
                                            navigator_request_t* req) {
  const char* title = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE);
  const char* content = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT);

  return dialog_warn(title, content);
}

navigator_handler_t* navigator_handler_awtk_warn_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_warn;

  return handler;
}

static ret_t navigator_handler_awtk_on_confirm(navigator_handler_t* handler,
                                               navigator_request_t* req) {
  const char* title = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE);
  const char* content = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_CONTENT);

  return dialog_confirm(title, content);
}

navigator_handler_t* navigator_handler_awtk_confirm_create(void) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_confirm;

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

static ret_t tk_on_choose_file_result(void* ctx, event_t* e) {
  navigator_request_t* req = (navigator_request_t*)ctx;
  file_chooser_t* chooser = (file_chooser_t*)(e->target);

  if (!file_chooser_is_aborted(chooser)) {
    value_t v;
    str_t str;
    const char* filename = file_chooser_get_filename(chooser);

    str_init(&str, MAX_PATH);
    str_append(&str, file_chooser_get_dir(chooser));
    if (filename != NULL && *filename) {
      str_append_char(&str, TK_PATH_SEP);
      str_append(&str, filename);
    }
    value_set_str(&v, str.str);
    navigator_request_on_result(req, &v);
    str_reset(&str);
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_on_pick_file(navigator_handler_t* handler,
                                                 navigator_request_t* req) {
  const char* defpath = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT);
  const char* mimetype = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_FILTER);
  bool_t for_save = object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_FOR_SAVE, FALSE);
  file_chooser_t* chooser = file_chooser_create();

  file_chooser_set_init_dir(chooser, defpath);
  file_chooser_set_filter(chooser, mimetype);
  emitter_on(EMITTER(chooser), EVT_DONE, tk_on_choose_file_result, req);

  if (for_save) {
    return file_chooser_choose_file_for_save(chooser);
  } else {
    return file_chooser_choose_file_for_open(chooser);
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
  const char* defpath = object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_DEFAULT);
  file_chooser_t* chooser = file_chooser_create();
  file_chooser_set_init_dir(chooser, defpath);
  emitter_on(EMITTER(chooser), EVT_DONE, tk_on_choose_file_result, req);

  return file_chooser_choose_folder(chooser);
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
