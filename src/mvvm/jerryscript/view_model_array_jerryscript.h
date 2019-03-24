/**
 * File:   view_model_array_jerryscript.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented view_model
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
 * 2019-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ARRAY_JERRYSCRIPT_H
#define TK_VIEW_MODEL_ARRAY_JERRYSCRIPT_H

#include "tkc/str.h"
#include "jerryscript.h"
#include "mvvm/base/view_model_array.h"

BEGIN_C_DECLS

struct _model_jerryscript_array_t;
typedef struct _model_jerryscript_array_t view_model_array_jerryscript_t;

/**
 * @class view_model_array_jerryscript_t
 * @parent view_model_t
 *
 * wrap jerryscript code to a view_model
 *
 */
struct _model_jerryscript_array_t {
  view_model_array_t view_model_array;

  /*private*/
  jerry_value_t jsobj;

  /*避免每次动态分配内存*/
  str_t temp;
};

/**
 * @method view_model_array_jerryscript_create
 * 通过一段JS代码创建一个view_model对象。
 *
 * @param {const char*} name 名称(通常是文件名)。
 * @param {jerry_value_t*} jsobj js对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
view_model_t* view_model_array_jerryscript_create(jerry_value_t jsobj);

#define VIEW_MODEL_ARRAY_JERRYSCRIPT(view_model) ((view_model_array_jerryscript_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_ARRAY_JERRYSCRIPT_H*/
