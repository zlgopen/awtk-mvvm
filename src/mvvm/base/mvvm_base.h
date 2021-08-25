/**
 * File:   mvvm_base.c
 * Author: BASE Develop Team
 * Brief:  mvvm base global functions.
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_BASE_H
#define TK_MVVM_BASE_H

#include "mvvm/base/view_model.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_app_conf.h"
#include "mvvm/base/view_model_object_wrapper.h"
#include "mvvm/base/view_model_array_object_wrapper.h"
#include "mvvm/base/binding_rule.h"
#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/items_binding.h"
#include "mvvm/base/condition_binding.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/custom_binder.h"
#include "mvvm/base/navigator_request.h"
#include "mvvm/base/navigator_handler.h"
#include "mvvm/base/navigator.h"
#include "mvvm/base/value_validator.h"
#include "mvvm/base/value_validator_delegate.h"
#include "mvvm/base/value_converter.h"
#include "mvvm/base/value_converter_delegate.h"

BEGIN_C_DECLS

/**
 * @method mvvm_base_init
 * 初始化MVVM base
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_base_init(void);

/**
 * @method mvvm_base_deinit
 * ~初始化MVVM base
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_base_deinit(void);

END_C_DECLS

#endif /*TK_MVVM_BASE_H*/
