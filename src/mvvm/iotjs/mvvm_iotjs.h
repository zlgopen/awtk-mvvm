/**
 * File:   mvvm_iotjs.h
 * Author: AWTK Develop Team
 * Brief:  mvvm iotjs global functions.
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
 * 2022-02-14 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef MVVM_IOTJS_H
#define MVVM_IOTJS_H

#include "mvvm/base/mvvm_types_def.h"
#include "mvvm/jerryscript/jerry_script_helper.h"

BEGIN_C_DECLS

/**
 * @method mvvm_iotjs_init
 * 初始化 MVVM iotjs。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_iotjs_init(void);

/**
 * @method mvvm_iotjs_deinit
 * 释放 MVVM iotjs。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_iotjs_deinit(void);


/*private*/
ret_t mvvm_iotjs_run(void);
ret_t mvvm_iotjs_get_iotjs_require(char* name, jerry_value_t* require);

END_C_DECLS

#endif
