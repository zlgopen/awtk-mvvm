/**
 * File:   navigator.c
 * Author: AWTK Develop Team
 * Brief:  navigator navigator
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/navigator.h"

static ret_t navigator_on_destroy(tk_object_t* obj) {
  navigator_t* nav = NAVIGATOR(obj);

  tk_object_unref(TK_OBJECT(nav->handlers));

  return RET_OK;
}

static const object_vtable_t s_navigator_vtable = {.type = "navigator",
                                                   .desc = "navigator",
                                                   .size = sizeof(navigator_t),
                                                   .is_collection = FALSE,
                                                   .on_destroy = navigator_on_destroy};

navigator_t* navigator_create(void) {
  tk_object_t* obj = NULL;
  navigator_t* nav = NULL;

  obj = tk_object_create(&s_navigator_vtable);
  nav = NAVIGATOR(obj);
  return_value_if_fail(obj != NULL, NULL);

  nav->handlers = object_default_create();
  if (nav->handlers == NULL) {
    tk_object_unref(obj);
    obj = NULL;
  }

  return nav;
}

static navigator_handler_t* navigator_find_handler(navigator_t* nav, const char* target) {
  navigator_handler_t* handler = NULL;
  return_value_if_fail(nav != NULL && target != NULL, NULL);
  handler = (navigator_handler_t*)tk_object_get_prop_object(nav->handlers, target);
  if (handler == NULL) {
    handler =
        (navigator_handler_t*)tk_object_get_prop_object(nav->handlers, NAVIGATOR_DEFAULT_HANDLER);
  }

  return handler;
}

ret_t navigator_handle_request(navigator_t* nav, navigator_request_t* req) {
  navigator_handler_t* handler = NULL;
  const char* request_type = NULL;
  return_value_if_fail(nav != NULL && req != NULL, RET_BAD_PARAMS);

  request_type = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ);
  if (request_type != NULL) {
    handler = navigator_find_handler(nav, request_type);
  }

  if (handler == NULL) {
    handler = navigator_find_handler(nav, NAVIGATOR_DEFAULT_HANDLER);
  }

  if (handler == NULL) {
    log_warn("not found %s\n", request_type);
  }
  return_value_if_fail(handler != NULL, RET_NOT_FOUND);

  return navigator_handler_on_request(handler, req);
}

ret_t navigator_register_handler(navigator_t* nav, const char* name, navigator_handler_t* handler) {
  ret_t ret = RET_OK;
  return_value_if_fail(nav != NULL && name != NULL && handler != NULL, RET_BAD_PARAMS);

  ret = tk_object_set_prop_object(nav->handlers, name, TK_OBJECT(handler));
  assert(TK_OBJECT(handler)->ref_count > 1);
  tk_object_unref(TK_OBJECT(handler));

  log_debug("navigator_register_handler ret=%d : %s\n", ret, name);

  return ret;
}

bool_t navigator_has_handler(navigator_t* nav, const char* name) {
  return_value_if_fail(nav != NULL && name != NULL, RET_BAD_PARAMS);

  return tk_object_get_prop_object(nav->handlers, name) != NULL;
}

ret_t navigator_unregister_handler(navigator_t* nav, const char* name) {
  return_value_if_fail(nav != NULL && name != NULL, RET_BAD_PARAMS);

  tk_object_remove_prop(TK_OBJECT(nav->handlers), name);

  return RET_OK;
}

static navigator_t* s_navigator;
ret_t navigator_set(navigator_t* nav) {
  s_navigator = nav;

  return RET_OK;
}

navigator_t* navigator(void) {
  return s_navigator;
}

ret_t navigator_to(const char* args) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(args != NULL && navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(args, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_to_by_object(tk_object_t* args) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(args != NULL && navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  navigator_request_set_args(req, args);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_to_with_model(const char* target, tk_object_t* model, const char* prefix) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(target != NULL && navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_object(TK_OBJECT(req), NAVIGATOR_ARG_MODEL, model);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_PREFIX, prefix);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_to_with_key_value(const char* target, const char* key, const char* value) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(target != NULL && navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_str(TK_OBJECT(req), key, value);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_replace(const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(target != NULL && navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, TRUE);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_switch_to(const char* target, bool_t close_current) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(target != NULL && navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, close_current);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_back_to_home(void) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_HOME);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_back(void) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_BACK);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_toast(const char* content, uint32_t timeout) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(content != NULL, RET_BAD_PARAMS);
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_TOAST);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT, content);
  tk_object_set_prop_int(TK_OBJECT(req), NAVIGATOR_ARG_DURATION, timeout);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_info(const char* title, const char* content) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL && content != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_INFO);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE, title);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT, content);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_warn(const char* title, const char* content) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL && content != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_WARN);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE, title);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT, content);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_confirm(const char* title, const char* content) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL && content != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_CONFIRM);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE, title);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT, content);

  ret = navigator_handle_request(navigator(), req);
  if (ret == RET_OK) {
    ret = (ret_t)value_int(&(req->result));
  }

  tk_object_unref(TK_OBJECT(req));

  return ret;
}

static ret_t on_str_result(navigator_request_t* req, const value_t* result) {
  str_t* str = (str_t*)(req->user_data);
  return_value_if_fail(str != NULL && result != NULL, RET_BAD_PARAMS);

  return str_set(str, value_str(result));
}

ret_t navigator_pick_dir(const char* title, str_t* result) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL && result != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, on_str_result);
  return_value_if_fail(req != NULL, RET_OOM);

  req->user_data = result;
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_PICK_DIR);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE, title);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT, result->str);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_pick_file(const char* title, const char* filter, bool_t for_save, str_t* result) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL && result != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, on_str_result);
  return_value_if_fail(req != NULL, RET_OOM);

  req->user_data = result;
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_PICK_FILE);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE, title);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_FOR_SAVE, for_save);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT, result->str);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_FILTER, filter);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_pick_color(const char* title, str_t* result) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL && result != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, on_str_result);
  return_value_if_fail(req != NULL, RET_OOM);

  req->user_data = result;
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_PICK_COLOR);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE, title);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT, result->str);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_close(const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, on_str_result);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_CLOSE);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_FORCE, TRUE);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_request_close(const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, on_str_result);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_CLOSE);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

int32_t navigator_count_view_models(const char* target) {
  int32_t cnt = 0;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, 0);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, 0);

  req->user_data = &cnt;
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_COUNT_VIEW_MODEL);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  if (navigator_handle_request(navigator(), req) == RET_OK) {
    cnt = value_int(&(req->result));
  }
  tk_object_unref(TK_OBJECT(req));

  return cnt;
}

ret_t navigator_get_view_models(const char* target, darray_t* result) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL && result != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  req->user_data = result;
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_GET_VIEW_MODEL);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_notify_view_props_changed(const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_NOTIFY_VIEW_MODEL);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_PROPS_CHANGED, TRUE);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_notify_view_items_changed(tk_object_t* items, const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_BAD_PARAMS);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_NOTIFY_VIEW_MODEL);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);
  tk_object_set_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_ITEMS_CHANGED, TRUE);
  tk_object_set_prop_object(TK_OBJECT(req), NAVIGATOR_ARG_EVENT_SOURCE, items);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

tk_object_t* navigator_get_locale(const char* target) {
  tk_object_t* obj = NULL;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, NULL);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_GET_LOCALE);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  if (navigator_handle_request(navigator(), req) == RET_OK) {
    obj = value_object(&(req->result));
    tk_object_ref(obj);
  }
  tk_object_unref(TK_OBJECT(req));

  return obj;
}

ret_t navigator_set_locale(const char* language, const char* country, const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_FAIL);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_SET_LOCALE);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_LANGUAGE, language);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_COUNTRY, country);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

const char* navigator_get_theme(const char* target) {
  const char* theme = NULL;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, NULL);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, NULL);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_GET_THEME);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  if (navigator_handle_request(navigator(), req) == RET_OK) {
    theme = value_str(&(req->result));
  }
  tk_object_unref(TK_OBJECT(req));

  return theme;
}

ret_t navigator_set_theme(const char* theme, const char* target) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_FAIL);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_SET_THEME);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_THEME, theme);
  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET, target);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;
}

ret_t navigator_set_screen_saver_time(uint32_t time) {
  ret_t ret = RET_OK;
  navigator_request_t* req = NULL;
  return_value_if_fail(navigator() != NULL, RET_FAIL);

  req = navigator_request_create(NULL, NULL);
  return_value_if_fail(req != NULL, RET_OOM);

  tk_object_set_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_SET_SCREEN_SAVER_TIME);
  tk_object_set_prop_int(TK_OBJECT(req), NAVIGATOR_ARG_TIME, time);

  ret = navigator_handle_request(navigator(), req);
  tk_object_unref(TK_OBJECT(req));

  return ret;

}

ret_t navigator_to_ex(navigator_request_t* req) {
  return navigator_handle_request(navigator(), req);
}
