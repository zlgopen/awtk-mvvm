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
 * 2019-02-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "temperature_timer.h"

ret_t application_init(void) {
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_init();
#endif/*WITHOUT_AWTK_MAIN*/
  view_model_factory_register("temperature_timer", temperature_view_model_timer_create);

  return navigator_to("temperature10");
}

ret_t application_exit(void) {
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_deinit();
#endif/*WITHOUT_AWTK_MAIN*/
  log_debug("application_exit\n");

  return RET_OK;
}

#include "../main.inc"
