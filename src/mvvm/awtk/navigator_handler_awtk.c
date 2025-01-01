/**
 * File:   navigator_handler.h
 * Author: AWTK Develop Team
 * Brief:  navigator_handler awtk global functions.
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "base/dialog.h"
#include "base/window_manager.h"
#include "ui_loader/ui_serializer.h"
#include "file_browser/file_chooser.h"
#include "mvvm/awtk/ui_loader_mvvm.h"
#include "mvvm/awtk/navigator_handler_awtk.h"

static widget_t* navigator_handler_awtk_lookup_widget_by_path(widget_t* widget, const char* path) {
  char* p = NULL;
  char temp[MAX_PATH + 1];
  const char* name = temp;
  widget_t* target = widget;
  uint32_t len = 0;
  uint32_t index = 0;
  return_value_if_fail(widget != NULL && path != NULL, NULL);

  len = strlen(path);
  return_value_if_fail(len <= MAX_PATH, NULL);

  memcpy(temp, path, len);
  temp[len] = '\0';

  do {
    p = strchr(name, '.');
    if (p != NULL) {
      *p = '\0';
    }

    if (*name == '[') {
      index = tk_atoi(name + 1);
      target = widget_get_child(target, index);
    } else {
      target = widget_lookup(target, name, FALSE);
    }

    if (p == NULL) {
      break;
    }

    name = p + 1;
  } while (p != NULL && target != NULL);

  return target;
}

static view_model_t* navigator_handler_awtk_get_view_model(widget_t* widget) {
  tk_object_t* props = widget->custom_props;

  if (props != NULL) {
    binding_context_t* bctx =
        BINDING_CONTEXT(tk_object_get_prop_pointer(props, WIDGET_PROP_V_MODEL));
    if (bctx != NULL) {
      return bctx->view_model;
    }
  }

  return NULL;
}

static ret_t navigator_handler_awtk_foreach_binding_context(widget_t* widget, tk_visit_t visit,
                                                            void* ctx) {
  ret_t ret = RET_OK;
  tk_object_t* props = widget->custom_props;

  if (props != NULL) {
    darray_t* children =
        (darray_t*)(tk_object_get_prop_pointer(props, WIDGET_PROP_V_MODEL_CHILDREN));
    binding_context_t* bctx =
        BINDING_CONTEXT(tk_object_get_prop_pointer(props, WIDGET_PROP_V_MODEL));

    if (bctx != NULL) {
      ret = visit(ctx, bctx);
      if (ret != RET_OK) {
        return ret;
      }
    }

    if (children != NULL) {
      uint32_t i = 0;
      for (i = 0; i < children->size; i++) {
        bctx = BINDING_CONTEXT(children->elms[i]);
        if (bctx != NULL) {
          ret = visit(ctx, bctx);
          if (ret != RET_OK) {
            return ret;
          }
        }
      }
    }
  }

  return RET_OK;
}

static ret_t visit_count_view_models(void* ctx, const void* data) {
  int32_t* result = (int32_t*)(ctx);
  binding_context_t* bctx = BINDING_CONTEXT(data);

  if (bctx->view_model != NULL) {
    *result = (*result) + 1;
  }

  return RET_OK;
}

static int32_t navigator_handler_awtk_count_view_models(widget_t* widget) {
  int32_t cnt = 0;
  navigator_handler_awtk_foreach_binding_context(widget, visit_count_view_models, &cnt);
  return cnt;
}

static ret_t visit_get_view_models(void* ctx, const void* data) {
  darray_t* result = (darray_t*)(ctx);
  binding_context_t* bctx = BINDING_CONTEXT(data);

  if (bctx->view_model != NULL) {
    darray_push(result, bctx->view_model);
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_get_view_models(widget_t* widget, darray_t* result) {
  return navigator_handler_awtk_foreach_binding_context(widget, visit_get_view_models, result);
}

static ret_t visit_notify_view_models_props_changed(void* ctx, const void* data) {
  binding_context_t* bctx = BINDING_CONTEXT(data);

  (void)ctx;

  if (bctx->view_model != NULL) {
    return view_model_notify_props_changed(bctx->view_model);
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_notify_view_models_props_changed(widget_t* widget) {
  return navigator_handler_awtk_foreach_binding_context(
      widget, visit_notify_view_models_props_changed, NULL);
}

static ret_t visit_notify_view_models_items_changed(void* ctx, const void* data) {
  binding_context_t* bctx = BINDING_CONTEXT(data);
  tk_object_t* items = TK_OBJECT(ctx);

  if (bctx->view_model != NULL) {
    return view_model_notify_items_changed(bctx->view_model, items);
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_notify_view_models_items_changed(widget_t* widget,
                                                                     tk_object_t* items) {
  return navigator_handler_awtk_foreach_binding_context(
      widget, visit_notify_view_models_items_changed, items);
}

static ret_t navigator_handler_awtk_on_window_open(void* ctx, event_t* e) {
  widget_t* to_close = WIDGET(ctx);
  widget_t* open = WIDGET(e->target);

  if (open != NULL && open->name != NULL) {
    log_debug("window %s open\n", open->name);
  }

  if (to_close != NULL) {
    log_debug("window %s close\n", to_close->name);
    window_manager_close_window_force(to_close->parent, to_close);
  }

  return RET_REMOVE;
}

static widget_t* navigator_handler_awtk_window_open_and_close(navigator_request_t* req,
                                                              widget_t* to_close) {
  widget_t* wm = NULL;
  widget_t* win = NULL;
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  return_value_if_fail(target != NULL, NULL);

  wm = window_manager();
  win = widget_child(wm, target);

  if (win != NULL) {
    bool_t single_instance = widget_get_prop_bool(win, WIDGET_PROP_SINGLE_INSTANCE, FALSE);
    if (single_instance) {
      window_manager_close_window_force(wm, win);
    }
  }

  win = ui_loader_mvvm_load_widget(req);
  if (win != NULL) {
    widget_on(win, EVT_WINDOW_OPEN, navigator_handler_awtk_on_window_open, to_close);
  }

  return win;
}

static ret_t navigator_handler_awtk_window_dump(widget_t* win) {
  value_t v;

  value_set_bool(&v, FALSE);
  if (widget_get_prop(win, "debug", &v) == RET_OK && value_bool(&v)) {
    str_t str;
    str_init(&str, 100000);
    widget_to_xml(win, &str);
    log_debug("%s\n", str.str);
    str_reset(&str);
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_window_open(navigator_request_t* req) {
  widget_t* win = NULL;
  widget_t* current = NULL;
  bool_t close_current =
      tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE);

  if (close_current) {
    current = window_manager_get_top_main_window(window_manager());
    if (current != NULL) {
      log_debug("close current window: %s\n", current->name);
    }
  }

  win = navigator_handler_awtk_window_open_and_close(req, current);
  return_value_if_fail(win != NULL, RET_NOT_FOUND);

  navigator_handler_awtk_window_dump(win);

  return RET_OK;
}

static const object_vtable_t s_navigator_handler_awtk_vtable = {
    .type = "navigator_handler_awtk",
    .desc = "navigator_handler_awtk",
    .size = sizeof(navigator_handler_awtk_t),
    .is_collection = FALSE,
};

static ret_t navigator_handler_awtk_on_request(navigator_handler_t* handler,
                                               navigator_request_t* req) {
  tk_object_t* obj = TK_OBJECT(req);
  const char* request_type = tk_object_get_prop_str(obj, NAVIGATOR_ARG_REQ);

  if (tk_str_ieq(request_type, NAVIGATOR_REQ_CLOSE)) {
    const char* target = tk_object_get_prop_str(obj, NAVIGATOR_ARG_TARGET);
    if (target != NULL) {
      widget_t* win = widget_child(window_manager(), target);
      if (win != NULL) {
        if (tk_object_get_prop_bool(obj, NAVIGATOR_ARG_FORCE, FALSE)) {
          return window_manager_close_window_force(window_manager(), win);
        } else {
          return widget_dispatch_simple_event(win, EVT_REQUEST_CLOSE_WINDOW);
        }
      } else {
        log_debug("%s is closed!\n", target);
        return RET_OK;
      }
    } else {
      log_debug("can't close without target.\n");
      return RET_FAIL;
    }
  } else if (tk_str_ieq(request_type, NAVIGATOR_REQ_BACK)) {
    return window_manager_back(window_manager());
  } else if (tk_str_ieq(request_type, NAVIGATOR_REQ_HOME)) {
    return window_manager_back_to_home(window_manager());
  }

  if (!tk_object_get_prop_bool(obj, NAVIGATOR_ARG_OPEN_NEW, TRUE)) {
    widget_t* wm = window_manager();
    const char* target = tk_object_get_prop_str(obj, NAVIGATOR_ARG_TARGET);

    if (target != NULL) {
      widget_t* target_win = widget_child(wm, target);

      if (target_win != NULL) {
        widget_t* curr_win = window_manager_get_top_window(wm);
        bool_t close_current = tk_object_get_prop_bool(obj, NAVIGATOR_ARG_CLOSE_CURRENT, FALSE);
        if (target_win != curr_win) {
          return window_manager_switch_to(wm, curr_win, target_win, close_current);
        } else {
          if (close_current) {
            return widget_dispatch_simple_event(curr_win, EVT_REQUEST_CLOSE_WINDOW);
          } else {
            return RET_OK;
          }
        }
      }
    }
  }

  return navigator_handler_awtk_window_open(req);
}

navigator_handler_t* navigator_handler_awtk_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_request;

  return handler;
}

static ret_t navigator_handler_awtk_on_toast(navigator_handler_t* handler,
                                             navigator_request_t* req) {
  const char* content = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT);
  int duration = tk_object_get_prop_int(TK_OBJECT(req), NAVIGATOR_ARG_DURATION, 3000);

  return dialog_toast(content, duration);
}

navigator_handler_t* navigator_handler_awtk_toast_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_toast;

  return handler;
}

static ret_t navigator_handler_awtk_on_info(navigator_handler_t* handler,
                                            navigator_request_t* req) {
  const char* title = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE);
  const char* content = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT);

  return dialog_info(title, content);
}

navigator_handler_t* navigator_handler_awtk_info_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_info;

  return handler;
}

static ret_t navigator_handler_awtk_on_warn(navigator_handler_t* handler,
                                            navigator_request_t* req) {
  const char* title = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE);
  const char* content = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT);

  return dialog_warn(title, content);
}

navigator_handler_t* navigator_handler_awtk_warn_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_warn;

  return handler;
}

static ret_t navigator_handler_awtk_on_confirm(navigator_handler_t* handler,
                                               navigator_request_t* req) {
  const char* title = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE);
  const char* content = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT);

  return dialog_confirm(title, content);
}

navigator_handler_t* navigator_handler_awtk_confirm_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_confirm;

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
  const char* defpath = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT);
  const char* mimetype = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_FILTER);
  bool_t for_save = tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_FOR_SAVE, FALSE);
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
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_pick_file;

  return handler;
}

static ret_t navigator_handler_awtk_on_pick_dir(navigator_handler_t* handler,
                                                navigator_request_t* req) {
  const char* defpath = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT);
  file_chooser_t* chooser = file_chooser_create();
  file_chooser_set_init_dir(chooser, defpath);
  emitter_on(EMITTER(chooser), EVT_DONE, tk_on_choose_file_result, req);

  return file_chooser_choose_folder(chooser);
}

navigator_handler_t* navigator_handler_awtk_pick_dir_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_pick_dir;

  return handler;
}

static ret_t navigator_handler_awtk_on_count_view_model(navigator_handler_t* handler,
                                                        navigator_request_t* req) {
  ret_t ret = RET_FAIL;
  int32_t cnt = 0;
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  widget_t* wm = window_manager();
  return_value_if_fail(wm != NULL, RET_FAIL);

  if (target == NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN(wm, iter, i)
    cnt += navigator_handler_awtk_count_view_models(iter);
    WIDGET_FOR_EACH_CHILD_END();
    ret = RET_OK;
  } else if (tk_str_eq(target, "")) {
    widget_t* win = window_manager_get_top_window(wm);
    if (win != NULL) {
      cnt = navigator_handler_awtk_count_view_models(win);
      ret = RET_OK;
    }
  } else {
    widget_t* widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
    if (widget != NULL) {
      view_model_t* view_model = navigator_handler_awtk_get_view_model(widget);
      if (view_model != NULL) {
        cnt = 1;
      }
      ret = RET_OK;
    }
  }

  value_set_int32(&(req->result), cnt);

  return ret;
}

navigator_handler_t* navigator_handler_awtk_count_view_model_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_count_view_model;

  return handler;
}

static ret_t navigator_handler_awtk_on_get_view_model(navigator_handler_t* handler,
                                                      navigator_request_t* req) {
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  widget_t* wm = window_manager();
  darray_t* result = (darray_t*)(req->user_data);
  return_value_if_fail(wm != NULL, RET_FAIL);

  if (target == NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN(wm, iter, i)
    navigator_handler_awtk_get_view_models(iter, result);
    WIDGET_FOR_EACH_CHILD_END();
  } else if (tk_str_eq(target, "")) {
    widget_t* win = window_manager_get_top_window(wm);
    if (win != NULL) {
      navigator_handler_awtk_get_view_models(win, result);
    }
  } else {
    widget_t* widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
    if (widget != NULL) {
      view_model_t* view_model = navigator_handler_awtk_get_view_model(widget);
      if (view_model != NULL) {
        darray_push(result, view_model);
      }
    }
  }

  value_set_pointer(&(req->result), result);
  return RET_OK;
}

navigator_handler_t* navigator_handler_awtk_get_view_model_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_get_view_model;

  return handler;
}

static ret_t navigator_handler_awtk_on_notify_view_model(navigator_handler_t* handler,
                                                         navigator_request_t* req) {
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  bool_t is_props_changed =
      tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_PROPS_CHANGED, FALSE);
  bool_t is_items_changed =
      tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_ITEMS_CHANGED, FALSE);
  widget_t* wm = window_manager();
  return_value_if_fail(wm != NULL, RET_FAIL);

  if (is_props_changed) {
    if (target == NULL) {
      WIDGET_FOR_EACH_CHILD_BEGIN(wm, iter, i)
      navigator_handler_awtk_notify_view_models_props_changed(iter);
      WIDGET_FOR_EACH_CHILD_END();
    } else if (tk_str_eq(target, "")) {
      widget_t* win = window_manager_get_top_window(wm);
      if (win != NULL) {
        navigator_handler_awtk_notify_view_models_props_changed(win);
      }
    } else {
      widget_t* widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
      if (widget != NULL) {
        view_model_t* view_model = navigator_handler_awtk_get_view_model(widget);
        if (view_model != NULL) {
          view_model_notify_props_changed(view_model);
        }
      }
    }
  }

  if (is_items_changed) {
    tk_object_t* items = tk_object_get_prop_object(TK_OBJECT(req), NAVIGATOR_ARG_EVENT_SOURCE);

    if (target == NULL) {
      WIDGET_FOR_EACH_CHILD_BEGIN(wm, iter, i)
      navigator_handler_awtk_notify_view_models_items_changed(iter, items);
      WIDGET_FOR_EACH_CHILD_END();
    } else if (tk_str_eq(target, "")) {
      widget_t* win = window_manager_get_top_window(wm);
      if (win != NULL) {
        navigator_handler_awtk_notify_view_models_items_changed(win, items);
      }
    } else {
      widget_t* widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
      if (widget != NULL) {
        view_model_t* view_model = navigator_handler_awtk_get_view_model(widget);
        if (view_model != NULL) {
          view_model_notify_items_changed(view_model, items);
        }
      }
    }
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_on_get_locale(navigator_handler_t* handler,
                                                  navigator_request_t* req) {
  tk_object_t* obj = NULL;
  widget_t* widget = NULL;
  locale_info_t* info = NULL;
  widget_t* wm = window_manager();
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  return_value_if_fail(wm != NULL, RET_FAIL);

  if (target == NULL) {
    widget = wm;
  } else if (tk_str_eq(target, "")) {
    widget = window_manager_get_top_window(wm);
  } else {
    widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
  }

  if (widget != NULL) {
    info = widget_get_locale_info(widget);
  }
  if (info != NULL) {
    obj = object_default_create();
    tk_object_set_prop_str(obj, "language", info->language);
    tk_object_set_prop_str(obj, "country", info->country);
  }

  value_set_object(&(req->result), obj);
  req->result.free_handle = TRUE;

  return RET_OK;
}

static ret_t navigator_handler_awtk_on_set_locale(navigator_handler_t* handler,
                                                  navigator_request_t* req) {
  ret_t ret = RET_OK;
  widget_t* widget = NULL;
  locale_info_t* info = NULL;
  widget_t* wm = window_manager();
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  const char* language = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_LANGUAGE);
  const char* country = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_COUNTRY);
  return_value_if_fail(wm != NULL && language != NULL && country != NULL, RET_FAIL);

  if (target == NULL) {
    widget = wm;
  } else if (tk_str_eq(target, "")) {
    widget = window_manager_get_top_window(wm);
  } else {
    widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
  }

  if (widget != NULL) {
    info = widget_get_locale_info(widget);
  }
  if (info != NULL) {
    ret = locale_info_change(info, language, country);
  }

  return ret;
}

static ret_t navigator_handler_awtk_on_get_theme(navigator_handler_t* handler,
                                                 navigator_request_t* req) {
  widget_t* widget = NULL;
  assets_manager_t* am = NULL;
  widget_t* wm = window_manager();
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  return_value_if_fail(wm != NULL, RET_FAIL);

  if (target == NULL) {
    widget = wm;
  } else if (tk_str_eq(target, "")) {
    widget = window_manager_get_top_window(wm);
  } else {
    widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
  }

  if (widget != NULL) {
    am = widget_get_assets_manager(widget);
  }
  if (am != NULL) {
    value_set_str(&(req->result), am->theme);
  }

  return RET_OK;
}

static ret_t navigator_handler_awtk_on_set_theme(navigator_handler_t* handler,
                                                 navigator_request_t* req) {
  ret_t ret = RET_OK;
  widget_t* widget = NULL;
  widget_t* wm = window_manager();
  const char* target = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET);
  const char* theme = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_THEME);
  return_value_if_fail(wm != NULL && theme != NULL, RET_FAIL);

  if (target == NULL) {
    widget = wm;
  } else if (tk_str_eq(target, "")) {
    widget = window_manager_get_top_window(wm);
  } else {
    widget = navigator_handler_awtk_lookup_widget_by_path(wm, target);
  }

  if (widget != NULL) {
    ret = widget_set_theme(widget, theme);
  }

  return ret;
}

static ret_t navigator_handler_awtk_on_set_screen_saver_time(navigator_handler_t* handler,
                                                             navigator_request_t* req) {
  ret_t ret = RET_OK;
  widget_t* wm = window_manager();
  uint32_t time = tk_object_get_prop_int(TK_OBJECT(req), NAVIGATOR_ARG_TIME, 0);

  ret = window_manager_set_screen_saver_time(wm, time);

  return ret;
}

navigator_handler_t* navigator_handler_awtk_notify_view_model_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_notify_view_model;

  return handler;
}

navigator_handler_t* navigator_handler_awtk_get_locale_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_get_locale;

  return handler;
}

navigator_handler_t* navigator_handler_awtk_set_locale_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_set_locale;

  return handler;
}

navigator_handler_t* navigator_handler_awtk_get_theme_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_get_theme;

  return handler;
}

navigator_handler_t* navigator_handler_awtk_set_theme_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_set_theme;

  return handler;
}

navigator_handler_t* navigator_handler_awtk_set_screen_saver_time_create(void) {
  tk_object_t* obj = NULL;
  navigator_handler_t* handler = NULL;

  obj = tk_object_create(&s_navigator_handler_awtk_vtable);
  handler = NAVIGATOR_HANDLER(obj);
  return_value_if_fail(handler != NULL, NULL);

  handler->on_request = navigator_handler_awtk_on_set_screen_saver_time;

  return handler;
}
