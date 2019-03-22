/**
 * File:   room_settings.h
 * Author: AWTK Develop Team
 * Brief:  room_settings
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
 * 2019-02-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ROOM_SETTINGS_H
#define TK_ROOM_SETTINGS_H

#include "mvvm/base/view_model.h"
#include "room_settings_defines.h"

BEGIN_C_DECLS

/**
 * @class room_settings_t
 *
 * 空气调节器对象。
 *
 */
typedef struct _room_settings_t {
  view_model_t view_model;

  /*房间名称*/
  char* room;
  /*温度*/
  double temp;
  /*湿度*/
  double humidity;

  navigator_request_t* req;
} room_settings_t;

/**
 * @method room_settings_create
 * 创建room_settings对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* room_settings_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_ROOM_SETTINGS_H*/
