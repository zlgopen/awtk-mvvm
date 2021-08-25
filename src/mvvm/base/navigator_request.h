/**
 * File:   navigator_request.h
 * Author: AWTK Develop Team
 * Brief:  navigator request
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
 * 2019-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NAVIGATOR_REQUEST_H
#define TK_NAVIGATOR_REQUEST_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _navigator_request_t;
typedef struct _navigator_request_t navigator_request_t;
typedef ret_t (*navigator_request_on_result_t)(navigator_request_t* req, const value_t* result);

/**
 * @class navigator_request_t
 * @parent object_t
 * 导航请求。
 * 如果需要传递参数或自定义请求时，才需要本类，否则直接使用navigator的API即可。
 */
struct _navigator_request_t {
  object_t object;

  /**
   * @property {value_t} result
   * @annotation ["readable"]
   * 用于同步请求返回结果。
   */
  value_t result;

  /**
   * @property {navigator_request_on_result_t} on_result
   * @annotation ["readable"]
   * 用于异步请求返回结果。
   */
  navigator_request_on_result_t on_result;

  /**
   * @property {void*} user_data
   * @annotation ["readable"]
   * 用户自定义数据。
   */
  void* user_data;

  /*private*/
  object_t* args;
};

/**
 * @method navigator_request_create
 * 创建request对象。
 * > 请求信息可以是普通字符串，比如"abc"表示参数target为"abc"；
 * > 也可以是"string?"前缀的形式，比如"string?arg1=xx&arg2=xx"表示参数arg1为"xx"、参数arg2为"xx"。
 * > 请求信息中的参数无顺序要求，可选参数请参阅navigator_request_argument_type_t
 *
 * @annotation ["constructor"]
 *
 * @param {const char*} args 请求的参数
 * @param {navigator_request_on_result_t} on_result 用于非模态窗口返回结果的回调函数。
 *
 * @return {navigator_request_t*} 返回request对象。
 */
navigator_request_t* navigator_request_create(const char* args,
                                              navigator_request_on_result_t on_result);

/**
 * @method navigator_request_set_args
 * 设置导航请求的参数。
 *
 * @annotation ["scriptable"]
 *
 * @param {navigator_request_t*} req request对象。
 * @param {object_t*} args 导航请求的参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_request_set_args(navigator_request_t* req, object_t* args);

/**
 * @method navigator_request_on_result
 * navigator hander调用本函数返回结果。
 *
 * @annotation ["scriptable"]
 *
 * @param {navigator_request_t*} req request对象。
 * @param {const value_t*} result 结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_request_on_result(navigator_request_t* req, const value_t* result);

#define NAVIGATOR_REQUEST(req) ((navigator_request_t*)(req))

/**
 * @enum navigator_request_type_t
 * @annotation ["scriptable", "string"]
 * @prefix NAVIGATOR_REQ_
 * 导航请求的类型，导航请求中不指定请求的类型时默认为打开一个新的窗口。
 */

/**
 * @const NAVIGATOR_REQ_CLOSE
 * 请求关闭指定窗口。
 */
#define NAVIGATOR_REQ_CLOSE "_close_"

/**
 * @const NAVIGATOR_REQ_HOME
 * 请求回到主窗口。
 */
#define NAVIGATOR_REQ_HOME "_home_"

/**
 * @const NAVIGATOR_REQ_BACK
 * 请求关闭顶层（最上面的）窗口。
 */
#define NAVIGATOR_REQ_BACK "_back_"

/**
 * @const NAVIGATOR_REQ_TOAST
 * 请求打开内置的toast窗口。
 */
#define NAVIGATOR_REQ_TOAST "_toast_"

/**
 * @const NAVIGATOR_REQ_INFO
 * 请求打开内置的提示窗口。
 */
#define NAVIGATOR_REQ_INFO "_info_"

/**
 * @const NAVIGATOR_REQ_WARN
 * 请求打开内置的告警窗口。
 */
#define NAVIGATOR_REQ_WARN "_warn_"

/**
 * @const NAVIGATOR_REQ_CONFIRM
 * 请求打开内置的确认窗口。
 */
#define NAVIGATOR_REQ_CONFIRM "_confirm_"

/**
 * @const NAVIGATOR_REQ_PICK_COLOR
 * 请求打开内置的颜色选择窗口。
 */
#define NAVIGATOR_REQ_PICK_COLOR "_pick_color_"

/**
 * @const NAVIGATOR_REQ_PICK_FILE
 * 请求打开内置的文件选择窗口。
 */
#define NAVIGATOR_REQ_PICK_FILE "_pick_file_"

/**
 * @const NAVIGATOR_REQ_PICK_DIR
 * 请求打开内置的文件夹选择窗口。
 */
#define NAVIGATOR_REQ_PICK_DIR "_pick_dir_"

/**
 * @const NAVIGATOR_REQ_COUNT_VIEW_MODEL
 * 请求获取指定的 View Model 的数量。
 */
#define NAVIGATOR_REQ_COUNT_VIEW_MODEL "_count_view_model_"

/**
 * @const NAVIGATOR_REQ_GET_VIEW_MODEL
 * 请求获取指定的 View Model。
 */
#define NAVIGATOR_REQ_GET_VIEW_MODEL "_get_view_model_"

/**
 * @const NAVIGATOR_REQ_NOTIFY_VIEW_MODEL
 * 请求触发指定 View Model 的变化事件。
 */
#define NAVIGATOR_REQ_NOTIFY_VIEW_MODEL "_notify_view_model_"

/**
 * @enum navigator_request_argument_type_t
 * @annotation ["scriptable", "string"]
 * @prefix NAVIGATOR_ARG_
 * 导航请求的可选参数。
 */

/**
 * @const NAVIGATOR_ARG_REQ
 * 导航请求的类型。
 */
#define NAVIGATOR_ARG_REQ "request"

/**
 * @const NAVIGATOR_ARG_TARGET
 * 目标对象（比如窗口或ViewModel）的名称或路径。
 */
#define NAVIGATOR_ARG_TARGET "target"

/**
 * @const NAVIGATOR_ARG_OPEN_NEW
 * 存在同名窗口时，是否打开新的窗口，当为true时则打开一个新的窗口，否则切换到已存在的窗口。
 */
#define NAVIGATOR_ARG_OPEN_NEW "open_new"

/**
 * @const NAVIGATOR_ARG_CLOSE_CURRENT
 * 是否关闭当前窗口。
 */
#define NAVIGATOR_ARG_CLOSE_CURRENT "close_current"

/**
 * @const NAVIGATOR_ARG_FORCE
 * 是否强制执行。
 */
#define NAVIGATOR_ARG_FORCE "force"

/**
 * @const NAVIGATOR_ARG_MIN
 * 最小值。
 */
#define NAVIGATOR_ARG_MIN "min"

/**
 * @const NAVIGATOR_ARG_MAX
 * 最大值。
 */
#define NAVIGATOR_ARG_MAX "max"

/**
 * @const NAVIGATOR_ARG_TITLE
 * 标题。
 */
#define NAVIGATOR_ARG_TITLE "title"

/**
 * @const NAVIGATOR_ARG_TIPS
 * 提示信息。
 */
#define NAVIGATOR_ARG_TIPS "tips"

/**
 * @const NAVIGATOR_ARG_DEFAULT
 * 默认值。
 */
#define NAVIGATOR_ARG_DEFAULT "default"

/**
 * @const NAVIGATOR_ARG_DURATION
 * 超时时间。
 */
#define NAVIGATOR_ARG_DURATION "timeout"

/**
 * @const NAVIGATOR_ARG_CONTENT
 * 内容。
 */
#define NAVIGATOR_ARG_CONTENT "content"

/**
 * @const NAVIGATOR_ARG_FOR_SAVE
 * 用于保存。
 */
#define NAVIGATOR_ARG_FOR_SAVE "for_save"

/**
 * @const NAVIGATOR_ARG_FILTER
 * 过滤的内容。
 */
#define NAVIGATOR_ARG_FILTER "filter"

/**
 * @const NAVIGATOR_ARG_PROPS_CHANGED
 * 是否为属性改变的事件。
 */
#define NAVIGATOR_ARG_PROPS_CHANGED "props_changed"

/**
 * @const NAVIGATOR_ARG_PROPS_CHANGED
 * 是否为集合中的项目增加或者删除的事件。
 */
#define NAVIGATOR_ARG_ITEMS_CHANGED "items_changed"

/**
 * @const NAVIGATOR_ARG_EVENT_SOURCE
 * 事件源。
 */
#define NAVIGATOR_ARG_EVENT_SOURCE "event_source"

END_C_DECLS

#endif /*TK_NAVIGATOR_REQUEST_H*/
