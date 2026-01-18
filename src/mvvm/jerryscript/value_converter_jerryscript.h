/**
 * File:   value_converter_jerryscript.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented value_converter
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_VALUE_CONVERTER_JERRYSCRIPT_H
#define TK_VALUE_CONVERTER_JERRYSCRIPT_H

#include "tkc/str.h"
#include "mvvm/base/value_converter.h"

BEGIN_C_DECLS

/**
 * @class value_converter_jerryscript_t
 * @parent value_converter_t
 *
 * 将jerryscript包装成值转换对象。
 *
 * JS的全局对象ValueConverters，记录了所有的ValueConverter。
 *
 */
typedef struct _value_converter_jerryscript_t {
  value_converter_t value_converter;

  /*避免每次动态分配内存*/
  str_t temp;
} value_converter_jerryscript_t;

#define VALUE_CONVERTER_JERRYSCRIPT(c) ((value_converter_jerryscript_t*)c)

/**
 * @method value_converter_jerryscript_init
 * 初始化jerryscript value converter，注册相应的工厂函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_jerryscript_init(void);

/**
 * @method value_converter_jerryscript_deinit
 * ~初始化jerryscript value converter。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_jerryscript_deinit(void);

END_C_DECLS

#endif /*TK_VALUE_CONVERTER_JERRYSCRIPT_H*/
