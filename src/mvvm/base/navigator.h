/**
 * File:   navigator.h
 * Author: AWTK Develop Team
 * Brief:  navigator
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
 * 2019-01-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NAVIGATOR_H
#define TK_NAVIGATOR_H

#include "tkc/str.h"
#include "tkc/darray.h"
#include "mvvm/base/navigator_request.h"
#include "mvvm/base/navigator_handler.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _navigator_t;
typedef struct _navigator_t navigator_t;

typedef ret_t (*navigator_on_result_t)(navigator_t* nav, const value_t* result);

/**
 * @class navigator_t
 * @parent object_t
 *
 * 导航器。负责窗口导航。
 *
 */
struct _navigator_t {
  object_t object;

  /*private*/
  object_t* handlers;
};

/**
 * @method navigator_create
 * 创建navigator对象。
 *
 * @annotation ["constructor"]
 *
 * @return {ret_t} 返回navigator对象。
 */
navigator_t* navigator_create(void);

/**
 * @method navigator
 * 获取缺省的navigator对象。
 *
 * @annotation ["constructor"]
 *
 * @return {ret_t} 返回navigator对象。
 */
navigator_t* navigator(void);

/**
 * @method navigator_set
 * 设置缺省navigator对象。
 *
 * @param {navigator_t*} nav navigator对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_set(navigator_t* nav);

/**
 * @method navigator_handle_request
 * 处理打开窗口的请求。
 *
 * @param {navigator_t*} nav navigator对象。
 * @param {navigator_request_t*} req request对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_handle_request(navigator_t* nav, navigator_request_t* req);

/**
 * @method navigator_register_handler
 * 注册请求处理器。
 *
 * @param {navigator_t*} nav navigator对象。
 * @param {const char*} target 目标窗口的名称。
 * @param {navigator_handler_t*} handler 但请求打开target窗口时，执行本handler。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_register_handler(navigator_t* nav, const char* target,
                                 navigator_handler_t* handler);

/**
 * @method navigator_has_handler
 * 检查是否存在指定名称的请求处理器。
 *
 * @param {navigator_t*} nav navigator对象。
 * @param {const char*} target 目标窗口的名称。
 *
 * @return {ret_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t navigator_has_handler(navigator_t* nav, const char* target);

/**
 * @method navigator_unregister_handler
 * 注销请求处理器。
 *
 * @param {navigator_t*} nav navigator对象。
 * @param {const char*} target 目标窗口的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_unregister_handler(navigator_t* nav, const char* target);

/**
 * @method navigator_to
 * 发送指定的请求。
 * 发送请求时可以用"string?"为前缀、用"&"分隔的格式传递参数。
 * 比如，"string?arg1=xx&arg2=yy"表示有两个参数，参数arg1的值为"xx"，参数arg2的值为"yy",
 * 如果没有用上述格式指定参数，则默认为target参数的值。
 *
 * @annotation ["static"]
 *
 * @param {const char*} args 发送请求时要传递的参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_to(const char* args);

/**
 * @method navigator_to_by_object
 * 发送指定的请求。
 *
 * @annotation ["static"]
 *
 * @param {const char*} args 发送请求时要传递的参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_to_by_object(object_t* args);

/**
 * @method navigator_to_with_key_value
 * 请求打开指定的窗口。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target 目标窗口的名称。
 * @param {const char*} key 参数名。
 * @param {const char*} value 参数值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_to_with_key_value(const char* target, const char* key, const char* value);

/**
 * @method navigator_replace
 * 请求打开指定的窗口，并关闭当前窗口。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target 目标窗口的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_replace(const char* target);

/**
 * @method navigator_switch_to
 * 如果目标窗口已经存在，直接切换到该窗口，否则打开新窗口。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target 目标窗口的名称。
 * @param {bool_t} close_current 是否关闭当前窗口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_switch_to(const char* target, bool_t close_current);

/**
 * @method navigator_back_to_home
 * 回到主屏。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_back_to_home(void);

/**
 * @method navigator_back
 * 关闭当前窗口，回到前一窗口。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_back(void);

/**
 * @method navigator_close
 * 关闭指定窗口。
 *
 * @param {const char*} target 目标窗口的名称。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_close(const char* target);

/**
 * @method navigator_request_close
 * 请求关闭关闭指定窗口。
 *
 * > 窗口是否被关闭，取决于窗口本身的处理逻辑。
 *
 * @param {const char*} target 目标窗口的名称。
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_request_close(const char* target);

/**
 * @method navigator_toast
 * 显示toast信息。
 *
 * @annotation ["static"]
 *
 * @param {const char*} content 信息内容。
 * @param {uint32_t} timeout 显示时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_toast(const char* content, uint32_t timeout);

/**
 * @method navigator_info
 * 显示信息。
 *
 * @annotation ["static"]
 *
 * @param {const char*} title 标题。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_info(const char* title, const char* content);

/**
 * @method navigator_warn
 * 显示警告信息。
 *
 * @annotation ["static"]
 *
 * @param {const char*} title 标题。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_warn(const char* title, const char* content);

/**
 * @method navigator_confirm
 * 显示确认信息。
 *
 * @annotation ["static"]
 *
 * @param {const char*} title 标题。
 * @param {const char*} content 内容。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_confirm(const char* title, const char* content);

/**
 * @method navigator_pick_dir
 * 选择目录。
 *
 * @annotation ["static"]
 *
 * @param {const char*} title 标题。
 * @param {str_t*} result 用于传递缺省值和返回结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_pick_dir(const char* title, str_t* result);

/**
 * @method navigator_pick_file
 * 选择文件。
 *
 * @annotation ["static"]
 *
 * @param {const char*} title 标题。
 * @param {const char*} filter 文件过滤(如：.txt.html), NULL表示不过滤。
 * @param {bool_t} for_save TRUE表示用于保存，FALSE表示用于打开。
 * @param {str_t*} result 用于传递缺省值和返回结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_pick_file(const char* title, const char* filter, bool_t for_save, str_t* result);

/**
 * @method navigator_pick_color
 * 选择颜色。
 *
 * @annotation ["static"]
 *
 * @param {const char*} title 标题。
 * @param {str_t*} result 用于传递缺省值和返回结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_pick_color(const char* title, str_t* result);

/**
 * @method navigator_count_view_models
 * 获取指定的ViewModel实例的个数。
 * > 路径为NULL时表示当前全部的ViewModel实例；
 * > 路径为空字符串时表示最上面的窗口绑定的全部ViewModel实例；
 * > 其他则为实际的路径，比如“a.[0]”时表示属性name为"a"的窗体的第0个子控件上绑定的ViewModel实例。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target ViewModel路径
 *
 * @return {ret_t} 返回实例的个数。
 */
int32_t navigator_count_view_models(const char* target);

/**
 * @method navigator_get_view_models
 * 获取指定的ViewModel实例。
 * > 路径为NULL时表示当前全部的ViewModel实例；
 * > 路径为空字符串时表示最上面的窗口绑定的全部ViewModel实例；
 * > 其他则为具体的路径，比如“a.[0]”表示name属性值为"a"的窗体的第0个子控件上绑定的ViewModel实例。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target ViewModel路径
 * @param {darray_t*} result 返回ViewModel实例。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_get_view_models(const char* target, darray_t* result);

/**
 * @method navigator_notify_view_models_props_changed
 * 触发指定的ViewModel实例的props改变事件。
 * > 路径为NULL时表示当前全部的ViewModel实例；
 * > 路径为空字符串时表示最上面的窗口绑定的全部ViewModel实例；
 * > 其他则为具体的路径，比如“a.[0]”表示name属性值为"a"的窗体的第0个子控件上绑定的ViewModel实例。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target ViewModel路径
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_notify_view_models_props_changed(const char* target);

/**
 * @method navigator_notify_view_models_items_changed
 * 触发指定的ViewModel实例的items改变事件。
 * > 路径为NULL时表示当前全部的ViewModel实例；
 * > 路径为空字符串时表示最上面的窗口绑定的全部ViewModel实例；
 * > 其他则为具体的路径，比如“a.[0]”表示name属性值为"a"的窗体的第0个子控件上绑定的ViewModel实例。
 *
 * @annotation ["static"]
 *
 * @param {const char*} target ViewModel路径
 * @param {object_t*} items 发生变化的items对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_notify_view_models_items_changed(const char* target, object_t* items);

/**
 * @method navigator_to_ex
 * 发送指定的请求，并可传递参数和返回结果。
 *
 * @annotation ["static"]
 *
 * @param {navigator_request_t*} req request对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_to_ex(navigator_request_t* req);

#define NAVIGATOR(nav) ((navigator_t*)(nav))
#define NAVIGATOR_DEFAULT_HANDLER "default_handler"

END_C_DECLS

#endif /*TK_NAVIGATOR_H*/
