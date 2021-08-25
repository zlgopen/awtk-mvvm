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
#include "tkc/object_default.h"
#include "base/window_manager.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/binding_rule.h"
#include "mvvm/awtk/mvvm_awtk.h"

#define STR_WINDOW "window"
#define STR_PARENT "parent"

static ret_t func_navigator_to(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  const char* str_args = NULL;
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  str_args = value_str(args->args);
  if (str_args != NULL && tk_str_start_with(str_args, COMMAND_ARGS_FSCRIPT_PREFIX)) {
    ret_t ret = RET_FAIL;
    object_t* obj_args = object_default_create();
    return_value_if_fail(obj_args != NULL, RET_OOM);

    ret = tk_command_arguments_to_object(str_args, obj_args);
    if (ret == RET_OK) {
      ret = tk_command_arguments_fscript(obj_args, fscript->obj);
    }
    if (ret == RET_OK) {
      ret = navigator_to_by_object(obj_args);
    }

    OBJECT_UNREF(obj_args);
    return ret;
  } else {
    return navigator_to(value_str(args->args));
  }
}

static ret_t func_navigator_replace(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  return navigator_replace(value_str(args->args));
}

static ret_t mvvm_funcs_init(void) {
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
  navigator_register_handler(navigator(), NAVIGATOR_REQ_COUNT_VIEW_MODEL,
                             navigator_handler_awtk_count_view_model_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_GET_VIEW_MODEL,
                             navigator_handler_awtk_get_view_model_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_NOTIFY_VIEW_MODEL,
                             navigator_handler_awtk_notify_view_model_create());

  return RET_OK;
}

ret_t mvvm_awtk_deinit(void) {
  return RET_OK;
}
