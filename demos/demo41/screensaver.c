/**
 * File:   screensaver.c
 * Author: AWTK Develop Team
 * Brief:  screensaver
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
 * 2020-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "screensaver.h"
#include "mvvm/base/navigator.h"

ret_t screensaver_set_time(screensaver_t* screensaver) {
  return_value_if_fail(screensaver != NULL, RET_BAD_PARAMS);

  navigator_set_screen_saver_time(screensaver->time);

  return RET_OK;
}

bool_t screensaver_can_set_time(screensaver_t* screensaver) {
  return_value_if_fail(screensaver != NULL, FALSE);

  return screensaver->time >= 1000;
}
