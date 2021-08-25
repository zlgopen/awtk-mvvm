/**
 * File:   controller.h
 * Author: AWTK Develop Team
 * Brief:  controller
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
 * 2021-05-27 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class controller_t
 *
 * @annotation ["model"]
 * 窗口控制器。
 *
 */
typedef struct _controller_t {
  /**
   * @property {bool_t} close_current
   * @annotation ["readable", "writable"]
   * 是否关闭当前窗口。
   */
  bool_t close_current;
} controller_t;

END_C_DECLS

#endif /*CONTROLLER_H*/
