/**
 * File:   binding_rule.h
 * Author: AWTK Develop Team
 * Brief:  binding rule
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
 * 2019-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_RULE_H
#define TK_BINDING_RULE_H

#include "tkc/object.h"
#include "mvvm/base/mvvm_types_def.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class binding_rule_t
 * @parent object_t
 *
 * 绑定规则基类。
 */
typedef struct _binding_rule_t {
  object_t object;
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
   * @property {view_model_t*} view_model
   * @annotation ["readable"]
   * 绑定的ViewModel。
   */
  view_model_t* view_model;
  /**
   * @property {uint32_t} cursor;
   * @annotation ["readable"]
   * 对于数组的ViewModel，保存cursor。
   */
  uint32_t cursor;

  /*private*/
  bool_t inited;
} binding_rule_t;

#define BINDING_RULE(rule) ((binding_rule_t*)(rule))
#define BINDING_RULE_CONTEXT(rule) (((binding_rule_t*)rule)->binding_context)
#define BINDING_RULE_VIEW_MODEL(rule) (((binding_rule_t*)rule)->binding_context->view_model)

#define BINDING_RULE_DATA_PREFIX "v-data"
#define BINDING_RULE_COMMAND_PREFIX "v-on"
#define BINDING_RULE_PROP_INITED "inited"

END_C_DECLS

#endif /*TK_BINDING_RULE_H*/
