/**
 * File:   view_model_array.h
 * Author: AWTK Develop Team
 * Brief:  view_model_array
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ARRAY_H
#define TK_VIEW_MODEL_ARRAY_H

#include "tkc/darray.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_array_t;
typedef struct _model_array_t view_model_array_t;

/**
 * @class view_model_array_t
 * @parent view_model_t
 *
 * array view_model
 *
 */
struct _model_array_t {
  view_model_t view_model;

  /**
   * @property {uint32_t} cursor
   * @annotation ["readable"]
   * 当前可以访问的submodel。
   */
  uint32_t cursor;

  /**
   * @property {uint32_t} selected_index
   * @annotation ["readable"]
   * 当前选择的项。
   */
  uint32_t selected_index;

  /*private*/
  str_t temp_prop;
  str_t temp_expr;
};

/**
 * @method view_model_array_init
 * 初始化。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_array_init(view_model_t* view_model);

/**
 * @method view_model_array_deinit
 * ~初始化。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_deinit(view_model_t* view_model);

/**
 * @method view_model_array_inc_cursor
 * 增加cursor的值。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_inc_cursor(view_model_t* view_model);

/**
 * @method view_model_array_set_cursor
 * 设置cursor的值。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {uint32_t} cursor 的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_set_cursor(view_model_t* view_model, uint32_t cursor);

/**
 * @method view_model_array_set_selected_index
 * 选中指定项。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {uint32_t} index 选定项的序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_set_selected_index(view_model_t* view_model, uint32_t index);

/**
 * @method view_model_array_notify_items_changed
 * 触发items改变事件。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_notify_items_changed(view_model_t* view_model);

/*for subclass*/
/**
 * @method view_model_array_default_set_prop
 * set prop的默认实现。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 属性名。
 * @param {const value_t*} v 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_default_set_prop(view_model_t* view_model, const char* name,
                                        const value_t* value);

/**
 * @method view_model_array_default_get_prop
 * get prop的默认实现。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 属性名。
 * @param {value_t*} v 属性值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_default_get_prop(view_model_t* view_model, const char* name, value_t* value);

/**
 * @method view_model_array_default_exec
 * exec的默认实现。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令参数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_default_exec(view_model_t* view_model, const char* name, const char* args);

/**
 * @method view_model_array_default_can_exec
 * can exec的默认实现。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {const char*} name 命令名。
 * @param {const char*} args 命令参数。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不能执行。
 */
bool_t view_model_array_default_can_exec(view_model_t* view_model, const char* name,
                                         const char* args);

#define VIEW_MODEL_ARRAY(view_model) ((view_model_array_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_ARRAY_H*/
