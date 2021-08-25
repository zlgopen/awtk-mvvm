/**
 * File:   custom_binder.h
 * Author: AWTK Develop Team
 * Brief:  custom_binder
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CUSTOM_BINDER_H
#define TK_CUSTOM_BINDER_H

#include "mvvm/base/binding_context.h"

BEGIN_C_DECLS

/**
 * @class custom_binder_t
 *
 * 自定义绑定器。
 *
 */
typedef struct _custom_binder_t {
  object_t* binders;
} custom_binder_t;

/**
 * @method custom_binder_init
 * 初始化自定义绑定器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_binder_init(void);

/**
 * @method custom_binder_exist
 * 检查自定义绑定器是否存在。
 * @param {const char*} type 控件的类型。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t custom_binder_exist(const char* type);

/**
 * @method custom_binder_register
 * 注册模型的自定义绑定器。
 * @param {const char*} type 控件的类型。
 * @param {custom_bind_t} bind 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_binder_register(const char* type, binding_context_bind_rule_t bind);

/**
 * @method custom_binder_unregister
 * 注销自定义绑定器。
 * @param {const char*} type 控件的类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_binder_unregister(const char* type);

/**
 * @method custom_binder_bind
 * 自定义绑定。
 * @param {const char*} type 模型的类型。
 * @param {void*} widget 控件。
 * @param {binding_context_t*} ctx ctx对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_binder_bind(const char* type, binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method custom_binder_deinit
 * ~初始化自定义绑定器。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_binder_deinit(void);

#define CUSTOM_BINDER(custom_binder) ((custom_binder_t*)(custom_binder))

END_C_DECLS

#endif /*TK_CUSTOM_BINDER_H*/
