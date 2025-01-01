/**
 * File:   room_info.h
 * Author: AWTK Develop Team
 * Brief:  room_info
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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef ROOM_INFO_H
#define ROOM_INFO_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class room_info_t
 *
 * 房间信息(用于在home和room_settings之间传递数据)。
 *
 */
typedef struct _room_info_t {
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
   * @property {char*} name
   * @annotation ["readable"]
   * 房间名称。
   */
  char* name;

  /*private*/
  str_t summary;
} room_info_t;

/**
 * @method room_info_create
 * 创建room_info对象。
 *
 * @param {const char*} name 名称。。
 * @annotation ["constructor"]
 * @return {room_info_t*} 返回room_info对象。
 */
room_info_t* room_info_create(const char* name);

/**
 * @method room_info_get_summary
 * 获取summary。
 *
 * @param {room_info_t*} room_info room_info对象。
 * @return {const char*} 返回summary。
 */
const char* room_info_get_summary(room_info_t* room_info);

/**
 * @method room_info_destroy
 * 销毁room_info对象。
 *
 * @annotation ["destructor"]
 * @param {room_info_t*} room_info room_info对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t room_info_destroy(room_info_t* room_info);

END_C_DECLS

#endif /*ROOM_INFO_H*/
