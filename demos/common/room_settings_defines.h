/**
 * File:   room_settings.h
 * Author: AWTK Develop Team
 * Brief:  room_settings
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
 * 2019-02-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ROOM_SETTINGS_DEFINES_H
#define TK_ROOM_SETTINGS_DEFINES_H

#define PROP_TEMP "temp"
#define PROP_ROOM "room"
#define PROP_HUMIDITY "humidity"

/*请求的参数*/
#define ROOM_SETTINGS_REQ_ARG_TEMP PROP_TEMP
#define ROOM_SETTINGS_REQ_ARG_ROOM PROP_ROOM
#define ROOM_SETTINGS_REQ_ARG_HUMIDITY PROP_HUMIDITY

/*返回的参数*/
#define ROOM_SETTINGS_RESULT_TEMP PROP_TEMP
#define ROOM_SETTINGS_RESULT_HUMIDITY PROP_HUMIDITY

#define ROOM_SETTINGS(t) ((room_settings_t*)(t))

#endif /*TK_ROOM_SETTINGS_DEFINES_H*/
