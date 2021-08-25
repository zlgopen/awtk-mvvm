/**
 * File:   application.c
 * Author: AWTK Develop Team
 * Brief:  application
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "devices_view_model.h"

ret_t application_init(void) {
  view_model_factory_register("devices", devices_view_model_create);
  return navigator_to("demo35");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");

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

#include "../main.inc"
