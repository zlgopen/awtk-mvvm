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

/**
 * @class navigator_handler_awtk_t
 * @parent navigator_handler_t
 *
 * 基于AWTK实现的导航处理器，负责打开指定的窗口。
 *
 */
typedef struct _navigator_handler_awtk_t {
  navigator_handler_t navigator_handler;
  model_create_t model_create;
  void* model_create_args;
} navigator_handler_awtk_t;

/**
 * @method navigator_handler_awtk_create
 * 创建navigator_handler对象(主要给C/C++使用)。
 *
 * @param {model_create_t} model_create 模型创建函数。
 * @param {void*} model_create_args 模型创建函数的参数。
 *
 * @return {navigator_handler_t*} 返回navigator_handler对象。
 */
navigator_handler_t* navigator_handler_awtk_create(model_create_t model_create,
                                                   void* model_create_args);

/**
 * @method navigator_handler_awtk_default_create
 * 创建navigator_handler对象(主要给脚本和DLL使用)。
 *
 * @return {navigator_handler_t*} 返回navigator_handler对象。
 */
navigator_handler_t* navigator_handler_awtk_default_create(void);

#define NAVIGATOR_HANDLER_AWTK(h) ((navigator_handler_awtk_t*)h)

END_C_DECLS

#endif /*TK_NAVIGATOR_HANDLER_AWTK_H*/
