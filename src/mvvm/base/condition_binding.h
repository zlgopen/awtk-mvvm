/**
 * File:   condition_binding.h
 * Author: AWTK Develop Team
 * Brief:  condition binding
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_CONDITION_BINDING_H
#define TK_CONDITION_BINDING_H

#include "tkc/object.h"
#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

/**
 * @class condition_binding_t
 * @parent binding_rule_t
 * @annotation ["scriptable"]
 * 条件渲染的绑定规则。
 *
 */
typedef struct _condition_binding_t {
  binding_rule_t binding_rule;
  tk_object_t* props;

  /**
   * @property {char*} current_expr
   * @annotation ["readable"]
   * 当前为true的条件表达式。
   */
  const char* current_expr;

  /**
   * @property {uint32_t} widget_data_pos
   * @annotation ["readable"]
   * 动态渲染的界面描述数据的位置。
   */
  uint32_t widget_data_pos;

  /**
   * @property {uint32_t} widget_data_size
   * @annotation ["readable"]
   * 动态渲染的界面描述数据的长度。
   */
  uint32_t widget_data_size;

  /**
   * @property {uint32_t} static_widget_before_next_dynamic_binding
   * @annotation ["readable"]
   * 到下一条动态渲染规则之间非动态渲染的控件的数量。
   */
  uint32_t static_widget_before_next_dynamic_binding;
} condition_binding_t;

/**
 * @method condition_binding_create
 * 创建列表渲染绑定对象。
 * @annotation ["constructor"]
 *
 * @return {binding_rule_t*} 返回列表渲染绑定对象。
 */
condition_binding_t* condition_binding_create(void);

/**
 * @method binding_rule_is_condition_binding
 * 判断当前规则是否为条件渲染规则。
 *
 * @param {binding_rule_t*} rule 绑定规则对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t binding_rule_is_condition_binding(binding_rule_t* rule);

/**
 * @method condition_binding_cast
 * 转换为condition_binding对象。
 * @annotation ["cast"]
 * @param {void*} rule 绑定规则对象。
 *
 * @return {data_binding_t*} 返回绑定规则对象。
 */
condition_binding_t* condition_binding_cast(void* rule);

#define CONDITION_BINDING(rule) condition_binding_cast((void*)rule)

END_C_DECLS

#endif /*TK_CONDITION_BINDING_H*/
