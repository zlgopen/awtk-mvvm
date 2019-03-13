/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
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
typedef ret_t (*binding_context_destroy_t)(binding_context_t* ctx);

typedef struct _binding_context_vtable_t {
  binding_context_update_to_view_t update_to_view;
  binding_context_update_to_model_t update_to_model;
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
   * @property {int32_t} request_update_view
   * @annotation ["readable"]
   * 请求更新视图的次数（真正更新在idle中完成）。
   *
   */
  int32_t request_update_view;
  /**
   * @property {void*} current_widget
   * @annotation ["readable"]
   * 当前真正绑定的控件。
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
   * 绑定的根控件(通常是窗口)
   */
  void* widget;
  /**
   * @property {navigator_request_t*} navigator_request
   * @annotation ["readable"]
   * 请求参数。
   */
  navigator_request_t* navigator_request;

  /*private*/
  const binding_context_vtable_t* vt;
};

/**
 * @method binding_context_init
 * 初始化。
 *
 * @param {binding_context_t*} ctx binding_context对象。
 * @param {navigator_request_t*} navigator_request 请求参数对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_init(binding_context_t* ctx, navigator_request_t* navigator_request);

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
