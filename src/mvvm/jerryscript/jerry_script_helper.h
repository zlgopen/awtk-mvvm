/**
 * File:   jerry_script_helper.h
 * Author: AWTK Develop Team
 * Brief:  jerry_script_helper
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/types_def.h"
#include "jerryscript.h"
#include "jerryscript-core.h"

#ifndef JERRY_SCRIPT_HELPER_H
#define JERRY_SCRIPT_HELPER_H

BEGIN_C_DECLS

ret_t jerry_script_init(void);

ret_t jerry_script_register_builtins(void);

void jerry_script_print_error(jerry_value_t error_value);

ret_t jerry_value_check(jerry_value_t value);
ret_t jerry_script_eval_file(const char* filename, bool_t global);
ret_t jerry_script_eval_buff(const char* script, uint32_t size, const char* filename,
                             bool_t global);

END_C_DECLS

#endif /*JERRY_SCRIPT_HELPER_H*/
