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
 * 2020-06-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "conf_io/app_conf.h"
#include "conf_io/app_conf_init_json.h"

static ret_t on_app_conf_events(void* ctx, event_t* e) {
  if (e->type == EVT_PROP_CHANGED) {
    prop_change_event_t* evt = prop_change_event_cast(e);
    log_debug("prop %s changed\n", evt->name);
    log_debug("font: %s\n", app_conf_get_str("font.name", NULL));
  } else if (e->type == EVT_CMD_EXECED) {
    cmd_exec_event_t* evt = cmd_exec_event_cast(e);
    if (tk_str_eq(evt->name, OBJECT_CMD_SAVE)) {
      log_debug("app conf saved\n");
    }
  }

  return RET_OK;
}

static ret_t on_app_conf_saved(void* ctx, event_t* e) {
  return RET_OK;
}

ret_t application_init(void) {
  app_conf_init_json("demo26");

  emitter_on(EMITTER(app_conf_get_instance()), EVT_PROP_CHANGED, on_app_conf_events, NULL);
  emitter_on(EMITTER(app_conf_get_instance()), EVT_CMD_EXECED, on_app_conf_events, NULL);

  return navigator_to("app_conf");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");

  if (app_conf_get_instance() != NULL) {
    app_conf_save();
    app_conf_deinit();
  }

  return RET_OK;
}

#include "../main.inc"
