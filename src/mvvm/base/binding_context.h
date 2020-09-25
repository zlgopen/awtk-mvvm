/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
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
 * 2019-01-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_CONTEXT_H
#define TK_BINDING_CONTEXT_H

#include "tkc/darray.h"
#include "mvvm/base/types_def.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

typedef ret_t (*binding_context_update_to_view_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_update_to_model_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_exec_t)(binding_context_t* ctx, const char* cmd, const char* args);
typedef bool_t (*binding_context_can_exec_t)(binding_context_t* ctx, const char* cmd,
                                             const char* args);

typedef ret_t (*binding_context_bind_t)(binding_context_t* ctx, void* widget);
typedef ret_t (*binding_context_update_widget_t)(binding_context_t* ctx, void* widget);

typedef ret_t (*binding_context_destroy_t)(binding_context_t* ctx);

typedef struct _binding_context_vtable_t {
  binding_context_update_to_view_t update_to_view;
  binding_context_update_to_model_t update_to_model;
  binding_context_exec_t exec;
  binding_context_bind_t bind;
  binding_context_update_widget_t update_widget;
  binding_context_can_exec_t can_exec;
  binding_context_destroy_t destroy;
} binding_context_vtable_t;

/**
 * @class binding_context_t
 * 主要负责绑定规则的管理。
 *
 */
struct _binding_context_t {
  /**
   * @property {darray_t*} command_bindings
   * @annotation ["readable"]
   * 命令绑定规则集合。
   */
  darray_t command_bindings;
  /**
   * @property {darray_t*} data_bindings
   * @annotation ["readable"]
   * 数据绑定规则集合。
   */
  darray_t data_bindings;
  /**
   * @property {bool_t} bound
   * @annotation ["readable"]
   * 已经完成绑定。
   */
  bool_t bound;
  /**
   * @property {bool_t} updating_view
   * @annotation ["readable"]
   * 正在更新视图。
   */
  bool_t updating_view;
  /**
   * @property {bool_t} updating_model
   * @annotation ["readable"]
   * 正在更新模型。
   */
  bool_t updating_model;
  /**
   * @property {uint32_t} update_view_idle_id
   * @annotation ["readable"]
   * 更新view的idle的id
   *
   */
  uint32_t update_view_idle_id;
  /**
   * @property {int32_t} request_rebind
   * @annotation ["readable"]
   * 请求Rebind的次数（真正Rebind在idle中完成）。
   *
   */
  int32_t request_rebind;
  /**
   * @property {void*} current_widget
   * @annotation ["readable"]
   * 当前正在绑定的控件。
   */
  void* current_widget;
  /**
   * @property {view_model_t*} view_model
   * @annotation ["readable"]
   * ViewModel
   */
  view_model_t* view_model;
  /**
   * @property {uint32_t} cursor;
   * @annotation ["readable"]
   * 用于数组ViewModel。
   */
  uint32_t cursor;
  /**
   * @property {void*} widget
   * @annotation ["readable"]
   * 绑定的根控件。
   */
  void* widget;
  /**
   * @property {navigator_request_t*} navigator_request
   * @annotation ["readable"]
   * 请求参数。
   */
  navigator_request_t* navigator_request;

  /*private*/
  /*列表绑定的模板*/
  darray_t cache_widgets;
  binding_context_t* parent; 
  const binding_context_vtable_t* vt;
};

/**
 * @method binding_context_init
 * 初始化。
 *
 *> 内部自动增加req和vm的引用计数。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {navigator_request_t*} req 请求参数对象。
 * @param {view_model_t**} vm view view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_init(binding_context_t* ctx, navigator_request_t* req, view_model_t* vm);

/**
 * @method binding_context_set_parent
 * 设置父context。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_context_t*} parent parent binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_set_parent(binding_context_t* ctx, binding_context_t* parent);

/**
 * @method binding_context_update_to_view
 * 更新数据到视图。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_update_to_view(binding_context_t* ctx);

/**
 * @method binding_context_exec
 * 执行内置命令。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {const char*} cmd 命令名。
 * @param {const char*} args 命令参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_exec(binding_context_t* ctx, const char* cmd, const char* args);

/**
 * @method binding_context_can_exec
 * 检查内置命令是否可以执行。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {const char*} cmd 命令名。
 * @param {const char*} args 命令参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t binding_context_can_exec(binding_context_t* ctx, const char* cmd, const char* args);

/**
 * @method binding_context_update_to_model
 * 更新数据到模型。
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_update_to_model(binding_context_t* ctx);

/**
 * @method binding_context_clear_bindings
 * 清除绑定规则(用于重新绑定)。
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_clear_bindings(binding_context_t* ctx);

/**
 * @method binding_context_bind
 * 绑定指定控件。
 * > 目前主要用于特殊控件(如table view)实现自定义绑定。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {void*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_bind(binding_context_t* ctx, void* widget);

/**
 * @method binding_context_update_widget
 *
 *  针对指定控件的update to view。
 *
 * > 目前主要用于特殊控件(如table view)实现自定义绑定。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {void*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_update_widget(binding_context_t* ctx, void* widget);

/**
 * @method binding_context_destroy
 * 销毁binding context对象。
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_destroy(binding_context_t* ctx);

#define BINDING_CONTEXT(ctx) ((binding_context_t*)(ctx))

END_C_DECLS

#endif /*TK_BINDING_CONTEXT_H*/
