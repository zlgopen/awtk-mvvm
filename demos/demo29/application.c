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
#include "conf_io/app_conf_init_json.h"
#include "conf_io/app_conf.h"

static ret_t on_app_conf_events(void* ctx, event_t* e) {
  if (e->type == EVT_CMD_CAN_EXEC) {
    cmd_exec_event_t* evt = cmd_exec_event_cast(e);
    if (tk_str_eq(evt->name, "return")) {
      evt->can_exec = TRUE;
      return RET_STOP;
    }
  } else if (e->type == EVT_CMD_EXECED) {
    cmd_exec_event_t* evt = cmd_exec_event_cast(e);
    if (tk_str_eq(evt->name, "return")) {
      log_debug("%s:%s\n", evt->name, evt->args);
      return RET_STOP;
    }
  }

  return RET_OK;
}

ret_t application_init() {
#ifdef WITHOUT_AWTK_MAIN
  mvvm_app_init();
#endif/*WITHOUT_AWTK_MAIN*/
  app_conf_init_json("demo29");

  emitter_on(EMITTER(app_conf_get_instance()), EVT_CMD_CAN_EXEC, on_app_conf_events, NULL);
  emitter_on(EMITTER(app_conf_get_instance()), EVT_CMD_EXECED, on_app_conf_events, NULL);

  return navigator_to("app_conf_extend_cmds");
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
