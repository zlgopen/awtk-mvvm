/**

 * File:   view_model.h
 * Author: AWTK Develop Team
 * Brief:  view_model
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
 * 2019-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_H
#define TK_VIEW_MODEL_H

#include "tkc/str.h"
#include "mvvm/base/model.h"

BEGIN_C_DECLS

struct _view_model_t;
typedef struct _view_model_t view_model_t;

typedef enum _view_model_type_t {
  VIEW_MODEL_NORMAL = 0,
  VIEW_MODEL_ARRAY,
  VIEW_MODEL_ITEM,
  VIEW_MODEL_SCRIPT
} view_model_type_t;

/**
 * @class view_model_t
 * @parent object_t
 *
 * ViewModel。
 *
 */
struct _view_model_t {
  object_t object;

  /*private*/
  model_t* model;
  str_t last_error;
  view_model_type_t type;
};

/**
 * @method view_model_init
 * 初始化模型(供子类使用)。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {view_model_type_t} type 类型。
 * @param {model_t*} model model对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_init(view_model_t* vm, view_model_type_t type, model_t* model);

/**
 * @method view_model_has_prop
 * 检查指定的属性是否存在。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {const char*} name 属性名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
bool_t view_model_has_prop(view_model_t* vm, const char* name);

/**
 * @method view_model_eval
 * 计算表达式的值。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {const char*} expr 表达式。
 * @param {value_t*} value 计算结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_eval(view_model_t* vm, const char* expr, value_t* value);

/**
 * @method view_model_get_prop
 * 获取指定属性的值。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {const char*} name 属性名。
 * @param {value_t*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_get_prop(view_model_t* vm, const char* name, value_t* value);

/**
 * @method view_model_set_prop
 * 设置指定属性的值。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {const char*} name 属性名。
 * @param {value_t*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_set_prop(view_model_t* vm, const char* name, const value_t* value);

/**
 * @method view_model_can_exec
 * 检查指定的命令是否可以执行。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令的参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t view_model_can_exec(view_model_t* vm, const char* name, const char* args);

/**
 * @method view_model_exec
 * 执行指定的命令。
 *
 * @param {view_model_t*} vm view_model对象。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令的参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_exec(view_model_t* vm, const char* name, const char* args);

/**
 * @method view_model_deinit
 * 初始化模型(供子类使用)。
 *
 * @param {view_model_t*} vm view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_deinit(view_model_t* vm);

#define VIEW_MODEL(view_model) ((view_model_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_H*/
