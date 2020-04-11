/**
 * File:   home.h
 * Author: AWTK Develop Team
 * Brief:  home
 *
 * Copyright (c) 2012 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HOME_H
#define TK_HOME_H

#include "tkc/str.h"
#include "tkc/emitter.h"
#include "room_info.h"

BEGIN_C_DECLS

/**
 * @class home_t
 * @parent emitter_t
 *
 * @annotation ["model"]
 * 房间控制器。
 *
 */
typedef struct _home_t {
  emitter_t emitter;

  /**
   * @property {str_t} bed_room_info
   * @annotation ["readable", "fake"]
   * bed_room_info。
   */

  /**
   * @property {str_t} living_room_info
   * @annotation ["readable", "fake"]
   * living_room_info。
   */

  /*private*/
  room_info_t* bed_room;
  room_info_t* living_room;
} home_t;

/**
 * @method home_create
 * 创建home对象。
 *
 * @annotation ["constructor"]
 * @return {home_t*} 返回home对象。
 */
home_t* home_create(void);

/**
 * @method home_destroy
 * 销毁home对象。
 *
 * @annotation ["destructor"]
 * @param {home_t*} home home对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t home_destroy(home_t* home);

/**
 * @method home_adjust_bed_room
 * 设置卧室参数
 *
 * @annotation ["command:adjustBedRoom"]
 * @param {home_t*} home home对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t home_adjust_bed_room(home_t* home);

/**
 * @method home_adjust_living_room
 * 设置客厅参数。
 *
 * @annotation ["command:adjustLivingRoom"]
 * @param {home_t*} home home对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t home_adjust_living_room(home_t* home);

/**
 * @method home_get_bed_room_info
 * 获取卧室参数。
 *
 * @param {home_t*} home home对象。
 *
 * @return {const char*} 返回卧室参数。
 */
const char* home_get_bed_room_info(home_t* home);

/**
 * @method home_get_living_room_info
 * 获取客厅参数。
 *
 * @param {home_t*} home home对象。
 *
 * @return {const char*} 返回客厅参数。
 */
const char* home_get_living_room_info(home_t* home);

END_C_DECLS

#endif /*TK_HOME_H*/
