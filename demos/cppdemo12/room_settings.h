/**
 * File:  temperature.h
 * Author: AWTK Develop Team
 * Brief:  temperature
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
 * 2020-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ROOM_SETTINGS_H
#define TK_ROOM_SETTINGS_H

#include <string>
#include "mvvm/base/navigator_request.h"

using std::string;

/**
 * @class RoomSettings
 *
 * @annotation ["model", "cpp"]
 * 房间设置。
 *
 */
class RoomSettings {
 public:
  /**
   * @method RoomSettings
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @param {navigator_request_t*} request
   * @return {void} 返回无。
   */
  RoomSettings(navigator_request_t* request);

 public:
  /**
   * @property {double} temp
   * @annotation ["readable", "writable"]
   * 温度。
   */
  double temp;

  /**
   * @property {double} humidity
   * @annotation ["readable", "writable"]
   * 湿度。
   */
  double humidity;

  /**
   * @property {string} room
   * @annotation ["readable", "writable"]
   * 房间。
   */
  string room;

 public:
  /**
   * @method Return
   * 返回设置给调用者。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Return(void);

 private:
  navigator_request_t* request;
};

#endif /*TK_ROOM_SETTINGS_H*/
