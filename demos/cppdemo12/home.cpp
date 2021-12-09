/**
 * File:  home.cpp
 * Author: AWTK Develop Team
 * Brief:  home
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
 * 2020-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "home.h"
#include "tkc/utils.h"
#include "mvvm/base/navigator.h"
#include "../common/room_settings_defines.h"

#define STR_HOME "home"
#define STR_BED_ROOM "bed_room"
#define STR_LIVING_ROOM "living_room"

RoomInfo::RoomInfo() {
  this->Init(40, 20);
}

void RoomInfo::Init(double humidity, double temp) {
  this->humidity = humidity;
  this->temp = temp;
}

Home::Home() {
  emitter_init(EMITTER(this));
  this->Sync();
}

Home::~Home() {
  emitter_deinit(EMITTER(this));
}

void Home::Sync() {
  char str[128];
  tk_snprintf(str, sizeof(str) - 1, "bed room: %g %g\n", this->bed_room.humidity,
              this->bed_room.temp);
  this->bed_room_info = str;

  tk_snprintf(str, sizeof(str) - 1, "living room: %g %g\n", this->living_room.humidity,
              this->living_room.temp);
  this->living_room_info = str;
}

ret_t Home::OnResult(navigator_request_t* req, const value_t* result) {
  tk_object_t* res = value_object(result);
  Home* h = (Home*)(tk_object_get_prop_pointer(TK_OBJECT(req), STR_HOME));
  const char* room_name = tk_object_get_prop_str(TK_OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM);
  RoomInfo* info = tk_str_eq(room_name, STR_BED_ROOM) ? &(h->bed_room) : &(h->living_room);

  info->temp = tk_object_get_prop_float(res, ROOM_SETTINGS_RESULT_TEMP, info->temp);
  info->humidity = tk_object_get_prop_float(res, ROOM_SETTINGS_RESULT_HUMIDITY, info->humidity);

  h->NotifyPropsChanged();

  return RET_OK;
}

void Home::NotifyPropsChanged(void) {
  this->Sync();
  emitter_dispatch_simple_event(this, EVT_PROPS_CHANGED);
}

ret_t Home::AdjustRoomSettings(const char* room_name, RoomInfo* info) {
  navigator_request_t* req = navigator_request_create("room_settings", Home::OnResult);

  tk_object_set_prop_pointer(TK_OBJECT(req), STR_HOME, this);
  tk_object_set_prop_str(TK_OBJECT(req), ROOM_SETTINGS_REQ_ARG_ROOM, room_name);
  tk_object_set_prop_float(TK_OBJECT(req), ROOM_SETTINGS_REQ_ARG_TEMP, info->temp);
  tk_object_set_prop_float(TK_OBJECT(req), ROOM_SETTINGS_REQ_ARG_HUMIDITY, info->humidity);

  navigator_to_ex(req);

  tk_object_unref(TK_OBJECT(req));

  return RET_OK;
}

ret_t Home::AdjustBedRoom(void) {
  return this->AdjustRoomSettings(STR_BED_ROOM, &(this->bed_room));
}

ret_t Home::AdjustLivingRoom(void) {
  return this->AdjustRoomSettings(STR_LIVING_ROOM, &(this->living_room));
}
