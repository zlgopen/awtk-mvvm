/**
 * File:   jerryscript_awtk.h
 * Author: AWTK Develop Team
 * Brief:  awtk for jerryscript
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
 * 2019-02-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_JERRYSCRIPT_AWTK_H
#define TK_JERRYSCRIPT_AWTK_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @method jerryscript_awtk_init
 * 初始化jerryscript awtk related stuff。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jerryscript_awtk_init(void);

/**
 * @method jerryscript_awtk_deinit
 * 初始化jerryscript awtk related stuff。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t jerryscript_awtk_deinit(void);

END_C_DECLS

#endif /*TK_JERRYSCRIPT_AWTK_H*/
