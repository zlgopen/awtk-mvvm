/**
 * File:  object.hpp
 * Author: AWTK Develop Team
 * Brief:  object interface for cpp
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-05-28 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_HPP
#define TK_OBJECT_HPP

#include "tkc/emitter.h"
#include "tkc/object.h"

namespace vm {

/**
 * @class Object
 *
 * C++ Object的基类。
 *
 */
class Object {
 public:
  Object();
  virtual ~Object();

 public:
  /**
   * @method Exec
   * 执行指定的命令。
   *
   * @param {const char*} name 命令名。
   * @param {const char*} args 命令的参数。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t Exec(const char* name, const char* args);

  /**
   * @method CanExec
   * 检查指定的命令是否可以执行。
   *
   * @param {const char*} name 命令名。
   * @param {const char*} args 命令的参数。
   *
   * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
   */
  virtual bool_t CanExec(const char* name, const char* args) const;

  /**
   * @method GetProp
   * 获取指定属性的值。
   *
   * @param {const char*} name 属性名。
   * @param {value_t*} value 属性值。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t GetProp(const char* name, value_t* v) const;

  /**
   * @method SetProp
   * 设置指定属性的值。
   *
   * @param {const char*} name 属性名。
   * @param {value_t*} value 属性值。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t SetProp(const char* name, const value_t* v);

 public:
  //基类提供的公共函数。
  /**
   * @method Off
   * 注销指定事件的处理函数。
   * @param {uint32_t} id emitter_on返回的ID。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t Off(uint32_t id);

  /**
   * @method OffByCtx
   * 注销指定事件的处理函数。
   * @param {void*} ctx 事件处理函数上下文。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t OffByCtx(void* ctx);

  /**
   * @method DispatchEvent
   * 分发事件。如果当前分发的回调函数返回RET_REMOVE，该回调函数将被移出。
   *
   * @param {event_t*} event 事件对象。
   *
   * @return {ret_t}
   * 如果当前分发的回调函数返回RET_STOP，dispatch中断分发，并返回RET_STOP，否则返回RET_OK。
   */
  ret_t DispatchEvent(event_t* event);

  /**
   * @method On
   * 注册指定事件的处理函数。
   * @param {uint32_t} type 事件类型。
   * @param {event_func_t} on_event 事件处理函数。
   * @param {void*} ctx 事件处理函数上下文。
   *
   * @return {uint32_t} 返回id，用于emitter_off。
   */
  uint32_t On(uint32_t event, event_func_t on_event, void* ctx);

  /**
   * @method NotifyPropsChanged
   * 触发props改变事件。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t NotifyPropsChanged();

  /**
   * @method NotifyPropsChanged
   * 触发items改变事件。
   * @param {Object*} target 发生变化的items对象。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  ret_t NotifyItemsChanged(Object* target = NULL);

 private:
  emitter_t emitter;

 public:
  /**
   * @property {object_t*} adapter
   * @annotation ["readable", "scriptable"]
   * Object的适配器对象。
   */
  object_t* adapter;
};

}  // namespace vm

#endif /*TK_OBJECT_HPP*/
