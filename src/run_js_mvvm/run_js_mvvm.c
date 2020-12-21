/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "conf_io/app_conf.h"
BEGIN_C_DECLS
#include "../res/assets.inc"
END_C_DECLS

static ret_t mvvm_app_init(void) {
  mvvm_init();
  mvvm_jerryscript_init();

  return RET_OK;
}

static ret_t mvvm_app_deinit(void) {
  mvvm_jerryscript_deinit();
  mvvm_deinit();

  return RET_OK;
}

#define GLOBAL_INIT() mvvm_app_init()

#define GLOBAL_EXIT() mvvm_app_deinit()

ret_t application_exit() {
  log_debug("application_exit\n");
  if (app_conf_get_instance() != NULL) {
    app_conf_save();
    app_conf_deinit();
  }

  return RET_OK;
}

static const char* s_home_win = "main";

static ret_t mvvm_on_cmd_line(int argc, char** argv) {
  if (argc < 2) {
    log_debug("Usage: %s main_window_name\n", argv[0]);
    log_debug(" ex: %s js_temperature1\n", argv[0]);
    exit(0);
  } else {
    s_home_win = argv[1];
    log_debug("navigator_to:%s\n", s_home_win);
    return RET_OK;
  }
}

#define ON_CMD_LINE mvvm_on_cmd_line

ret_t application_init(void) {
  navigator_to(s_home_win);

  return RET_OK;
}

#include "awtk_main.inc"

