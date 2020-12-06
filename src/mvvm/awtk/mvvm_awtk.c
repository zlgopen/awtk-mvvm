/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm awtk global functions.
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
 * 2019-02-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk_global.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "base/window_manager.h"
#include "mvvm/base/binding_rule.h"
#include "mvvm/awtk/mvvm_awtk.h"

#define STR_WINDOW "window"
#define STR_PARENT "parent"

static widget_t* binding_rule_find_widget(binding_rule_t* rule, const char* path) {
  char name[TK_NAME_LEN + 1];
  const char* p = strchr(path, '.');
  widget_t* widget = WIDGET(rule->widget);

  if (p != NULL) {
    memset(name, 0, sizeof(name));
    tk_strncpy(name, path, tk_min(TK_NAME_LEN, p - path));
    if (tk_str_eq(name, STR_WINDOW)) {
      widget = widget_get_window(widget);
      p++;
    } else if (tk_str_eq(name, STR_PARENT)) {
      widget = widget->parent;
      p++;
    } else {
      widget = widget_get_window(widget);
      p = path;
    }
    path = p;
    p = strchr(path, '.');
    if (p != NULL) {
      memset(name, 0, sizeof(name));
      tk_strncpy(name, path, tk_min(TK_NAME_LEN, p - path));
      widget = widget_lookup(widget, name, TRUE);
    }
  }

  return widget;
}

static ret_t binding_rule_widget_set(binding_rule_t* rule, const char* path, const value_t* value) {
  const char* prop = NULL;
  widget_t* widget = binding_rule_find_widget(rule, path);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  prop = strrchr(path, '.');
  prop = prop == NULL ? path : prop + 1;
  return widget_set_prop(widget, prop, value);
}

static ret_t binding_rule_widget_get(binding_rule_t* rule, const char* path, value_t* value) {
  const char* prop = NULL;
  widget_t* widget = binding_rule_find_widget(rule, path);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  prop = strrchr(path, '.');
  prop = prop == NULL ? path : prop + 1;
  return widget_get_prop(widget, prop, value);
}

static ret_t func_tr(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  value_t* input = args->args;
  if (input->type == VALUE_TYPE_STRING) {
    const char* str = value_str(input);
    str = locale_info_tr(locale_info(), str);
    value_dup_str(v, str);
    return RET_OK;
  } else {
    value_set_str(v, NULL);
    return RET_OK;
  }
}

static ret_t func_quit(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  tk_quit();
  return RET_OK;
}

static ret_t func_back(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  window_manager_back(window_manager());

  return RET_OK;
}

static ret_t func_back_to_home(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  window_manager_back_to_home(window_manager());

  return RET_OK;
}

static ret_t func_navigator_to(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  return navigator_to(value_str(args->args));
}

static ret_t func_navigator_replace(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  binding_rule_t* rule = BINDING_RULE(fscript->obj);
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  return navigator_replace(value_str(args->args));
}

static ret_t func_widget_get(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  binding_rule_t* rule = BINDING_RULE(fscript->obj);
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  return binding_rule_widget_get(rule, value_str(args->args), v);
}

static ret_t func_widget_set(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  ret_t ret = RET_OK;
  binding_rule_t* rule = BINDING_RULE(fscript->obj);
  return_value_if_fail(args->size == 2, RET_BAD_PARAMS);

  ret = binding_rule_widget_set(rule, value_str(args->args), args->args + 1);
  value_set_bool(v, ret == RET_OK);

  return RET_OK;
}

static ret_t mvvm_funcs_init(void) {
  fscript_register_func("tr", func_tr);
  fscript_register_func("quit", func_quit);
  fscript_register_func("back", func_back);
  fscript_register_func("widget_get", func_widget_get);
  fscript_register_func("widget_set", func_widget_set);
  fscript_register_func("back_to_home", func_back_to_home);
  fscript_register_func("navigator_to", func_navigator_to);
  fscript_register_func("navigator_replace", func_navigator_replace);

  return RET_OK;
}

ret_t mvvm_awtk_init(void) {
  mvvm_funcs_init();
  navigator_register_handler(navigator(), NAVIGATOR_DEFAULT_HANDLER,
                             navigator_handler_awtk_create());

  navigator_register_handler(navigator(), NAVIGATOR_REQ_TOAST,
                             navigator_handler_awtk_toast_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_INFO, navigator_handler_awtk_info_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_WARN, navigator_handler_awtk_warn_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_CONFIRM,
                             navigator_handler_awtk_confirm_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_PICK_FILE,
                             navigator_handler_awtk_pick_file_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_PICK_DIR,
                             navigator_handler_awtk_pick_dir_create());

  return RET_OK;
}

ret_t mvvm_awtk_deinit(void) {
  return RET_OK;
}
