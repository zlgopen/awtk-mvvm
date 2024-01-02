/**
 * File:   device_factory.h
 * Author: AWTK Develop Team
 * Brief:  device_factory
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEVICE_FACTORY_H
#define TK_DEVICE_FACTORY_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class device_factory_t
 *
 * 设备工厂。
 *
 */

typedef tk_object_t* (*device_object_create_t)(const char* args);

/**
 * @method device_factory_init
 * 初始化。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t device_factory_init(void);

/**
 * @method device_factory_has
 * 检查是否存在。
 * @param {const char*} type 类型。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示FALSE表示不存在。
 */
bool_t device_factory_has(const char* type);

/**
 * @method device_factory_unregister
 * 注销。
 *
 * @param {const char*} type 类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t device_factory_unregister(const char* type);

/**
 * @method device_factory_create_device
 * 创建设备。
 *
 * @param {const char*} type 类型。
 * @param {const char*} args 参数。
 *
 * @return {object_t} 返回object对象。
 */
tk_object_t* device_factory_create_device(const char* type, const char* args);

/**
 * @method device_factory_register
 * 注册。
 *
 * @param {const char*} type 类型。
 * @param {device_object_create_t} create 创建回调函数指针。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t device_factory_register(const char* type, device_object_create_t create);

/**
 * @method device_factory_deinit
 * ~初始化。
 *
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t device_factory_deinit(void);

END_C_DECLS

#endif /*TK_DEVICE_FACTORY_H*/
