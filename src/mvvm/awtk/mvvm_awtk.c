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


static ret_t func_navigator_to(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  return_value_if_fail(args->size == 1, RET_BAD_PARAMS);

  return navigator_to(value_str(args->args));
}

static ret_t func_navigator_replace(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  binding_rule_t* rule = BINDING_RULE(fscript->obj);
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

  return RET_OK;
}

ret_t mvvm_awtk_deinit(void) {
  return RET_OK;
}
