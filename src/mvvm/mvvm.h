﻿/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_H
#define TK_MVVM_H

#include "mvvm/base/mvvm_base.h"
#include "mvvm/awtk/mvvm_awtk.h"
#include "mvvm/awtk/binding_context_awtk.h"

#ifdef WITH_JERRYSCRIPT
#include "mvvm/jerryscript/mvvm_jerryscript.h"
#endif /*WITH_JERRYSCRIPT*/

#ifdef WITH_IOTJS
#include "mvvm/iotjs/mvvm_iotjs.h"
#endif /*WITH_IOTJS*/

BEGIN_C_DECLS

/**
 * @method mvvm_init
 * 初始化MVVM。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_init(void);

/**
 * @method mvvm_deinit
 * ~初始化MVVM。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_deinit(void);

END_C_DECLS

#endif /*TK_MVVM_H*/
