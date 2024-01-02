/**
 * File:   buzzer_log.c
 * Author: AWTK Develop Team
 * Brief:  buzzer_log
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "buzzer_log.h"

static ret_t buzzer_log_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (v->type == VALUE_TYPE_STRING) {
    log_debug("buzzer: %s = %s\n", name, value_str(v));
  } else {
    log_debug("buzzer: %s = %d\n", name, value_int(v));
  }

  return RET_OK;
}

static const object_vtable_t s_buzzer_log_vtable = {.type = "buzzer_log",
                                                    .desc = "buzzer_log",
                                                    .size = sizeof(buzzer_log_t),
                                                    .set_prop = buzzer_log_set_prop};

tk_object_t* buzzer_log_create(const char* args) {
  return tk_object_create(&s_buzzer_log_vtable);
}
