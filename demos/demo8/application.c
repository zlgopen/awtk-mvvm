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
#include "shape_view_model.h"

ret_t application_init(void) {
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_init();
#endif/*WITHOUT_AWTK_MAIN*/
  view_model_factory_register("shape", shape_view_model_create);

  return navigator_to("shape");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_deinit();
#endif/*WITHOUT_AWTK_MAIN*/

  return RET_OK;
}

#include "../main.inc"
