/**
 * File:   model_factory.h
 * Author: AWTK Develop Team
 * Brief:  model factory
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MODEL_FACTORY_H
#define TK_MODEL_FACTORY_H

#include "mvvm/base/model.h"

BEGIN_C_DECLS

/**
 * @class model_factory_t
 *
 * 模型工厂。
 *
 */
typedef struct _model_factory_t {
  object_t* creators;
} model_factory_t;

/**
 * @method model_factory_init
 * 初始化模型工厂。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_factory_init(void);

/**
 * @method model_factory_exist
 * 检查指定的模型是否存在。
 * @param {const char*} type 模型的类型。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t model_factory_exist(const char* type);

/**
 * @method model_factory_register
 * 注册模型的创建函数。
 * @param {const char*} type 模型的类型。
 * @param {model_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_factory_register(const char* type, model_create_t create);

/**
 * @method model_factory_unregister
 * 注销模型的创建函数。
 * @param {const char*} type 模型的类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_factory_unregister(const char* type);

/**
 * @method model_factory_create_model
 * 创建模型。
 * @param {const char*} type 模型的类型。
 * @param {void*} args 参数。
 *
 * @return {model_t*} 返回model对象。
 */
model_t* model_factory_create_model(const char* type, void* args);

/**
 * @method model_factory_deinit
 * ~初始化模型工厂。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_factory_deinit(void);

#define MODEL_FACTORY(model_factory) ((model_factory_t*)(model_factory))

END_C_DECLS

#endif /*TK_MODEL_FACTORY_H*/
