/**
 * File:   device_object.h
 * Author: AWTK Develop Team
 * Brief:  device_object
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-07-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEVICE_OBJECT_H
#define TK_DEVICE_OBJECT_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _device_object_t;
typedef struct _device_object_t device_object_t;

/**
 * @class device_object_t
 * @parent tk_object_t
 *
 * 外设。
 *
 *> 目前device\_object并没有做实际的工作，引入device\_object的目的是保持概念一致性。
 */
struct _device_object_t {
  tk_object_t object;
};

#define DEVICE_TK_OBJECT(object) ((device_object_t*)(object))

END_C_DECLS

#endif /*TK_DEVICE_OBJECT_H*/
