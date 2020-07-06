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

static ret_t on_key_down(void* ctx, event_t* e) {
  key_event_t* evt = (key_event_t*)e;
  if (evt->key == TK_KEY_ESCAPE) {
    tk_quit();
  } else if (evt->key == TK_KEY_F1) {
    tk_mem_dump();
  }
  return RET_OK;
}

static ret_t install_quit_shortcut(void) {
  widget_t* wm = window_manager();
  widget_on(wm, EVT_KEY_DOWN, on_key_down, wm);

  return RET_OK;
}

static ret_t mvvm_app_init(void) {
  mvvm_init();
#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_init();
#endif/*WITH_JERRYSCRIPT*/
  install_quit_shortcut();
  
  return RET_OK;
}

static ret_t mvvm_app_deinit(void) {
#ifdef WITH_JERRYSCRIPT
  mvvm_jerryscript_deinit();
#endif/*WITH_JERRYSCRIPT*/
  mvvm_deinit();

  return RET_OK;
}

#define GLOBAL_INIT() mvvm_app_init()

#define GLOBAL_EXIT() mvvm_app_deinit()

extern ret_t application_init(void);

ret_t application_exit() {
  log_debug("application_exit\n");
  if (app_conf_get_instance() != NULL) {
    app_conf_save();
    app_conf_deinit();
  }

  return RET_OK;
}

#define APP_DEFAULT_FONT "default_full"
#include "awtk_main.inc"
