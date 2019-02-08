/**
 * File:   main.h
 * Author: AWTK Develop Team
 * Brief:  main
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

#ifndef TK_MAIN_H
#define TK_MAIN_H

#include "mvvm/base/model.h"

BEGIN_C_DECLS

/**
 * @method main_create
 * 创建main对象。
 *
 * @annotation ["constructor"]
 *
 * @return {model_t} 返回model_t对象。
 */
model_t* main_create(void* args);

END_C_DECLS

#endif /*TK_MAIN_H*/
