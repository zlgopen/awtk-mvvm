/**
 * File:   device_factory.h
 * Author: AWTK Develop Team
 * Brief:  device_factory
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
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEVICE_FACTORY_H
#define TK_DEVICE_FACTORY_H

#include "tkc/object.h"

BEGIN_C_DECLS
typedef object_t* (*device_object_create_t)(const char* args);

ret_t device_factory_init(void);
bool_t device_factory_has(const char* type);
ret_t device_factory_unregister(const char* type);
object_t* device_factory_create_device(const char* type, const char* args);
ret_t device_factory_register(const char* type, device_object_create_t create);
ret_t device_factory_deinit(void);

END_C_DECLS

#endif /*TK_DEVICE_FACTORY_H*/
