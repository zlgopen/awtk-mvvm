/**
 * File:   jerry_script_helper.h
 * Author: AWTK Develop Team
 * Brief:  jerry_script_helper
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef JERRY_SCRIPT_HELPER_H
#define JERRY_SCRIPT_HELPER_H

#include "jerryscript.h"
#include "jerryscript-port.h"
#include "jerryscript-ext/handler.h"
#include "tkc/types_def.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

ret_t jerry_script_init(void);
ret_t jerry_script_deinit(void);
void jerry_script_print_error(jerry_value_t error_value);

void jerry_init_external_context(void);
void jerry_deinit_external_context(void);

/**
 * @method jerry_script_eval_buff
 * annotation ["global"]
 * export for test only
 */
ret_t jerry_script_eval_buff(const char* script, uint32_t size, const char* filename,
                             bool_t global);

ret_t jerry_script_eval_file(const char* filename, bool_t global);
ret_t jerry_script_eval_snapshot(const uint8_t* snapshot, uint32_t size);

/**
 * @method jerry_value_check
 * annotation ["global"]
 * export for test only
 */
ret_t jerry_value_check(jerry_value_t value);

END_C_DECLS

#endif /*JERRY_SCRIPT_HELPER_H*/
