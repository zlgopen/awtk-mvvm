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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/view_model.h"
#include "mvvm/base/view_model_array.h"
#include "mvvm/base/value_validator.h"
#include "mvvm/base/value_converter.h"

#include "mvvm/cpp/view_model.hpp"
#include "mvvm/cpp/view_model_array.hpp"
#include "mvvm/cpp/value_converter.hpp"
#include "mvvm/cpp/value_validator.hpp"

#ifdef WIN32
#ifdef MVVM_DLL_EXPORT
#define MVVM_API __declspec(dllexport)
#else
#define MVVM_API __declspec(dllimport)
#endif
#else
#define MVVM_API 
#endif

namespace vm {

/*将C++的ViewModel适配成C的ViewModel*/
 MVVM_API view_model_t* To(ViewModel* cpp);

/*将C++的ViewModelArray适配成C的ViewModel*/
 MVVM_API view_model_t* To(ViewModelArray* cpp);

/*将C++的ValueConverter适配成C的ValueConverter*/
 MVVM_API value_converter_t* To(ValueConverter* cpp);

/*将C++的ValueValidator适配成C的ValueValidator*/
 MVVM_API value_validator_t* To(ValueValidator* cpp);

} /*namespae vm*/

#endif/*TK_VIEW_MODEL_ADAPTER_H*/

