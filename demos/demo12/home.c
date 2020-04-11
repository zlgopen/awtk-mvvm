/**
 * File:   home.c
 * Author: AWTK Develop Team
 * Brief:  home
 *
 * Copyright (c) 2012 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "home.h"
#include "tkc/mem.h"
#include "mvvm/base/navigator_request.h"
#include "../common/room_settings_defines.h"

home_t* home_create(void) {
  home_t* home = TKMEM_ZALLOC(home_t);
  return_value_if_fail(home != NULL, NULL);

  emitter_init(EMITTER(home));
  home->bed_room = room_info_create("bed room");
  home->living_room = room_info_create("living room");

  return home;
}

ret_t home_destroy(home_t* home) {
  return_value_if_fail(home != NULL, RET_BAD_PARAMS);

  emitter_deinit(EMITTER(home));
  room_info_destroy(home->bed_room);
  room_info_destroy(home->living_room);
  TKMEM_FREE(home);

  return RET_OK;
}

static ret_t home_on_room_result(navigator_request_t* req, const value_t* result) {
  object_t* res = value_object(result);
  home_t* h = (home_t*)(object_get_prop_pointer(OBJECT(req), STR_HOME));
  const char* room_name = object_get_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM);
  double temp = object_get_prop_float(res, ROOM_SETTINGS_RESULT_TEMP, 0);
  double humidity = object_get_prop_float(res, ROOM_SETTINGS_RESULT_HUMIDITY, 0);

  if (tk_str_eq(room_name, h->bed_room->name)) {
    h->bed_room->temp = temp;
    h->bed_room->humidity = humidity;
    ;
  } else {
    h->living_room->temp = temp;
    h->living_room->humidity = humidity;
    ;
  }

  emitter_dispatch_simple_event(EMITTER(h), EVT_PROPS_CHANGED);

  return RET_OK;
}

static ret_t adjust_room_params(home_t* h, room_info_t* info) {
  navigator_request_t* req = navigator_request_create("room_settings", home_on_room_result);

  object_set_prop_pointer(OBJECT(req), STR_HOME, h);
  object_set_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM, info->name);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_TEMP, info->temp);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_HUMIDITY, info->humidity);

  navigator_to_ex(req);

  object_unref(OBJECT(req));

  return RET_OK;
}

ret_t home_adjust_bed_room(home_t* home) {
  return adjust_room_params(home, home->bed_room);
}

ret_t home_adjust_living_room(home_t* home) {
  return adjust_room_params(home, home->living_room);
}

const char* home_get_bed_room_info(home_t* home) {
  return_value_if_fail(home != NULL, NULL);

  return room_info_get_summary(home->bed_room);
}

const char* home_get_living_room_info(home_t* home) {
  return_value_if_fail(home != NULL, NULL);

  return room_info_get_summary(home->living_room);
}
