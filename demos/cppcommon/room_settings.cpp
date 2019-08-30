
/**
 * File:  room_settings.cpp
 * Author: AWTK Develop Team
 * Brief:  room_settings view model
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "room_settings.hpp"
#include "../common/room_settings_defines.h"

#define CMD_RETURN "return"

RoomSettings::RoomSettings(navigator_request_t* request) : ViewModel(request) {
  this->room = NULL;
}

RoomSettings::~RoomSettings() {
  TKMEM_FREE(this->room);
}

ret_t RoomSettings::Exec(const char* name, const char* args) {
  if (tk_str_eq(name, CMD_RETURN)) {
    value_t v;
    value_set_object(&v, OBJECT(this->request));
    object_set_prop_float(OBJECT(this->request), PROP_TEMP, this->temp);
    object_set_prop_float(OBJECT(this->request), PROP_HUMIDITY, this->humidity);

    return navigator_request_on_result(this->request, &v);
  }

  return RET_NOT_IMPL;
}

bool_t RoomSettings::CanExec(const char* name, const char* args) const {
  return TRUE;
}

ret_t RoomSettings::GetProp(const char* name, value_t* v) const {
  if (tk_str_eq(PROP_TEMP, name)) {
    value_set_double(v, this->temp);

    return RET_OK;
  } else if (tk_str_eq(PROP_HUMIDITY, name)) {
    value_set_double(v, this->humidity);

    return RET_OK;
  } else if (tk_str_eq(PROP_ROOM, name)) {
    value_set_str(v, this->room);

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t RoomSettings::SetProp(const char* name, const value_t* v) {
  if (tk_str_eq(PROP_TEMP, name)) {
    this->temp = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(PROP_HUMIDITY, name)) {
    this->humidity = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(PROP_ROOM, name)) {
    this->room = tk_str_copy(this->room, value_str(v));
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

view_model_t* room_settings_view_model_create(navigator_request_t* request) {
  view_model_t* view_model = vm::To(new RoomSettings(request));

  object_copy_prop(OBJECT(view_model), OBJECT(request), PROP_ROOM);
  object_copy_prop(OBJECT(view_model), OBJECT(request), PROP_TEMP);
  object_copy_prop(OBJECT(view_model), OBJECT(request), PROP_HUMIDITY);

  return view_model;
}
