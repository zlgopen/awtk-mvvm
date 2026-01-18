/**
 * File:   ui_loader_mvvm.h
 * Author: AWTK Develop Team
 * Brief:  ui loader for mvvm
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-03-03 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#ifndef TK_UI_LOADER_MVVM_H
#define TK_UI_LOADER_MVVM_H

#include "base/ui_loader.h"
#include "base/assets_manager.h"
#include "mvvm/base/binding_rule.h"
#include "mvvm/base/binding_context.h"

BEGIN_C_DECLS

#define EVENT_TAG 0x11223300

/**
 * @class ui_loader_mvvm_t
 * @parent ui_loader_t
 *
 * 支持MVVM绑定的UI加载器。
 *
 */
typedef struct _ui_loader_mvvm_t {
  ui_loader_t base;
  /**
   * @property {asset_info_t*} ui
   * @annotation ["readable"]
   * 界面描述数据。
   */
  const asset_info_t* ui;
  /**
   * @property {navigator_request_t*} navigator_request
   * @annotation ["readable"]
   * 导航请求。
   */
  navigator_request_t* navigator_request;
  /**
   * @property {void*} binding_context
   * @annotation ["readable"]
   * 当前的绑定上下文。
   */
  binding_context_t* binding_context;
  /**
   * @property {binding_rule_t*} rule
   * @annotation ["readable"]
   * 当前的动态规则。
   */
  binding_rule_t* rule;
} ui_loader_mvvm_t;

/**
 * @method ui_loader_mvvm
 * 获取支持MVVM绑定的UI加载器对象。
 * @annotation ["constructor"]
 *
 * @return {ui_loader_t*} 返回UI加载器对象。
 *
 */
ui_loader_t* ui_loader_mvvm(void);

/**
 * @method ui_loader_mvvm_load_widget
 * 加载导航请求指定的控件。

 * @param {navigator_request_t*} req 导航请求。
 *
 * @return {widget_t*} 控件对象。
 */
widget_t* ui_loader_mvvm_load_widget(navigator_request_t* req);

/**
 * @method ui_loader_mvvm_load_widget_with_parent
 * 加载导航请求指定的控件，并指定父控件对象。

 * @param {navigator_request_t*} req 导航请求。
 * @param {widget_t*} parent 父控件对象。
 *
 * @return {widget_t*} 控件对象。
 */
widget_t* ui_loader_mvvm_load_widget_with_parent(navigator_request_t* req, widget_t* parent);

/**
 * @method ui_loader_mvvm_reload_widget
 * 重新加载动态渲染规则指定的控件。

 * @param {binding_rule_t*} rule 动态渲染规则。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ui_loader_mvvm_reload_widget(binding_rule_t* rule);

/**
 * @method ui_loader_mvvm_cast
 * 转换为ui_loader_mvvm对象。
 * @annotation ["cast"]
 * @param {ui_loader_t*} loader ui_loader对象。
 *
 * @return {ui_loader_mvvm_t*} ui_loader_mvvm对象。
 */
ui_loader_mvvm_t* ui_loader_mvvm_cast(ui_loader_t* loader);

#define UI_LOADER_MVVM(l) ui_loader_mvvm_cast(l)

// 动态渲染时内部使用的临时属性
#define WIDGET_PROP_MVVM_ASSETS_INFO " ai "
#define WIDGET_PROP_MVVM_WIDGET_ID " id "
#define WIDGET_PROP_MVVM_DATA_CURSOR " cursor "
#define WIDGET_PROP_MVVM_COUNT_OF_WIDGET_BEFORE_FIRST_DYNAMIC_RULE " cowbfdr "

END_C_DECLS

#endif /*TK_UI_LOADER_MVVM_H*/
