/**
 * File:   temperature.h
 * Author: AWTK Develop Team
 * Brief:  temperature
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

#ifndef TK_HOME_H
#define TK_HOME_H

#include "mvvm/base/model.h"

BEGIN_C_DECLS

typedef struct _room_info_t {
  double temp;
  double humidity;
} room_info_t;

/**
 * @class home_t
 *
 * home对象。
 *
 */
typedef struct _home_t {
  model_t model;

  room_info_t bed_room;
  room_info_t living_room;
} home_t;

model_t* home_create(navigator_request_t* req);

#define HOME(t) ((home_t*)(t))

END_C_DECLS

#endif /*TK_HOME_H*/
