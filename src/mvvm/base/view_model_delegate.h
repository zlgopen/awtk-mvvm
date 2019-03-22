/**
 * File:   view_model_delegate.h
 * Author: AWTK Develop Team
 * Brief:  view_model_delegate
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
 * 2019-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_DELEGATE_H
#define TK_VIEW_MODEL_DELEGATE_H

#include "tkc/str.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_delegate_t;
typedef struct _model_delegate_t view_model_delegate_t;

typedef ret_t (*view_model_get_prop_t)(void* obj, value_t* value);
typedef ret_t (*view_model_set_prop_t)(void* obj, const value_t* value);
typedef ret_t (*view_model_exec_t)(void* obj, const char* args);
typedef bool_t (*view_model_can_exec_t)(void* obj, const char* args);

/**
 * @class view_model_delegate_t
 * @parent view_model_t
 *
 * 将简单对象包装成view_model对象。
 *
 */
struct _model_delegate_t {
  view_model_t view_model;

  /*private*/
  void* obj;
  tk_destroy_t obj_destroy;

  object_t* props;
  object_t* commands;

  str_t temp;
};

/**
 * @method view_model_delegate_create
 * 创建view_model_delegate对象。
 *
 * @param {void*} obj 被包装的对象。
 * @param {tk_destroy_t} obj_destroy 被包装的对象的析构函数。
 *
 * @return {view_model_t} 返回view_model对象。
 */
view_model_t* view_model_delegate_create(void* obj, tk_destroy_t obj_destroy);

/**
 * @method view_model_delegate_install_command
 * 安装一个命令。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 命令名称。
 * @param {view_model_exec_t} exec 执行函数。
 * @param {view_model_can_exec_t} can_exec 检查是否可以执行的函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_delegate_install_command(view_model_t* view_model, const char* name,
                                          view_model_exec_t exec, view_model_can_exec_t can_exec);

/**
 * @method view_model_delegate_install_prop
 * 安装一个属性。在访问一个属性时，优先使用get/set函数，其次通过地址访问。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 属性名称。
 * @param {value_type_t} type 属性的类型(可选)。
 * @param {void*} addr 属性的地址(可选)。
 * @param {view_model_get_prop_t} get get函数(可选)。
 * @param {view_model_set_prop_t} set set函数(可选)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_delegate_install_prop(view_model_t* view_model, const char* name,
                                       value_type_t type, void* addr, view_model_get_prop_t get,
                                       view_model_set_prop_t set);

#define VIEW_MODEL_DELEGATE(view_model) ((view_model_delegate_t*)(view_model))

#define VIEW_MODEL_COMMAND(view_model, name, exec, can_exec)                     \
  view_model_delegate_install_command(view_model, name, (view_model_exec_t)exec, \
                                      (view_model_can_exec_t)can_exec)

#define VIEW_MODEL_PROP(view_model, name, get, set)                            \
  view_model_delegate_install_prop(view_model, name, VALUE_TYPE_INVALID, NULL, \
                                   (view_model_get_prop_t)get, (view_model_set_prop_t)set)

#define VIEW_MODEL_SIMPLE_PROP(view_model, name, type, addr) \
  view_model_delegate_install_prop(view_model, name, type, addr, NULL, NULL)

END_C_DECLS

#endif /*TK_VIEW_MODEL_DELEGATE_H*/
