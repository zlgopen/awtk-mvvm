/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/slist.h"
#include "mvvm/base/mvvm_types_def.h"
#include "mvvm/base/view_model.h"
#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

typedef ret_t (*binding_context_destroy_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_update_to_view_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_update_to_model_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_exec_t)(binding_context_t* ctx, const char* cmd, const char* args);
typedef bool_t (*binding_context_can_exec_t)(binding_context_t* ctx, const char* cmd,
                                             const char* args);
typedef ret_t (*binding_context_bind_rule_t)(binding_context_t* ctx, binding_rule_t* rule);
typedef ret_t (*binding_context_notify_items_changed_t)(binding_context_t* ctx, tk_object_t* items,
                                                        bool_t sync);
typedef uint32_t (*binding_context_get_items_cursor_of_rule_t)(binding_context_t* ctx,
                                                               binding_rule_t* rule);
typedef uint32_t (*binding_context_calc_widget_index_of_rule_t)(binding_context_t* ctx,
                                                                binding_rule_t* rule);
typedef const char* (*binding_context_resolve_path_by_rule_t)(binding_context_t* ctx,
                                                              binding_rule_t* rule,
                                                              const char* path, bool_t* is_cursor);

typedef struct _binding_context_vtable_t {
  binding_context_destroy_t destroy;
  binding_context_update_to_view_t update_to_view;
  binding_context_update_to_model_t update_to_model;
  binding_context_exec_t exec;
  binding_context_can_exec_t can_exec;
  binding_context_bind_rule_t bind_data;
  binding_context_bind_rule_t bind_command;
  binding_context_bind_rule_t bind_conditon;
  binding_context_bind_rule_t bind_items;
  binding_context_notify_items_changed_t notify_items_changed;
  binding_context_get_items_cursor_of_rule_t get_items_cursor_of_rule;
  binding_context_calc_widget_index_of_rule_t calc_widget_index_of_rule;
  binding_context_resolve_path_by_rule_t resolve_path_by_rule;
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
  slist_t command_bindings;
  /**
   * @property {darray_t*} data_bindings
   * @annotation ["readable"]
   * 数据绑定规则集合。
   */
  slist_t data_bindings;
  /**
   * @property {darray_t*} dynamic_bindings
   * @annotation ["readable"]
   * 动态绑定规则集合（包括条件渲染和列表渲染）。
   */
  slist_t dynamic_bindings;
  /**
   * @property {bool_t} bound
   * @annotation ["readable"]
   * 已经完成绑定。
   */
  bool_t bound;
  /**
   * @property {bool_t} updating_model
   * @annotation ["readable"]
   * 正在更新模型。
   */
  bool_t updating_model;
  /**
   * @property {bool_t} updating_view
   * @annotation ["readable"]
   * 正在更新视图。
   */
  bool_t updating_view;
  /**
   * @property {bool_t} updating_view_by_ui
   * @annotation ["readable"]
   * 更新视图的事件由ui触发。
   */
  bool_t updating_view_by_ui;
  /**
   * @property {uint32_t} update_view_idle_id
   * @annotation ["readable"]
   * 更新view的idle的id。
   */
  uint32_t update_view_idle_id;
  /**
   * @property {view_model_t*} view_model
   * @annotation ["readable"]
   * 绑定的ViewModel。
   */
  view_model_t* view_model;
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
  /**
   * @property {binding_context_t*} parent
   * @annotation ["readable"]
   * 父的binding_context对象。
   */
  binding_context_t* parent;

  /*private*/
  void* ui_data;
  str_t temp;
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
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_init(binding_context_t* ctx, navigator_request_t* req);

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

/**
 * @method binding_context_get_root
 * 获取根context。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 *
 * @return {ret_t} 返回binding_context对象。
 */
binding_context_t* binding_context_get_root(binding_context_t* ctx);

/**
 * @method binding_context_set_view_model
 * 设置绑定的ViewModel。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {view_model_t*} view_model 绑定的ViewModel对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_set_view_model(binding_context_t* ctx, view_model_t* view_model);

/**
 * @method binding_context_set_bound
 * 设置是否完成绑定。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_context_t*} parent 父binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_set_bound(binding_context_t* ctx, bool_t bound);

/**
 * @method binding_context_set_parent
 * 设置父context。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_context_t*} parent 父binding_context对象。
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
 * @method binding_context_clear_bindings
 * 清除绑定规则。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_clear_bindings(binding_context_t* ctx);

/**
 * @method binding_context_clear_bindings_of_widget
 * 清除指定控件的绑定规则。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {void*} widget 控件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_clear_bindings_of_widget(binding_context_t* ctx, void* widget);

/**
 * @method binding_context_bind_data
 * 绑定数据到上下文。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 数据绑定规则。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_bind_data(binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method binding_context_bind_command
 * 绑定命令到上下文。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 命令绑定规则。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_bind_command(binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method binding_context_bind_condition
 * 绑定条件渲染到上下文。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 条件渲染规则。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_bind_condition(binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method binding_context_bind_items
 * 绑定列表渲染到上下文。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 列表渲染规则。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_bind_items(binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method binding_context_notify_items_changed
 * 触发items改变事件。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {tk_object_t*} items items对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_notify_items_changed(binding_context_t* ctx, tk_object_t* items);

/**
 * @method binding_context_notify_items_changed_sync
 * 触发items改变事件，并同步更新数据到视图。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {tk_object_t*} items items对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_notify_items_changed_sync(binding_context_t* ctx, tk_object_t* items);

/**
 * @method binding_context_get_items_cursor_of_rule
 * 获取与指定绑定规则最近的一个列表渲染规则的当前数组cursor。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 绑定规则。
 *
 * @return {uint32_t} 返回cursor。
 */
uint32_t binding_context_get_items_cursor_of_rule(binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method binding_context_calc_widget_index_of_rule
 * 获取指定所绑定的控件在控件树上的索引（列表渲染规则返回所渲染的第0个元素的索引）。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 绑定规则。
 *
 * @return {uint32_t} 返回索引。
 */
uint32_t binding_context_calc_widget_index_of_rule(binding_context_t* ctx, binding_rule_t* rule);

/**
 * @method binding_context_resolve_path_by_rule
 * 解析相对于指定绑定规则的数据路径为ViewModel的绝对路径。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 绑定规则。
 * @param {const char*} path 数据路径。
 * @param {bool_t*} is_cursor 是否为cursor。
 *
 * @return {const char*} 为cursor时返回列表渲染规则的当前数组cursor，否则返回路径。
 */
const char* binding_context_resolve_path_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                                 const char* path, bool_t* is_cursor);

/**
 * @method binding_context_get_prop_by_rule
 * 获取相对于指定绑定规则的数据的值。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 绑定规则。
 * @param {const char*} name 数据名称。
 * @param {value_t*} v 返回的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_get_prop_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                       const char* name, value_t* v);

/**
 * @method binding_context_set_prop_by_rule
 * 设置相对于指定绑定规则的数据的值。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {binding_rule_t*} rule 绑定规则。
 * @param {const char*} name 数据名称。
 * @param {value_t*} v 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_set_prop_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                       const char* name, const value_t* v);

#define BINDING_CONTEXT(ctx) ((binding_context_t*)(ctx))

END_C_DECLS

#endif /*TK_BINDING_CONTEXT_H*/
