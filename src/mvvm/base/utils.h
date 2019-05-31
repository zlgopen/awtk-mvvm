/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils
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
 * 2019-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_UTILS_H
#define TK_MVVM_UTILS_H

#include "tkc/str.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

const char* destruct_array_prop_name(const char* name, uint32_t* index);
ret_t str_random(str_t* str, const char* format, uint32_t max);

END_C_DECLS

#endif /*TK_MVVM_UTILS_H*/
