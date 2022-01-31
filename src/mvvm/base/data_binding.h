/**
 * File:   data_binding.c
 * Author: AWTK Develop Team
 * Brief:  data binding
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_DATA_BINDING_H
#define TK_DATA_BINDING_H

#include "tkc/str.h"
#include "tkc/object.h"
#include "tkc/fscript.h"
#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

/**
 * @enum update_model_trigger_t
 * @prefix UPDATE_WHEN_
 * 更新模型的时机。
 */
typedef enum _update_model_trigger_t {
  /**
   * @const UPDATE_WHEN_CHANGED
   * 编辑完成时才触发，即回车会或失去焦点时更新数据到模型。
   */
  UPDATE_WHEN_CHANGED = 0,

  /**
   * @const UPDATE_WHEN_CHANGING
   * 编辑时实时触发，在编辑时实时更新数据到模型。
   */
  UPDATE_WHEN_CHANGING,

  /**
   * @const EUPDATE_WHEN_EXPLICIT
   * 需要显式触发。
   * 一般在命令绑定规则中指定的UpdateModel=True，执行命令前自动调用UpdateModel。
   */
  UPDATE_WHEN_EXPLICIT
} update_model_trigger_t;

/**
 * @enum binding_mode_t
 * @annotation ["scriptable"]
 * @prefix BINDING_
 * 绑定模式。
 */
typedef enum _binding_mode_t {
  /**
   * @const BINDING_TWO_WAY
   * 双向绑定，模型变化时自动更新视图，视图上数据变化时自动更新模型。
   */
  BINDING_TWO_WAY,

  /**
   * @const BINDING_ONE_WAY
   * 单向绑定，模型变化时自动更新视图。
   */
  BINDING_ONE_WAY,

  /**
   * @const BINDING_ONE_WAY_TO_VIEW_MODEL
   * 单向绑定，视图上数据变化时自动更新模型。
   */
  BINDING_ONE_WAY_TO_VIEW_MODEL,

  /**
   * @const BINDING_ONCE
   * 单向绑定，仅仅绑定一次。
   */
  BINDING_ONCE
} binding_mode_t;

/**
 * @class data_binding_t
 * @parent binding_rule_t
 * @annotation ["scriptable"]
 * 数据绑定规则。
 *
 */
typedef struct _data_binding_t {
  binding_rule_t binding_rule;
  tk_object_t* props;

  /**
   * @property {char*} path
   * @annotation ["readable"]
   * 模型中的数据名称。
   */
  char* path;

  /**
   * @property {char*} prop
   * @annotation ["readable"]
   * 控件的属性。
   */
  char* prop;

  /**
   * @property {char*} converter
   * @annotation ["readable"]
   * 格式转换器的名称。
   */
  char* converter;

  /**
   * @property {char*} to_view
   * @annotation ["readable"]
   * 转换成视图需要的格式。
   */
  char* to_view;

  /**
   * @property {char*} to_model
   * @annotation ["readable"]
   * 转换成模型需要的格式。
   */
  char* to_model;

  /**
   * @property {char*} validator
   * @annotation ["readable"]
   * 数据校验器的名称。
   */
  char* validator;

  /**
   * @property {binding_mode_t} mode
   * @annotation ["readable"]
   * 绑定模式。
   */
  binding_mode_t mode;

  /**
   * @property {update_model_trigger_t} trigger;
   * @annotation ["readable"]
   * 触发更新模型的时机。
   */
  update_model_trigger_t trigger;
  /*private*/
  fscript_t* expr;
  fscript_t* to_model_expr;
  fscript_t* to_view_expr;
  const value_t* value;
} data_binding_t;

/**
 * @method data_binding_create
 * 创建数据绑定对象。
 * @annotation ["constructor"]
 *
 * @return {binding_rule_t*} 返回数据绑定对象。
 */
data_binding_t* data_binding_create(void);

/**
 * @method data_binding_get_prop
 * 从模型中获取属性值。
 *
 * @param {data_binding_t*} rule 绑定规则对象。
 * @param {value_t*} v 值对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_binding_get_prop(data_binding_t* rule, value_t* v);

/**
 * @method data_binding_set_prop
 * 设置属性值到模型。
 *
 * @param {data_binding_t*} rule 绑定规则对象。
 * @param {const value_t*} v 值对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t data_binding_set_prop(data_binding_t* rule, const value_t* v);

/**
 * @method binding_rule_is_data_binding
 * 判断当前规则是否为数据绑定规则。
 *
 * @param {binding_rule_t*} rule 绑定规则对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t binding_rule_is_data_binding(binding_rule_t* rule);

/**
 * @method data_binding_cast
 * 转换为data_binding对象。
 * @annotation ["cast"]
 * @param {void*} rule 绑定规则对象。
 *
 * @return {data_binding_t*} 返回绑定规则对象。
 */
data_binding_t* data_binding_cast(void* rule);

#define DATA_BINDING(rule) data_binding_cast((void*)rule)

#define DATA_BINDING_PATH "Path"
#define DATA_BINDING_MODE "Mode"
#define DATA_BINDING_PROP "Prop"
#define DATA_BINDING_TRIGGER "Trigger"
#define DATA_BINDING_CONVERTER "Converter"
#define DATA_BINDING_VALIDATOR "Validator"
#define DATA_BINDING_ERROR_OF "error.of."
#define DATA_BINDING_TO_VIEW "ToView"
#define DATA_BINDING_TO_MODEL "ToModel"

#define BINDING_STR_ONCE "Once"
#define BINDING_STR_TWO_WAY "TwoWay"
#define BINDING_STR_ONE_WAY "OneWay"
#define BINDING_STR_ONE_WAY_TO_VIEW_MODEL "OneWayToModel"

END_C_DECLS

#endif /*TK_DATA_BINDING_H*/
