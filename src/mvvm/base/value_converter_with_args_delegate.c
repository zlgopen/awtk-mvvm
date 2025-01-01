/**
 * File:   value_converter_with_args_delegate.c
 * Author: AWTK Develop Team
 * Brief:  value converter with args delegate
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "mvvm/base/value_converter_with_args_delegate.h"

static const object_vtable_t s_value_converter_with_args_delegate_vtable = {
    .type = "value_converter_with_args_delegate",
    .desc = "value_converter_with_args_delegate",
    .size = sizeof(value_converter_with_args_delegate_t),
    .is_collection = FALSE};

static ret_t value_converter_with_args_delegate_to_view(value_converter_t* c, const value_t* from,
                                                        value_t* to) {
  value_converter_with_args_delegate_t* value_convert_delegate = VALUE_CONVERTER_WITH_ARGS_DELEGATE(c);

  return value_convert_delegate->to_view(from, to, c->args);
}

static ret_t value_converter_with_args_delegate_to_model(value_converter_t* c, const value_t* from,
                                                         value_t* to) {
  value_converter_with_args_delegate_t* value_convert_delegate = VALUE_CONVERTER_WITH_ARGS_DELEGATE(c);

  return value_convert_delegate->to_model(from, to, c->args);
}

value_converter_t* value_converter_with_args_delegate_create(value_convert_with_args_t to_model,
                                                             value_convert_with_args_t to_view) {
  tk_object_t* obj = NULL;
  value_converter_t* value_convert = NULL;
  value_converter_with_args_delegate_t* value_convert_delegate = NULL;
  return_value_if_fail(to_model != NULL || to_view != NULL, NULL);

  obj = tk_object_create(&s_value_converter_with_args_delegate_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_CONVERTER(obj);
  value_convert->to_view = to_view != NULL ? value_converter_with_args_delegate_to_view : value_converter_delegate_unconvert;
  value_convert->to_model =  to_model != NULL? value_converter_with_args_delegate_to_model : value_converter_delegate_unconvert;

  value_convert_delegate = VALUE_CONVERTER_WITH_ARGS_DELEGATE(obj);
  value_convert_delegate->to_view = to_view;
  value_convert_delegate->to_model = to_model;

  return value_convert;
}
