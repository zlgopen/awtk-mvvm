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

ret_t model_factory_init(void);

bool_t model_factory_exist(const char* type);
ret_t model_factory_unregister(const char* type);
ret_t model_factory_register(const char* type, model_create_t create);

model_t* model_factory_create(const char* type, void* args);
model_t* model_factory_create_with_filename(const char* filename);

ret_t model_factory_deinit(void);

#define MODEL_FACTORY(model_factory) ((model_factory_t*)(model_factory))

END_C_DECLS

#endif /*TK_MODEL_FACTORY_H*/
