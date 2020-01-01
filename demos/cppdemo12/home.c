/**
 * File:   home.c
 * Author: AWTK Develop Team
 * Brief:  home
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/mvvm.h"
#include "tkc/object_default.h"

#include "home.h"
#include "../common/room_settings_defines.h"

#define STR_HOME "home"
#define STR_BED_ROOM "Bed Room"
#define STR_LIVING_ROOM "Living Room"

#define PROP_BED_ROOM_INFO "bed_room_info"
#define PROP_LIVING_ROOM_INFO "living_room_info"

#define CMD_QUIT "quit"
#define CMD_ADJUST_BED_ROOM "adjustBedRoom"
#define CMD_ADJUST_LIVING_ROOM "adjustLivingRoom"

static ret_t home_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t home_get_prop(object_t* obj, const char* name, value_t* v) {
  char info[128];
  home_t* h = HOME(obj);

  if (tk_str_eq(PROP_BED_ROOM_INFO, name)) {
    tk_snprintf(info, sizeof(info) - 1, "bedroom: temp=%.2f humi=%.2f", h->bed_room.temp,
                h->bed_room.humidity);
    value_dup_str(v, info);
    return RET_OK;
  } else if (tk_str_eq(PROP_LIVING_ROOM_INFO, name)) {
    tk_snprintf(info, sizeof(info) - 1, "livingroom: temp=%.2f humi=%.2f", h->living_room.temp,
                h->living_room.humidity);
    value_dup_str(v, info);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static bool_t home_can_exec(object_t* obj, const char* name, const char* args) {
  return TRUE;
}

static ret_t home_on_room_result(navigator_request_t* req, const value_t* result) {
  object_t* res = value_object(result);
  home_t* h = HOME(object_get_prop_pointer(OBJECT(req), STR_HOME));
  const char* room_name = object_get_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM);
  room_info_t* info = tk_str_eq(room_name, STR_BED_ROOM) ? &(h->bed_room) : &(h->living_room);

  info->temp = object_get_prop_float(res, ROOM_SETTINGS_RESULT_TEMP, info->temp);
  info->humidity = object_get_prop_float(res, ROOM_SETTINGS_RESULT_HUMIDITY, info->humidity);

  object_notify_changed(OBJECT(h));

  return RET_OK;
}

static ret_t adjust_room_params(home_t* h, const char* room_name, room_info_t* info) {
  navigator_request_t* req = navigator_request_create("room_settings", home_on_room_result);

  object_set_prop_pointer(OBJECT(req), STR_HOME, h);
  object_set_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM, room_name);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_TEMP, info->temp);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_HUMIDITY, info->humidity);

  navigator_to_ex(req);

  object_unref(OBJECT(req));

  return RET_OK;
}

static ret_t home_exec(object_t* obj, const char* name, const char* args) {
  home_t* h = HOME(obj);
  ret_t ret = RET_NOT_FOUND;

  if (tk_str_ieq(name, CMD_ADJUST_BED_ROOM)) {
    ret = adjust_room_params(h, STR_BED_ROOM, &(h->bed_room));
  } else if (tk_str_ieq(name, CMD_ADJUST_LIVING_ROOM)) {
    ret = adjust_room_params(h, STR_LIVING_ROOM, &(h->living_room));
  } else if (tk_str_ieq(name, CMD_QUIT)) {
    tk_quit();
  } else {
    log_debug("not supported\n");
  }

  object_notify_changed(obj);

  return ret;
}

static const object_vtable_t s_home_vtable = {.type = "home",
                                              .desc = "home",
                                              .size = sizeof(home_t),
                                              .exec = home_exec,
                                              .can_exec = home_can_exec,
                                              .get_prop = home_get_prop,
                                              .set_prop = home_set_prop};

view_model_t* home_create(navigator_request_t* req) {
  view_model_t* view_model = VIEW_MODEL(object_create(&s_home_vtable));
  home_t* h = HOME(view_model);
  return_value_if_fail(h != NULL, NULL);

  h->bed_room.temp = 10;
  h->bed_room.humidity = 20;

  h->living_room.temp = 15;
  h->living_room.humidity = 25;

  return view_model;
}
