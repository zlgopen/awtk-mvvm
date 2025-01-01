/**
 * File:   room_settings.c
 * Author: AWTK Develop Team
 * Brief:  room_settings
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"

#include "room_settings.h"

room_settings_t* room_settings_create(navigator_request_t* req) {
  room_settings_t* room_settings = TKMEM_ZALLOC(room_settings_t);
  return_value_if_fail(room_settings != NULL, NULL);

  room_settings->req = req;
  room_settings->temp = tk_object_get_prop_int(TK_OBJECT(req), "temp", 0);
  room_settings->room = tk_strdup(tk_object_get_prop_str(TK_OBJECT(req), "room"));
  room_settings->humidity = tk_object_get_prop_int(TK_OBJECT(req), "humidity", 0);

  return room_settings;
}

ret_t room_settings_return(room_settings_t* room_settings) {
  value_t v;
  ret_t ret = RET_OK;
  tk_object_t* result = object_default_create();
  return_value_if_fail(room_settings != NULL, RET_BAD_PARAMS);

  tk_object_set_prop_int(result, "temp", room_settings->temp);
  tk_object_set_prop_int(result, "humidity", room_settings->humidity);

  value_set_object(&v, result);
  ret = navigator_request_on_result(room_settings->req, &v);
  value_reset(&v);
  TK_OBJECT_UNREF(result);

  return ret;
}

ret_t room_settings_destroy(room_settings_t* room_settings) {
  return_value_if_fail(room_settings != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(room_settings->room);
  TKMEM_FREE(room_settings);

  return RET_OK;
}
