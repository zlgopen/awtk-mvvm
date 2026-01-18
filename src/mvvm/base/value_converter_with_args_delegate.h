/**
 * File:   value_converter_with_args_delegate.c
 * Author: AWTK Develop Team
 * Brief:  value converter with args delegate
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
 * 2022-01-20 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_VALUE_CONVERTER_WITH_ARGS_DELEGATE_H
#define TK_VALUE_CONVERTER_WITH_ARGS_DELEGATE_H

#include "mvvm/base/value_converter.h"

BEGIN_C_DECLS

typedef ret_t (*value_convert_with_args_t)(const value_t* from, value_t* to, value_t args);

/**
 * @class value_converter_with_args_delegate_t
 * @parent value_converter_t
 *
 * 把两个转换函数包装成value_converter_t接口。
 *
 */
typedef struct _value_converter_with_args_delegate_t {
  value_converter_t value_converter;

  /*private*/
  value_convert_with_args_t to_view;
  value_convert_with_args_t to_model;
} value_converter_with_args_delegate_t;

/**
 * @method value_converter_with_args_delegate_create
 *
 * @annotation ["constructor"]
 * @param {value_convert_with_args_t} to_model 到模型的转换函数。
 * @param {value_convert_with_args_t} to_view 到视图的转换函数。
 *
 * @return {value_converter_t*} 返回value_converter对象。
 */
value_converter_t* value_converter_with_args_delegate_create(value_convert_with_args_t to_model,
                                                             value_convert_with_args_t to_view);

#define VALUE_CONVERTER_WITH_ARGS_DELEGATE(converter) ((value_converter_with_args_delegate_t*)(converter))

END_C_DECLS

#endif /*TK_VALUE_CONVERTER_WITH_ARGS_DELEGATE_H*/
