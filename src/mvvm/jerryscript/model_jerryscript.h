/**
 * File:   model_jerryscript.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented model
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MODEL_JERRYSCRIPT_H
#define TK_MODEL_JERRYSCRIPT_H

#include "tkc/str.h"
#include "jerryscript.h"
#include "mvvm/base/model.h"

BEGIN_C_DECLS

struct _model_jerryscript_t;
typedef struct _model_jerryscript_t model_jerryscript_t;

/**
 * @class model_jerryscript_t
 * @parent model_t
 * @annotation ["scriptable"]
 *
 * wrap jerryscript code to a model
 *
 */
struct _model_jerryscript_t {
  model_t model;

  jerry_value_t jscode;
  jerry_value_t jsobj;

  str_t temp;
};

ret_t model_jerryscript_init(void);
ret_t model_jerryscript_deinit(void);

model_t* model_jerryscript_create(const char* name, const char* code, uint32_t code_size);

#define MODEL_JERRYSCRIPT(model) ((model_jerryscript_t*)(model))

END_C_DECLS

#endif /*TK_MODEL_JERRYSCRIPT_H*/
