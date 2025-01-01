/**
 * File:   application.c
 * Author: AWTK Develop Team
 * Brief:  application
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "awtk.h"
#include "mvvm/mvvm.h"

#include "app_view_model.h"
#include "humidity_view_model.h"
#include "temperature_view_model.h"

static ret_t mvvm_app_init(void);
static ret_t mvvm_app_deinit(void);

ret_t application_init(void) {
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_init();
#endif/*WITHOUT_AWTK_MAIN*/

  view_model_factory_register("app", app_view_model_create);
  view_model_factory_register("humidity", humidity_view_model_create);
  view_model_factory_register("temperature", temperature_view_model_create);

  return navigator_to("demo9_main");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_deinit();
#endif/*WITHOUT_AWTK_MAIN*/

  return RET_OK;
}

#include "../main.inc"
