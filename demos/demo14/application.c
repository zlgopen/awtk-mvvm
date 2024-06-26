/**
 * File:   application.c
 * Author: AWTK Develop Team
 * Brief:  application
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "temperature_view_model.h"

static ret_t mvvm_app_init(void);
static ret_t mvvm_app_deinit(void);

ret_t application_init(void) {
#ifdef AWTK_WEB
  mvvm_app_init();
#endif/*AWTK_WEB*/
  view_model_factory_register("temperature_ex", temperature_view_model_create);

  return navigator_to("temperature14");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");
#ifdef AWTK_WEB
  mvvm_app_deinit();
#endif/*AWTK_WEB*/

  return RET_OK;
}

#include "../main.inc"
