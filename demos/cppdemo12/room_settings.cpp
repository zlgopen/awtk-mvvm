
/**
 * File:  temperature.cpp
 * Author: AWTK Develop Team
 * Brief:  temperature
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

#include "room_settings.h"
#include "mvvm/base/navigator.h"
#include "../common/room_settings_defines.h"

RoomSettings::RoomSettings(navigator_request_t* request) {
  this->request = request;

  this->room = object_get_prop_str(OBJECT(request), PROP_ROOM);
  this->temp = object_get_prop_float(OBJECT(request), PROP_TEMP, 0);
  this->humidity = object_get_prop_float(OBJECT(request), PROP_HUMIDITY, 0);
}

ret_t RoomSettings::Return(void) {
  value_t v;
  value_set_object(&v, OBJECT(this->request));
  object_set_prop_float(OBJECT(this->request), PROP_TEMP, this->temp);
  object_set_prop_float(OBJECT(this->request), PROP_HUMIDITY, this->humidity);

  return navigator_request_on_result(this->request, &v);
}
