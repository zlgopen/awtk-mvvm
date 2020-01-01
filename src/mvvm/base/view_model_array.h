/**
 * File:   view_model_array.h
 * Author: AWTK Develop Team
 * Brief:  view_model_array
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
 * @method view_model_array_notify_items_changed
 * 触发items改变事件。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_notify_items_changed(view_model_t* view_model);

#define VIEW_MODEL_ARRAY(view_model) ((view_model_array_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_ARRAY_H*/
