/**
 * File:   screensaver.h
 * Author: AWTK Develop Team
 * Brief:  screensaver
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
 * 2020-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class screensaver_t
 *
 * @annotation ["model"]
 * 屏保时间设定。
 *
 */
typedef struct _screensaver_t {
  /**
   * @property {int32_t} time
   * @annotation ["readable", "writable"]
   * 时间。
   */
  int32_t time;
} screensaver_t;

/**
 * @method screensaver_apply
 * 设置屏保时间。
 * @annotation ["command"]
 * @param {screensaver_t*} screensaver screensaver对象
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t screensaver_apply(screensaver_t* screensaver);

/**
 * @method screensaver_can_apply
 * 检查 screensaver_apply 是否可以执行。
 *
 * @param {screensaver_t*} screensaver screensaver对象
 *
 * @return {ret_t} 返回TRUE表示成功，否则表示失败。
 */
bool_t screensaver_can_apply(screensaver_t* screensaver);

END_C_DECLS

#endif /*SCREENSAVER_H*/
