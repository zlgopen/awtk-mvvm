/**
 * File:   binding_context_awtk.h
 * Author: AWTK Develop Team
 * Brief:  binding context awtk
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
 * 2019-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_CONTEXT_AWTK_H
#define TK_BINDING_CONTEXT_AWTK_H

#include "base/widget.h"
#include "mvvm/base/binding_context.h"

BEGIN_C_DECLS

/**
 * @class binding_context_awtk_t
 * @parent binding_context_t
 *
 * binding_context_awtk
 */

/**
 * @method binding_context_awtk_create
 * 创建binding_context对象。
 *
 * @param {binding_context_t*} parent 父的binding_context对象。
 * @param {const char*} vmodel ViewModel的名称。
 * @param {navigator_request_t*} req 请求参数。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {binding_context_t*} 返回binding_context对象。
 */
binding_context_t* binding_context_awtk_create(binding_context_t* parent, const char* vmodel,
                                               navigator_request_t* req, widget_t* widget);

END_C_DECLS

#endif /*TK_BINDING_CONTEXT_AWTK_H*/
