/**
 * File:  view_model.hpp
 * Author: AWTK Develop Team
 * Brief:  view_model array interface for cpp
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
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ARRAY_HPP
#define TK_VIEW_MODEL_ARRAY_HPP

#include "tkc/emitter.h"
#include "mvvm/base/navigator_request.h"

namespace vm {

class ViewModelArray {
  public:
    ViewModelArray(navigator_request_t* request);
    virtual ~ViewModelArray();
  
  public:
    /**
     * @method GetSize
     * 获取数据的条数。
     *
     * @return {uint32_t} 返回数据的条数。
     */
    virtual uint32_t GetSize(void) const;

    /**
     * @method Exec
     * 执行指定的命令。
     *
     * @param {int32_t} index 索引。
     * @param {const char*} name 命令名。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t Exec(int32_t index, const char* name);

    /**
     * @method CanExec
     * 检查指定的命令是否可以执行。
     *
     * @param {int32_t} index 索引。
     * @param {const char*} name 命令名。
     *
     * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
     */
    virtual bool_t CanExec(int32_t index, const char* name) const;

    /**
     * @method GetProp
     * 获取指定属性的值。
     *
     * @param {int32_t} index 索引。
     * @param {const char*} name 属性名。
     * @param {value_t*} value 属性值。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t GetProp(int32_t index, const char* name, value_t* v) const;

    /**
     * @method SetProp
     * 设置指定属性的值。
     *
     * @param {int32_t} index 索引。
     * @param {const char*} name 属性名。
     * @param {value_t*} value 属性值。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t SetProp(int32_t index, const char* name, const value_t* v);
    
    /**
     * @method Remove
     * 删除指定的条目。
     *
     * @param {int32_t} index 索引。
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t Remove(int32_t index);

    /**
     * @method Clear
     * 清除全部条目。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t Clear();

  public:
    //事件
    /**
     * @method OnWillMount
     * 打开视图即将加载模型时通知view_model。
     *
     * @param {navigator_request_t*} req request对象。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t OnWillMount(navigator_request_t* request);
    /**
     * @method OnMount
     * 视图与模型绑定完成后通知模型。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t OnMount();
    /**
     * @method OnWillUnmount
     * 视图即将关闭时通知模型。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t OnWillUnmount();
    /**
     * @method OnUnmount
     * 视图销毁时通知模型。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t OnUnmount();

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
     * @param {event_t*} e 事件对象。
     * @return {ret_t}
     *  如果当前分发的回调函数返回RET_STOP，dispatch中断分发，并返回RET_STOP，否则返回RET_OK。
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

  public:
    navigator_request_t* request;

  private:
    emitter_t emitter;
};

} /*namespae vm*/

#endif/*TK_VIEW_MODEL_ARRAY_HPP*/

