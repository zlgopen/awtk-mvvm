/**
 * File:   view_model_factory.h
 * Author: AWTK Develop Team
 * Brief:  view_model factory
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

#ifndef TK_VIEW_MODEL_FACTORY_H
#define TK_VIEW_MODEL_FACTORY_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class view_model_factory_t
 *
 * 模型工厂。
 *
 */
typedef struct _model_factory_t {
  object_t* creators;
} view_model_factory_t;

/**
 * @method view_model_factory_init
 * 初始化模型工厂。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_factory_init(void);

/**
 * @method view_model_factory_exist
 * 检查指定的模型是否存在。
 * @param {const char*} type 模型的类型。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t view_model_factory_exist(const char* type);

/**
 * @method view_model_factory_register
 * 注册模型的创建函数。
 * @param {const char*} type 模型的类型。
 * @param {view_model_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_factory_register(const char* type, view_model_create_t create);

/**
 * @method view_model_factory_unregister
 * 注销模型的创建函数。
 * @param {const char*} type 模型的类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_factory_unregister(const char* type);

/**
 * @method view_model_factory_create_model
 * 创建模型。
 * @param {const char*} type 模型的类型。
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_factory_create_model(const char* type, navigator_request_t* req);

/**
 * @method view_model_factory_deinit
 * ~初始化模型工厂。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_factory_deinit(void);

#define VIEW_MODEL_FACTORY(view_model_factory) ((view_model_factory_t*)(view_model_factory))

END_C_DECLS

#endif /*TK_VIEW_MODEL_FACTORY_H*/
