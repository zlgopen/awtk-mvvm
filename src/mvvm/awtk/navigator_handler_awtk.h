/**
 * File:   navigator_handler_awtk.c
 * Author: AWTK Develop Team
 * Brief:  navigator_handler awtk
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NAVIGATOR_HANDLER_AWTK_H
#define TK_NAVIGATOR_HANDLER_AWTK_H

#include "mvvm/base/model.h"
#include "mvvm/base/navigator.h"

BEGIN_C_DECLS

typedef struct _navigator_handler_awtk_t {
  navigator_handler_t navigator_handler;
  model_create_t model_create;
  void* model_create_args;
} navigator_handler_awtk_t;

navigator_handler_t* navigator_handler_awtk_create(model_create_t model_create,
                                                   void* model_create_args);
navigator_handler_t* navigator_handler_awtk_default_create(void);

#define NAVIGATOR_HANDLER_AWTK(h) ((navigator_handler_awtk_t*)h)

END_C_DECLS

#endif /*TK_NAVIGATOR_HANDLER_AWTK_H*/
