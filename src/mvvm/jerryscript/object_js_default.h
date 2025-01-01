/**
 * File:   object_js_default.h
 * Author: AWTK Develop Team
 * Brief:  wrap jerry script generic object to an tkc object.
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-31 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_JS_DEFAULT_H
#define TK_OBJECT_JS_DEFAULT_H

#include "mvvm/jerryscript/object_js_base.h"

BEGIN_C_DECLS

/**
 * @class object_js_default_t
 * @parent tk_object_t
 *
 * 将通用的jerry script object包装成tkc形式的object。
 *
 */
typedef struct _object_js_default_t {
  object_js_base_t base;
} object_js_default_t;

/**
 * @method object_js_default_create
 * 创建jerry script object对象。
 *
 * @annotation ["constructor"]
 * @param {jsvalue_t} jsobj jerryscript对象。
 * @param {bool_t} free_handle object销毁的同时释放jerryscript对象。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* object_js_default_create(jsvalue_t jsobj, bool_t free_handle);

/**
 * @method object_is_object_js_default
 * 检查是否为jerry script object对象。
 *
 * @param {jsvalue_t} obj object对象。
 *
 * @return {bool_t} 返回TRUE表示是jerry script object，否则不是。
 */
bool_t object_is_object_js_default(tk_object_t* obj);

object_js_default_t* object_js_default_cast(tk_object_t* obj);
#define OBJECT_JS_DEFAULT(obj) object_js_default_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_JS_DEFAULT_H*/
