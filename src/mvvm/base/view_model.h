/**
 * File:   view_model.h
 * Author: AWTK Develop Team
 * Brief:  view_model
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
 * 2019-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_H
#define TK_VIEW_MODEL_H

#include "tkc/str.h"
#include "tkc/object.h"
#include "mvvm/base/navigator_request.h"

BEGIN_C_DECLS

struct _view_model_t;
typedef struct _view_model_t view_model_t;

typedef ret_t (*view_model_on_will_mount_t)(view_model_t* view_model, navigator_request_t* req);
typedef ret_t (*view_model_on_mount_t)(view_model_t* view_model);
typedef ret_t (*view_model_on_will_unmount_t)(view_model_t* view_model);
typedef ret_t (*view_model_on_unmount_t)(view_model_t* view_model);
typedef const char* (*view_model_preprocess_expr_t)(view_model_t* view_model, const char* expr);
typedef const char* (*view_model_preprocess_prop_t)(view_model_t* view_model, const char* prop);
typedef view_model_t* (*view_model_create_sub_view_model_t)(view_model_t* view_model,
                                                            const char* name);
typedef view_model_t* (*view_model_create_sub_view_model_array_t)(view_model_t* view_model,
                                                                  const char* name);
typedef view_model_t* (*view_model_create_t)(navigator_request_t* req);

typedef struct _model_vtable_t {
  view_model_on_will_mount_t on_will_mount;
  view_model_on_mount_t on_mount;
  view_model_on_will_unmount_t on_will_unmount;
  view_model_on_unmount_t on_unmount;
  view_model_create_sub_view_model_t create_sub_view_model;
  view_model_create_sub_view_model_array_t create_sub_view_model_array;
} view_model_vtable_t;

/**
 * @class view_model_t
 * @parent tk_object_t
 *
 * 视图模型的基类。
 *
 */
struct _view_model_t {
  tk_object_t object;

  /*private*/
  str_t temp;
  str_t last_error;
  const view_model_vtable_t* vt;

  view_model_t* parent;

  view_model_preprocess_expr_t preprocess_expr;
  view_model_preprocess_prop_t preprocess_prop;
};

/**
 * @method view_model_init
 * 初始化。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_init(view_model_t* view_model);

/**
 * @method view_model_deinit
 * ~初始化。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_deinit(view_model_t* view_model);

/**
 * @method view_model_on_will_mount
 * 打开视图即将加载模型时通知view_model。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {navigator_request_t*} req request对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_on_will_mount(view_model_t* view_model, navigator_request_t* req);

/**
 * @method view_model_on_mount
 * 视图与模型绑定完成后通知模型。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_on_mount(view_model_t* view_model);

/**
 * @method view_model_on_will_unmount
 * 视图即将关闭时通知模型。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_on_will_unmount(view_model_t* view_model);

/**
 * @method view_model_on_unmount
 * 视图销毁时通知模型。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_on_unmount(view_model_t* view_model);

/**
 * @method view_model_preprocess_expr
 * 对表达式预处理。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} expr 表达式。
 *
 * @return {ret_t} 返回处理后的表达式。
 */
const char* view_model_preprocess_expr(view_model_t* view_model, const char* expr);

/**
 * @method view_model_preprocess_prop
 * 对属性进行预处理。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} prop 表达式。
 *
 * @return {ret_t} 返回处理后的表达式。
 */
const char* view_model_preprocess_prop(view_model_t* view_model, const char* prop);

/**
 * @method view_model_has_prop
 * 检查指定的属性是否存在。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 属性名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
bool_t view_model_has_prop(view_model_t* view_model, const char* name);

/**
 * @method view_model_get_prop
 * 获取指定属性的值。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 属性名。
 * @param {value_t*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_get_prop(view_model_t* view_model, const char* name, value_t* value);

/**
 * @method view_model_set_prop
 * 设置指定属性的值。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 属性名。
 * @param {value_t*} value 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_set_prop(view_model_t* view_model, const char* name, const value_t* value);

/**
 * @method view_model_can_exec
 * 检查指定的命令是否可以执行。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令的参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t view_model_can_exec(view_model_t* view_model, const char* name, const char* args);

/**
 * @method view_model_exec
 * 执行指定的命令。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令的参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_exec(view_model_t* view_model, const char* name, const char* args);

/**
 * @method view_model_notify_props_changed
 * 触发props改变事件。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_notify_props_changed(view_model_t* view_model);

/**
 * @method view_model_notify_items_changed
 * 触发items改变事件。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {tk_object_t*} target 发生变化的items对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_notify_items_changed(view_model_t* view_model, tk_object_t* target);

/**
 * @method view_model_create_sub_view_model
 * 创建子ViewModel。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
view_model_t* view_model_create_sub_view_model(view_model_t* view_model, const char* name);

/**
 * @method view_model_create_sub_view_model_array
 * 创建子ViewModelArray。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
view_model_t* view_model_create_sub_view_model_array(view_model_t* view_model, const char* name);

#define VIEW_MODEL(view_model) ((view_model_t*)(view_model))

#define VIEW_MODEL_PROP_ITEMS "items"
#define VIEW_MODEL_PROP_CURSOR "index"
#define VIEW_MODEL_PROP_SELECTED_INDEX "selected_index"

#define VIEW_MODEL_CMD_SET_SELECTED "set_selected"

/**
 * @enum view_model_event_type_t
 *
 * 模型相关的事件。
 *
 */
typedef enum _model_event_type_t {
  /**
   * @const EVT_VIEW_MODEL_WILL_MOUNT
   *
   * 打开视图即将加载模型时通知view_model。
   */
  EVT_VIEW_MODEL_WILL_MOUNT = 0xff,
  /**
   * @const EVT_VIEW_MODEL_MOUNT
   *
   * 视图与模型绑定完成后通知模型。
   */
  EVT_VIEW_MODEL_MOUNT,
  /**
   * @const EVT_VIEW_MODEL_WILL_UNMOUNT
   *
   * 视图即将关闭时通知模型。
   */
  EVT_VIEW_MODEL_WILL_UNMOUNT,
  /**
   * @const EVT_VIEW_MODEL_UNMOUNT
   *
   * 视图销毁时通知模型。
   */
  EVT_VIEW_MODEL_UNMOUNT,
} view_model_event_type_t;

/**
 * @class view_model_will_mount_event_t
 * @parent event_t
 * 打开视图即将加载模型时通知view_model时的数据结构。
 *
 */
typedef struct _model_will_mount_event_t {
  event_t e;

  /**
   * @property {navigator_request_t*} req
   * 请求对象。
   */
  navigator_request_t* req;
} view_model_will_mount_event_t;

END_C_DECLS

#endif /*TK_VIEW_MODEL_H*/
