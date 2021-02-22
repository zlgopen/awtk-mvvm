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

#define GLOBAL_INIT() mvvm_init()
#define GLOBAL_EXIT() mvvm_deinit()

extern ret_t application_init();

ret_t application_exit() {
  log_debug("application_exit\n");
  if (app_conf_get_instance() != NULL) {
    app_conf_save();
    app_conf_deinit();
  }

  return RET_OK;
}

#ifdef LCD_WIDTH
#undef LCD_WIDTH
#endif
#define LCD_WIDTH 800

#ifdef LCD_HEIGHT
#undef LCD_HEIGHT
#endif
#define LCD_HEIGHT 480

#include "awtk_main.inc"
