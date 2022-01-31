/**
 * File:   mvvm_awtk.c
 * Author: AWTK Develop Team
 * Brief:  mvvm awtk global functions.
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
 * 2019-02-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_AWTK_H
#define TK_MVVM_AWTK_H

#include "base/types_def.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "mvvm/awtk/navigator_handler_awtk.h"

BEGIN_C_DECLS

/**
 * @method mvvm_awtk_init
 * 初始化MVVM awtk
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_awtk_init(void);

/**
 * @method mvvm_awtk_deinit
 * ~初始化MVVM awtk
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_awtk_deinit(void);

END_C_DECLS

#endif /*TK_MVVM_AWTK_H*/
