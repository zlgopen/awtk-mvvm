/**
 * File:   items_binding.h
 * Author: AWTK Develop Team
 * Brief:  list binding
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

#ifndef TK_ITEMS_BINDING_H
#define TK_ITEMS_BINDING_H

#include "tkc/str.h"
#include "tkc/object.h"
#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

/**
 * @class items_binding_t
 * @parent binding_rule_t
 * @annotation ["scriptable"]
 * 列表渲染的绑定规则。
 *
 */
typedef struct _items_binding_t {
  binding_rule_t binding_rule;
  tk_object_t* props;

  /**
   * @property {char*} items_name
   * @annotation ["readable"]
   * 源数组的变量名称。
   */
  char* items_name;

  /**
   * @property {char*} item_name
   * @annotation ["readable"]
   * 被迭代的数组元素的别名。
   */
  char* item_name;

  /**
   * @property {char*} index_name
   * @annotation ["readable"]
   * 被迭代的数组元素的索引的别名。
   */
  char* index_name;

  /**
   * @property {char*} id_name
   * @annotation ["readable"]
   * 被迭代的数组元素的键值的别名。
   */
  char* id_name;

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

  /**
   * @property {uint32_t} items_count
   * @annotation ["readable"]
   * 最近一次渲染时源数组的元素数量。
   */
  uint32_t items_count;

  /**
   * @property {uint32_t} cursor;
   * @annotation ["readable"]
   * 当前的数组cursor。
   */
  uint32_t cursor;

  /**
   * @property {uint32_t} rebind_idle_id
   * @annotation ["readable"]
   * Rebind的idle的id
   *
   */
  uint32_t rebind_idle_id;

  /**
   * @property {bool_t} bound;
   * @annotation ["readable"]
   * 规则是否完成绑定。
   */
  bool_t bound;

  /**
   * @property {uint32_t} fixed_widget_count
   * @annotation ["readable"]
   * 渲染时固定创建的控件数量, -1表示不固定
   *
   */
  int32_t fixed_widget_count;

  /**
   * @property {uint32_t} start_index
   * @annotation ["readable"]
   * 渲染的第0个控件对应的item索引
   *
   */
  uint32_t start_item_index;
} items_binding_t;

/**
 * @method items_binding_create
 * 创建列表渲染绑定对象。
 * @annotation ["constructor"]
 *
 * @return {binding_rule_t*} 返回列表渲染绑定对象。
 */
items_binding_t* items_binding_create(void);

/**
 * @method binding_rule_is_items_binding
 * 判断当前规则是否为列表渲染规则。
 *
 * @param {binding_rule_t*} rule 绑定规则对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t binding_rule_is_items_binding(binding_rule_t* rule);

/**
 * @method items_binding_cast
 * 转换为items_binding对象。
 * @annotation ["cast"]
 * @param {void*} rule 绑定规则对象。
 *
 * @return {data_binding_t*} 返回绑定规则对象。
 */
items_binding_t* items_binding_cast(void* rule);

#define ITEMS_BINDING(rule) items_binding_cast((void*)rule)

#define ITEMS_BINDING_ITEMS_NAME "Items"
#define ITEMS_BINDING_ITEM_NAME "Item"
#define ITEMS_BINDING_INDEX_NAME "Index"
#define ITEMS_BINDING_ID_NAME "Id"

END_C_DECLS

#endif /*TK_ITEMS_BINDING_H*/
