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
 * 2019-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "conf_io/app_conf_init_json.h"
#include "humidity_view_model.h"
#include "temperature_view_model.h"

ret_t application_init(void) {
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_init();
#endif/*WITHOUT_AWTK_MAIN*/
  app_conf_init_json("demo27");
  view_model_factory_register("temperature", temperature_view_model_create);
  view_model_factory_register("humidity", humidity_view_model_create);

  return navigator_to("compositor");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");

  if (app_conf_get_instance() != NULL) {
    app_conf_save();
    app_conf_deinit();
  }
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_deinit();
#endif/*WITHOUT_AWTK_MAIN*/

  return RET_OK;
}

#include "../main.inc"
