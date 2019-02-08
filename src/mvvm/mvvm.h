/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_H
#define TK_MVVM_H

#include "mvvm/base/model.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/base/navigator_request.h"
#include "mvvm/base/view_model_normal.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/view_model.h"
#include "mvvm/base/binding_rule.h"
#include "mvvm/base/navigator_handler.h"
#include "mvvm/base/value_validator.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/value_converter.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/navigator.h"
#include "mvvm/base/model_delegate.h"
#include "mvvm/base/value_validator_delegate.h"
#include "mvvm/base/value_converter_delegate.h"
#include "mvvm/awtk/mvvm_awtk.h"

#ifdef WITH_JERRYSCRIPT
#include "mvvm/jerryscript/mvvm_jerryscript.h"
#endif /*WITH_JERRYSCRIPT*/

BEGIN_C_DECLS

/**
 * @method mvvm_init
 * 初始化MVVM。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_init(void);

/**
 * @method mvvm_deinit
 * ~初始化MVVM。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_deinit(void);

#define NAVIGATOR_ADD_HANDLER(name, model_create)           \
  navigator_register_handler(navigator(), name,             \
                             navigator_handler_awtk_create( \
                                 model_create != NULL ? model_create : model_dummy_create, NULL))

END_C_DECLS

#endif /*TK_MVVM_H*/
