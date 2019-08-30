
/**
 * File:  home.cpp
 * Author: AWTK Develop Team
 * Brief:  home view model
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "home.hpp"
#include "mvvm/base/navigator.h"
#include "../common/room_settings_defines.h"

#define STR_HOME "home"
#define STR_BED_ROOM "Bed Room"
#define STR_LIVING_ROOM "Living Room"

#define PROP_BED_ROOM_INFO "bed_room_info"
#define PROP_LIVING_ROOM_INFO "living_room_info"

#define CMD_QUIT "quit"
#define CMD_ADJUST_BED_ROOM "adjustBedRoom"
#define CMD_ADJUST_LIVING_ROOM "adjustLivingRoom"

Home::Home(navigator_request_t* request) : ViewModel(request) {
  this->bed_room.temp = 10;
  this->bed_room.humidity = 20;

  this->living_room.temp = 15;
  this->living_room.humidity = 25;
}

Home::~Home() {
}

ret_t Home::OnResult(navigator_request_t* req, const value_t* result) {
  object_t* res = value_object(result);
  Home* h = (Home*)(object_get_prop_pointer(OBJECT(req), STR_HOME));
  const char* room_name = object_get_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM);
  RoomInfo* info = tk_str_eq(room_name, STR_BED_ROOM) ? &(h->bed_room) : &(h->living_room);

  info->temp = object_get_prop_float(res, ROOM_SETTINGS_RESULT_TEMP, info->temp);
  info->humidity = object_get_prop_float(res, ROOM_SETTINGS_RESULT_HUMIDITY, info->humidity);

  h->NotifyObjectChanged();

  return RET_OK;
}

ret_t Home::AdjustRoomSettings(const char* room_name, RoomInfo* info) {
  navigator_request_t* req = navigator_request_create("room_settings", Home::OnResult);

  object_set_prop_pointer(OBJECT(req), STR_HOME, this);
  object_set_prop_str(OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM, room_name);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_TEMP, info->temp);
  object_set_prop_float(OBJECT(req), ROOM_SETTINGS_REQ_ARG_HUMIDITY, info->humidity);

  navigator_to_ex(req);

  object_unref(OBJECT(req));

  return RET_OK;
}

ret_t Home::Exec(const char* name, const char* args) {
  ret_t ret = RET_OBJECT_CHANGED;

  if (tk_str_ieq(name, CMD_ADJUST_BED_ROOM)) {
    ret = this->AdjustRoomSettings(STR_BED_ROOM, &(this->bed_room));
  } else if (tk_str_ieq(name, CMD_ADJUST_LIVING_ROOM)) {
    ret = this->AdjustRoomSettings(STR_LIVING_ROOM, &(this->living_room));
  } else if (tk_str_ieq(name, CMD_QUIT)) {
    tk_quit();
  } else {
    log_debug("not supported\n");
    ret = RET_NOT_FOUND;
  }

  return ret;
}

bool_t Home::CanExec(const char* name, const char* args) const {
  return TRUE;
}

ret_t Home::GetProp(const char* name, value_t* v) const {
  char info[256];
  if (tk_str_eq(PROP_BED_ROOM_INFO, name)) {
    tk_snprintf(info, sizeof(info) - 1, "bedroom: temp=%.2f humi=%.2f", this->bed_room.temp,
                this->bed_room.humidity);
    value_dup_str(v, info);
    return RET_OK;
  } else if (tk_str_eq(PROP_LIVING_ROOM_INFO, name)) {
    tk_snprintf(info, sizeof(info) - 1, "livingroom: temp=%.2f humi=%.2f", this->living_room.temp,
                this->living_room.humidity);
    value_dup_str(v, info);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t Home::SetProp(const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

view_model_t* home_view_model_create(navigator_request_t* req) {
  return vm::To(new Home(req));
}
