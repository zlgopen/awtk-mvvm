/**
 * File:   mvvm_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  mvvm jerryscript global functions.
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_MVVM_JERRYSCRIPT_H
#define TK_MVVM_JERRYSCRIPT_H

#include "mvvm/base/mvvm_types_def.h"

BEGIN_C_DECLS

/**
 * @method mvvm_jerryscript_init
 * 初始化MVVM jerryscript。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_jerryscript_init(void);

/**
 * @method mvvm_jerryscript_run
 * 执行js代码。
 * @param {const char*} filename 文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_jerryscript_run(const char* filename);

/**
 * @method mvvm_jerryscript_deinit
 * ~初始化MVVM jerryscript。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_jerryscript_deinit(void);

END_C_DECLS

#endif /*TK_MVVM_JERRYSCRIPT_H*/
