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
 *
 * 导航请求，请求打开指定的窗口。
 *
 * 如果需要传递参数给要打开的窗口或需要返回结果给调用者时，才需要本类，否则指定窗口的名字就行了。
 *
 */
struct _navigator_request_t {
  object_t object;

  /**
   * @property {char*} target
   * @annotation ["readable"]
   * 目标窗口的名称。
   */
  char target[TK_NAME_LEN + 1];

  /**
   * @property {value_t} result
   * @annotation ["readable"]
   * 用于模态窗口返回结果。
   */
  value_t result;

  /**
   * @property {navigator_request_on_result_t} on_result
   * @annotation ["readable"]
   * 用于非模态窗口返回结果。
   */
  navigator_request_on_result_t on_result;

  /**
   * @property {bool_t} close_current
   * @annotation ["readable"]
   * 是否关闭当前窗口。
   */
   bool_t close_current;
  
   /**
   * @property {bool_t} open_new
   * @annotation ["readable"]
   * 是否总是打开新窗口。
   */
   bool_t open_new;

  void* user_data;
  /*private*/
  object_t* args;
};

/**
 * @method navigator_request_create
 * 创建request对象。
 *
 * @annotation ["constructor"]
 *
 * @param {const char*} target 目标窗口的名称。
 * @param {navigator_request_on_result_t} on_result 用于非模态窗口返回结果的回调函数。
 *
 * @return {navigator_request_t*} 返回request对象。
 */
navigator_request_t* navigator_request_create(const char* target,
                                              navigator_request_on_result_t on_result);

/**
 * @method navigator_request_set_close_current
 * 设置是否关闭当前窗口。
 *
 * @annotation ["scriptable"]
 *
 * @param {navigator_request_t*} req request对象。
 * @param {bool_t} close_current 关闭当前窗口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_request_set_close_current(navigator_request_t* req, bool_t close_current);

/**
 * @method navigator_request_set_open_new
 * 设置是否打开新窗口。
 * > 缺省情况下，如果目标窗口存在，则切换到目标窗口，否则打开新窗口。
 *
 * @annotation ["scriptable"]
 *
 * @param {navigator_request_t*} req request对象。
 * @param {bool_t} open_new 是否打开新窗口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_request_set_open_new(navigator_request_t* req, bool_t open_new);

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

#define NAVIGATOR_REQ_TOAST "_toast_"
#define NAVIGATOR_REQ_HOME "_home_"
#define NAVIGATOR_REQ_BACK "_back_"
#define NAVIGATOR_REQ_INFO "_info_"
#define NAVIGATOR_REQ_WARN "_warn_"
#define NAVIGATOR_REQ_CLOSE "_close_"
#define NAVIGATOR_REQ_CONFIRM "_confirm_"
#define NAVIGATOR_REQ_INPUT_TEXT "_input_text_"
#define NAVIGATOR_REQ_INPUT_FLOAT "_input_float_"
#define NAVIGATOR_REQ_INPUT_INT "_input_int_"
#define NAVIGATOR_REQ_PICK_COLOR "_pick_color_"
#define NAVIGATOR_REQ_PICK_FILE "_pick_file_"
#define NAVIGATOR_REQ_PICK_DIR "_pick_dir_"

#define NAVIGATOR_ARG_TARGET "target"
#define NAVIGATOR_ARG_OPEN_NEW "open_new"
#define NAVIGATOR_ARG_CLOSE_CURRENT "close_current"

#define NAVIGATOR_ARG_NAME "name"
#define NAVIGATOR_ARG_MIN "min"
#define NAVIGATOR_ARG_MAX "max"
#define NAVIGATOR_ARG_TITLE "title"
#define NAVIGATOR_ARG_TIPS "tips"
#define NAVIGATOR_ARG_DEFAULT "default"
#define NAVIGATOR_ARG_DURATION "timeout"
#define NAVIGATOR_ARG_CONTENT "content"
#define NAVIGATOR_ARG_FOR_SAVE "for_save"
#define NAVIGATOR_ARG_FILTER "filter"
#define NAVIGATOR_ARG_VIEW "__view__"

END_C_DECLS

#endif /*TK_NAVIGATOR_REQUEST_H*/
