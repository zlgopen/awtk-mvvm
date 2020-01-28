/**
 * File:   room_settings.h
 * Author: AWTK Develop Team
 * Brief:  room_settings
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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef ROOM_SETTINGS_H
#define ROOM_SETTINGS_H

#include "mvvm/base/navigator_request.h"

BEGIN_C_DECLS

/**
 * @class room_settings_t
 *
 * @annotation ["model"]
 * 温度控制器。
 *
 */
typedef struct _room_settings_t {
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
   * @property {char*} room
   * @annotation ["readable", "writable"]
   * 房间名称。
   */
  char* room;

  /*private*/
  navigator_request_t* req;
} room_settings_t;

/**
 * @method room_settings_create
 * 创建room_settings对象。
 *
 * @param {navigator_request_t*} req 请求对象。
 * @annotation ["constructor"]
 * @return {room_settings_t*} 返回room_settings对象。
 */
room_settings_t* room_settings_create(navigator_request_t* req);

/**
 * @method room_settings_return
 * 返回值。
 *
 * @annotation ["command"]
 * @param {room_settings_t*} room_settings room_settings对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t room_settings_return(room_settings_t* room_settings);

/**
 * @method room_settings_destroy
 * 销毁room_settings对象。
 *
 * @annotation ["destructor"]
 * @param {room_settings_t*} room_settings room_settings对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t room_settings_destroy(room_settings_t* room_settings);

END_C_DECLS

#endif /*ROOM_SETTINGS_H*/
