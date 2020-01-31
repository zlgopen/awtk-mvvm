/**
 * File:  home.h
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

#ifndef TK_HOME_H
#define TK_HOME_H

#include <string>
#include "tkc/types_def.h"
#include "mvvm/base/navigator_request.h"

using std::string;

class RoomInfo {
 public:
  RoomInfo();
  void Init(double humidity, double temp);

  double humidity;
  double temp;
};

/**
 * @class Home
 * @parent emitter_t
 * @annotation ["model", "cpp"]
 * 房间控制器。
 *
 */
class Home : public emitter_t {
 public:
  /**
   * @method Home
   * 构造函数。
   *
   * @annotation ["constructor"]
   * @return {void} 返回无。
   */
  Home();
  ~Home();

 public:
  /**
   * @property {string} bed_room_info
   * @annotation ["readable", "writable"]
   * bed_room_info。
   */
  string bed_room_info;

  /**
   * @property {string} living_room_info
   * @annotation ["readable", "writable"]
   * living_room_info。
   */
  string living_room_info;

 public:
  /**
   * @method AdjustBedRoom
   * 设置客厅参数。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t AdjustBedRoom(void);

  /**
   * @method AdjustLivingRoom
   * 设置客厅参数。
   *
   * @annotation ["command"]
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t AdjustLivingRoom(void);

  void NotifyObjectChanged(void);
  ret_t AdjustRoomSettings(const char* room_name, RoomInfo* info);
  static ret_t OnResult(navigator_request_t* req, const value_t* result);

 private:
  void Sync();

 private:
  RoomInfo bed_room;
  RoomInfo living_room;
};

#endif /*TK_HOME_H*/
