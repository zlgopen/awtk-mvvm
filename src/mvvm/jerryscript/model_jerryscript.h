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
 *
 * wrap jerryscript code to a model
 *
 */
struct _model_jerryscript_t {
  model_t model;

  /*private*/
  jerry_value_t jsobj;

  /*避免每次动态分配内存*/
  str_t temp;
};

/**
 * @method model_jerryscript_create
 * 通过一段JS代码创建一个model对象。
 *
 * @param {const char*} name 名称(通常是文件名)。
 * @param {const char*} code 代码字符字符串(UTF8)。
 * @param {uint32_t} code_size 代码的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
model_t* model_jerryscript_create(const char* name, const char* code, uint32_t code_size);

#define MODEL_JERRYSCRIPT(model) ((model_jerryscript_t*)(model))

END_C_DECLS

#endif /*TK_MODEL_JERRYSCRIPT_H*/
