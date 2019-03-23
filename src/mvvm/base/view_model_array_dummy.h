/**
 * File:   view_model_array_dummy.h
 * Author: AWTK Develop Team
 * Brief:  view_model_array dummy
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ARRAY_DUMMY_H
#define TK_VIEW_MODEL_ARRAY_DUMMY_H

#include "tkc/darray.h"
#include "mvvm/base/view_model_array.h"

BEGIN_C_DECLS

struct _model_array_dummy_t;
typedef struct _model_array_dummy_t view_model_array_dummy_t;

/**
 * @class view_model_array_dummy_t
 * @parent view_model_array_dummy_t
 *
 * array view_model
 *
 */
struct _model_array_dummy_t {
  view_model_array_t view_model_array;

  /*private*/
  darray_t array;
};

/**
 * @method view_model_array_dummy_create
 * 创建array模型对象。
 *
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_array_dummy_create(navigator_request_t* req);

/**
 * @method view_model_array_dummy_clear
 * 清除全部submodel。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_dummy_clear(view_model_t* view_model);

/**
 * @method view_model_array_dummy_add
 * 增加submodel。
 *
 *> 增加submodel的引用计数，并保存submodel的引用。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {view_model_t*} submodel submodel对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_dummy_add(view_model_t* view_model, view_model_t* submodel);

/**
 * @method view_model_array_dummy_remove
 * 删除指定的submodel。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {uint32_t} index submodel的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t view_model_array_dummy_remove(view_model_t* view_model, uint32_t index);

/**
 * @method view_model_array_dummy_get
 * 获取指定的submodel。
 *
 * @param {view_model_t*} view_model view_model对象。
 * @param {uint32_t} index submodel的索引。
 *
 * @return {view_model_t} 返回指定的submodel。
 */
view_model_t* view_model_array_dummy_get(view_model_t* view_model, uint32_t index);

/**
 * @method view_model_array_dummy_size
 * 获取submodel的个数。
 *
 * @param {view_model_t*} view_model view_model对象。
 *
 * @return {int32_t} 返回submodel的个数。
 */
int32_t view_model_array_dummy_size(view_model_t* view_model);

#define VIEW_MODEL_ARRAY_DUMMY(view_model) ((view_model_array_dummy_t*)(view_model))

END_C_DECLS

#endif /*TK_VIEW_MODEL_ARRAY_DUMMY_H*/
