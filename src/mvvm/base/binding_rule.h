/**
 * File:   binding_rule.h
 * Author: AWTK Develop Team
 * Brief:  binding rule
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
 * 2019-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_RULE_H
#define TK_BINDING_RULE_H

#include "tkc/object.h"
#include "base/widget_consts.h"
#include "mvvm/base/mvvm_types_def.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _binding_rule_t;
typedef struct _binding_rule_t binding_rule_t;

/**
 * @class binding_rule_t
 * @parent tk_object_t
 *
 * 绑定规则基类。
 */
struct _binding_rule_t {
  tk_object_t object;
  /**
   * @property {void*} widget
   * @annotation ["readable"]
   * 绑定的控件。
   */
  void* widget;
  /**
   * @property {binding_context_t*} binding_context
   * @annotation ["readable"]
   * 绑定的上下文。
   */
  binding_context_t* binding_context;
  /**
   * @property {binding_rule_t*} parent
   * @annotation ["readable"]
   * 上一级的绑定规则。
   */
  binding_rule_t* parent;

  /*private*/
  bool_t inited;
};

#define BINDING_RULE(rule) ((binding_rule_t*)(rule))
#define BINDING_RULE_CONTEXT(rule) (BINDING_RULE(rule)->binding_context)
#define BINDING_RULE_VIEW_MODEL(rule) (BINDING_RULE_CONTEXT(rule)->view_model)
#define BINDING_RULE_WIDGET(rule) (BINDING_RULE(rule)->widget)
#define BINDING_RULE_PARENT(rule) (BINDING_RULE(rule)->parent)

#define BINDING_RULE_DATA_PREFIX "v-data"
#define BINDING_RULE_COMMAND_PREFIX "v-on"
#define BINDING_RULE_ITEMS "v-for"
#define BINDING_RULE_CONDITION_IF "v-if"
#define BINDING_RULE_CONDITION_ELIF "v-elif"
#define BINDING_RULE_CONDITION_ELSE "v-else"
#define BINDING_RULE_PROP_INITED "inited"

END_C_DECLS

#endif /*TK_BINDING_RULE_H*/
