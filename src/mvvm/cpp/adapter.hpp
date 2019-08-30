/**
 * File:  adapter
 * Author: AWTK Develop Team
 * Brief:  adapters between c and cpp
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
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ADAPTER_H
#define TK_VIEW_MODEL_ADAPTER_H

#include "mvvm/base/view_model.h"
#include "mvvm/base/value_validator.h"
#include "mvvm/base/value_converter.h"

#include "mvvm/cpp/view_model.hpp"
#include "mvvm/cpp/view_model_array.hpp"
#include "mvvm/cpp/value_converter.hpp"
#include "mvvm/cpp/value_validator.hpp"

/*将C++的ViewModel适配成C的ViewModel*/
view_model_t* view_model_cpp_create(ViewModel* cpp);

/*将C++的ViewModelArray适配成C的ViewModel*/
view_model_t* view_model_array_cpp_create(ViewModelArray* cpp);

/*将C++的ValueConverter适配成C的ValueConverter*/
value_converter_t* value_converter_cpp_create(ValueConverter* cpp);

/*将C++的ValueValidator适配成C的ValueValidator*/
value_validator_t* value_validator_cpp_create(ValueValidator* cpp);

#endif/*TK_VIEW_MODEL_ADAPTER_H*/

