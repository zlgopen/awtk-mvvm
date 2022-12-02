/**
 * File:   jerryscript_awtk.c
 * Author: AWTK Develop Team
 * Brief:  awtk for jerryscript
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
 * 2019-02-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "tkc/utils.h"
#include "base/idle.h"
#include "base/timer.h"
#include "base/widget.h"
#include "base/events.h"
#include "awtk_global.h"
#include "conf_io/app_conf.h"
#include "mvvm/base/navigator.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/jerryscript_awtk.h"

static ret_t ret_t_init(void) {
  jsvalue_t obj = jsvalue_get_global_object();

  jsobj_set_prop_int(obj, "RET_OK", RET_OK);
  jsobj_set_prop_int(obj, "RET_OOM", RET_OOM);
  jsobj_set_prop_int(obj, "RET_FAIL", RET_FAIL);
  jsobj_set_prop_int(obj, "RET_NOT_IMPL", RET_NOT_IMPL);
  jsobj_set_prop_int(obj, "RET_QUIT", RET_QUIT);
  jsobj_set_prop_int(obj, "RET_FOUND", RET_FOUND);
  jsobj_set_prop_int(obj, "RET_BUSY", RET_BUSY);
  jsobj_set_prop_int(obj, "RET_REMOVE", RET_REMOVE);
  jsobj_set_prop_int(obj, "RET_REPEAT", RET_REPEAT);
  jsobj_set_prop_int(obj, "RET_NOT_FOUND", RET_NOT_FOUND);
  jsobj_set_prop_int(obj, "RET_DONE", RET_DONE);
  jsobj_set_prop_int(obj, "RET_STOP", RET_STOP);
  jsobj_set_prop_int(obj, "RET_CONTINUE", RET_CONTINUE);
  jsobj_set_prop_int(obj, "RET_BAD_PARAMS", RET_BAD_PARAMS);
  jsobj_set_prop_int(obj, "RET_ITEMS_CHANGED", RET_ITEMS_CHANGED);
  jsobj_set_prop_int(obj, "RET_OBJECT_CHANGED", RET_OBJECT_CHANGED);

  jsvalue_unref(obj);

  return RET_OK;
}

static ret_t event_type_t_init(void) {
  jsvalue_t obj = jsvalue_get_global_object();

  jsobj_set_prop_int(obj, "EVT_ITEMS_CHANGED", EVT_ITEMS_CHANGED);
  jsobj_set_prop_int(obj, "EVT_PROPS_CHANGED", EVT_PROPS_CHANGED);
  jsobj_set_prop_int(obj, "EVT_PROP_CHANGED", EVT_PROP_CHANGED);

  jsvalue_unref(obj);

  return RET_OK;
}

static ret_t timer_info_on_destroy(void* data) {
  timer_info_t* item = (timer_info_t*)data;

  uint32_t func = (char*)(item->ctx) - (char*)NULL;
  jsvalue_unref(func);

  return RET_OK;
}

static ret_t call_on_timer(const timer_info_t* timer) {
  jsvalue_t res;
  jsvalue_t args[1];
  jsvalue_t this_value = JS_UNDEFINED;
  jsvalue_t func = (jsvalue_t)((char*)timer->ctx - (char*)NULL);

  args[0] = JS_UNDEFINED;
  res = jsfunc_call(func, this_value, args, 1);

  jsvalue_unref(args[0]);
  jsvalue_unref(this_value);

  return (ret_t)jsvalue_to_number(res);
}

static JSFUNC_DECL(wrap_timer_add) {
  int32_t ret = RET_BAD_PARAMS;

  if (args_count >= 2) {
    jsvalue_t on_timer = jsvalue_ref(args_p[0]);
    uint32_t duration_ms = (uint32_t)jsvalue_to_number(args_p[1]);
    void* ctx = (char*)NULL + (int32_t)on_timer;

    ret = (uint32_t)timer_add(call_on_timer, ctx, duration_ms);
    timer_set_on_destroy(ret, timer_info_on_destroy, NULL);
  } else {
    log_warn("%s invalid args\n", __FUNCTION__);
  }

  return jsvalue_from_number(ret);
}

static ret_t idle_info_on_destroy(void* data) {
  idle_info_t* item = (idle_info_t*)data;

  jsvalue_t func = (jsvalue_t)((char*)(item->ctx) - (char*)NULL);
  jsvalue_unref(func);

  return RET_OK;
}

static ret_t call_on_idle(const idle_info_t* idle) {
  jsvalue_t res;
  jsvalue_t args[1];
  jsvalue_t this_value = JS_UNDEFINED;
  jsvalue_t func = (jsvalue_t)((char*)idle->ctx - (char*)NULL);

  args[0] = JS_UNDEFINED;
  res = jsfunc_call(func, this_value, args, 1);

  jsvalue_unref(args[0]);
  jsvalue_unref(this_value);

  return (ret_t)jsvalue_to_number(res);
}

static JSFUNC_DECL(wrap_idle_add) {
  int32_t ret = RET_BAD_PARAMS;

  if (args_count >= 1) {
    jsvalue_t on_idle = jsvalue_ref(args_p[0]);
    void* ctx = (char*)NULL + (int32_t)on_idle;

    ret = (uint32_t)idle_add(call_on_idle, ctx);
    idle_set_on_destroy(ret, idle_info_on_destroy, NULL);
  } else {
    log_warn("%s invalid args\n", __FUNCTION__);
  }

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_timer_remove) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count == 1) {
    uint32_t timer_id = (uint32_t)jsvalue_to_number(args_p[0]);
    ret = (ret_t)timer_remove(timer_id);
  }

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_idle_remove) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 1) {
    uint32_t idle_id = (uint32_t)jsvalue_to_number(args_p[0]);
    ret = (ret_t)idle_remove(idle_id);
  }

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_quit) {
  tk_quit();
  return jsvalue_from_number(0);
}

static JSFUNC_DECL(wrap_navigate_to) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 1) {
    jsvalue_t jsreq = args_p[0];

    if (jsvalue_is_object(jsreq)) {
      navigator_request_t* req = jsvalue_to_navigator_request(jsreq);
      if (req != NULL) {
        ret = navigator_to_ex(req);
        tk_object_unref(TK_OBJECT(req));
      }
    } else if (jerry_value_is_string(jsreq)) {
      str_t str;
      str_init(&str, 0);
      if (jsvalue_to_utf8(jsreq, &str) != NULL) {
        ret = navigator_to(str.str);
      }
      str_reset(&str);
    }
  }

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_count_view_models) {
  int32_t cnt = 0;
  const char* target = NULL;
  str_t str;
  str_init(&str, 0);

  if (args_count >= 1) {
    if (jerry_value_is_string(args_p[0])) {
      if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
        target = str.str;
      }
    }
  }

  cnt = navigator_count_view_models(target);
  str_reset(&str);

  return jsvalue_from_number(cnt);
}

static JSFUNC_DECL(wrap_get_view_models) {
  jsvalue_t ret = JS_EMPTY_ARRAY;
  darray_t* temp = darray_create(1, NULL, NULL);

  if (temp != NULL) {
    const char* target = NULL;
    str_t str;
    str_init(&str, 0);

    if (args_count >= 1) {
      if (jerry_value_is_string(args_p[0])) {
        if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
          target = str.str;
        }
      }
    }

    if (navigator_get_view_models(target, temp) == RET_OK) {
      uint32_t i = 0;
      for (i = 0; i < temp->size; i++) {
        jsvalue_t vm = jsvalue_from_obj(TK_OBJECT(temp->elms[i]));
        jsobj_set_prop_value_by_index(ret, i, vm);
      }
    }

    str_reset(&str);
    darray_destroy(temp);
  }

  return ret;
}

static JSFUNC_DECL(wrap_notify_props_changed_to_view_models) {
  ret_t ret = RET_FAIL;
  const char* target = NULL;
  str_t str;
  str_init(&str, 0);

  if (args_count >= 1) {
    if (jerry_value_is_string(args_p[0])) {
      if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
        target = str.str;
      }
    }
  }

  ret = navigator_notify_view_props_changed(target);
  str_reset(&str);

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_notify_items_changed_to_view_models) {
  ret_t ret = RET_FAIL;
  const char* target = NULL;
  tk_object_t* items = NULL;
  str_t str;
  str_init(&str, 0);

  if (args_count >= 1) {
    if (jsvalue_is_object(args_p[0])) {
      items = jsvalue_to_obj(args_p[0]);
    }
  }

  if (args_count >= 2) {
    if (jerry_value_is_string(args_p[1])) {
      if (jsvalue_to_utf8(args_p[1], &str) != NULL) {
        target = str.str;
      }
    }
  }

  ret = navigator_notify_view_items_changed(items, target);
  str_reset(&str);

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_get_locale) {
  jsvalue_t res;
  tk_object_t* obj = NULL;
  const char* target = NULL;
  str_t str;
  str_init(&str, 0);

  if (args_count >= 1) {
    if (jerry_value_is_string(args_p[0])) {
      if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
        target = str.str;
      }
    }
  }

  obj = navigator_get_locale(target);
  res = jsvalue_from_obj_copy(obj);
  tk_object_unref(obj);
  str_reset(&str);

  return res;
}

static JSFUNC_DECL(wrap_set_locale) {
  ret_t ret = RET_FAIL;
  char language[3] = {0};
  char country[3] = {0};
  const char* target = NULL;
  str_t str;
  str_init(&str, 0);

  if (args_count >= 2) {
    if (jerry_value_is_string(args_p[0])) {
      if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
        tk_strncpy_s(language, sizeof(language), str.str, tk_strlen(str.str));
      }
    }
    if (jerry_value_is_string(args_p[1])) {
      if (jsvalue_to_utf8(args_p[1], &str) != NULL) {
        tk_strncpy_s(country, sizeof(country), str.str, tk_strlen(str.str));
      }
    }
    if (args_count >= 3 && jerry_value_is_string(args_p[2])) {
      if (jsvalue_to_utf8(args_p[2], &str) != NULL) {
        target = str.str;
      }
    }
  }

  ret = navigator_set_locale(language, country, target);
  str_reset(&str);

  return jsvalue_from_number(ret);
}

static JSFUNC_DECL(wrap_get_theme) {
  jsvalue_t ret;
  const char* theme = NULL;
  const char* target = NULL;
  str_t str;
  str_init(&str, 0);

  if (args_count >= 1) {
    if (jerry_value_is_string(args_p[0])) {
      if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
        target = str.str;
      }
    }
  }

  theme = navigator_get_theme(target);
  ret = jsvalue_from_utf8(theme);
  str_reset(&str);

  return ret;
}

static JSFUNC_DECL(wrap_set_theme) {
  ret_t ret = RET_FAIL;
  char* theme = NULL;
  const char* target = NULL;

  str_t str;
  str_init(&str, 0);

  if (args_count >= 1) {
    if (jerry_value_is_string(args_p[0])) {
      if (jsvalue_to_utf8(args_p[0], &str) != NULL) {
        theme = tk_str_copy(theme, str.str);
      }
    }
    if (args_count >= 2 && jerry_value_is_string(args_p[1])) {
      if (jsvalue_to_utf8(args_p[1], &str) != NULL) {
        target = str.str;
      }
    }
  }

  ret = navigator_set_theme(theme, target);
  str_reset(&str);
  TKMEM_FREE(theme);

  return jsvalue_from_number(ret);
}

ret_t jerryscript_awtk_init(void) {
  ret_t_init();
  event_type_t_init();

  jsfunc_register_global("exit", wrap_quit);
  jsfunc_register_global("quit", wrap_quit);
  jsfunc_register_global("timerAdd", wrap_timer_add);
  jsfunc_register_global("timerRemove", wrap_timer_remove);
  jsfunc_register_global("idleAdd", wrap_idle_add);
  jsfunc_register_global("idleRemove", wrap_idle_remove);
  jsfunc_register_global("navigateTo", wrap_navigate_to);
  jsfunc_register_global("countViewModels", wrap_count_view_models);
  jsfunc_register_global("getViewModels", wrap_get_view_models);
  jsfunc_register_global("notifyViewPropsChanged", wrap_notify_props_changed_to_view_models);
  jsfunc_register_global("notifyViewItemsChanged", wrap_notify_items_changed_to_view_models);
  jsfunc_register_global("getLocale", wrap_get_locale);
  jsfunc_register_global("setLocale", wrap_set_locale);
  jsfunc_register_global("getTheme", wrap_get_theme);
  jsfunc_register_global("setTheme", wrap_set_theme);

  if (jsobj_register_global("appConf", app_conf_get_instance()) != RET_OK) {
    jsvalue_t global_obj = jsvalue_get_global_object();
    jsobj_set_prop_value(global_obj, "appConf", JS_UNDEFINED);
    jsvalue_unref(global_obj);
  }

  return RET_OK;
}

ret_t jerryscript_awtk_deinit(void) {
  /* Close all windows to unref view models, avoid assert while jerryscript clean up. */
  window_manager_close_all(window_manager());
  idle_dispatch();

  return RET_OK;
}
