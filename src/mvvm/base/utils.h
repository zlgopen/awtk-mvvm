/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils
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
 * 2019-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_UTILS_H
#define TK_MVVM_UTILS_H

#include "tkc/str.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @method tk_destruct_array_prop_name
 * 从完整属性名中取出属性名和索引。
 * @annotation ["global"]
 * @param {const char*} name 属性名。
 * @param {uint32_t*} index  返回索引。
 *
 * @return {const char*} 返回属性名。
 */
const char* tk_destruct_array_prop_name(const char* name, uint32_t* index);

/**
 * @method str_random
 * 生产一个随机字符串。
 *
 * @param {str_t*} str str对象。
 * @param {const char*} format格式。
 * @param {uint32_t} max 最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_random(str_t* str, const char* format, uint32_t max);

bool_t tk_is_valid_prop_name(const char* name);

#define destruct_array_prop_name tk_destruct_array_prop_name

END_C_DECLS

#endif /*TK_MVVM_UTILS_H*/
