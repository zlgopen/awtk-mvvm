/**
 * File:   window1.c
 * Author: AWTK Develop Team
 * Brief:  window1 source
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-08-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "window1.h"

static ret_t on_close_window(void* ctx, event_t* e) {
  window_close(WIDGET(ctx));

  return RET_OK;
}

ret_t window1_open(void) {
  widget_t* win = window_open("window1");

  widget_child_on(win, "close", EVT_CLICK, on_close_window, win);

  return RET_OK;
}
