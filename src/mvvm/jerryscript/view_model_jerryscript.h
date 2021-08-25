/**
 * File:   view_model_jerryscript.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented view_model
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_JERRYSCRIPT_H
#define TK_VIEW_MODEL_JERRYSCRIPT_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @method view_model_jerryscript_init
 * 初始化jerryscript view model，注册相应的工厂函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_jerryscript_init(void);

/**
 * @method view_model_jerryscript_deinit
 * ~初始化jerryscript view model。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_jerryscript_deinit(void);

END_C_DECLS

#endif /*TK_VIEW_MODEL_JERRYSCRIPT_H*/
