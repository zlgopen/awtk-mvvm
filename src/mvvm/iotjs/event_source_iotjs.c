/**
 * File:   event_source_iotjs.h
 * Author: AWTK Develop Team
 * Brief:  event source iotjs
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-02-14 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "tkc/types_def.h"

#ifdef WITH_IOTJS
#include "mvvm_iotjs.h"
#include "event_source_iotjs.h"

static const object_vtable_t s_event_source_iotjs_vtable = {
    .type = "event_source_iotjs", .desc = "event_source_iotjs", .size = sizeof(event_source_iotjs_t)};

static int32_t event_source_iotjs_get_fd(event_source_t* source) {
  (void)source;
  return -1;
}

static ret_t event_source_iotjs_check(event_source_t* source) {
  (void)source;
  return RET_OK;
}

static ret_t event_source_iotjs_dispatch(event_source_t* source) {
  (void)source;
  return mvvm_iotjs_run();
}

uint32_t event_source_iotjs_get_wakeup_time(event_source_t* source) {
  (void)source;
  return 0;
}

event_source_t* event_source_iotjs_create() {
  tk_object_t* obj = NULL;
  event_source_t* event_source = NULL;
  event_source_iotjs_t* event_source_iotjs = NULL;

  obj = tk_object_create(&s_event_source_iotjs_vtable);
  event_source = EVENT_SOURCE(obj);
  event_source_iotjs = EVENT_SOURCE_IDLE(obj);
  return_value_if_fail(event_source_iotjs != NULL, NULL);

  event_source->check = event_source_iotjs_check;
  event_source->get_fd = event_source_iotjs_get_fd;
  event_source->dispatch = event_source_iotjs_dispatch;
  event_source->get_wakeup_time = event_source_iotjs_get_wakeup_time;

  return event_source;
}
#endif /*WITH_IOTJS*/
