/**
 * File:   object_js_array.h
 * Author: AWTK Develop Team
 * Brief:  wrap jerry script array object to an tkc object.
 *
 * Copyright (c) 2020 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_OBJECT_JS_ARRAY_H
#define TK_OBJECT_JS_ARRAY_H

#include "fscript_ext/fscript_array.h"
#include "mvvm/jerryscript/object_js_base.h"

BEGIN_C_DECLS

/**
 * @class object_js_array_t
 * @parent tk_object_t
 *
 * 将jerry script array object包装成tkc形式的object。
 *
 */
typedef struct _object_js_array_t {
  object_js_base_t base;
  const fscript_array_vtable_t* vt;
} object_js_array_t;

/**
 * @method object_js_array_create
 * 创建jerry script object对象。
 *
 * @annotation ["constructor"]
 * @param {jsvalue_t} jsobj jerryscript对象。
 * @param {bool_t} free_handle object销毁的同时释放jerryscript对象。
 *
 * @return {tk_object_t*} 返回object对象。
 */
tk_object_t* object_js_array_create(jsvalue_t jsobj, bool_t free_handle);

/**
 * @method object_is_object_js_array
 * 检查是否为jerry script object对象。
 *
 * @param {jsvalue_t} obj object对象。
 *
 * @return {bool_t} 返回TRUE表示是jerry script array object，否则不是。
 */
bool_t object_is_object_js_array(tk_object_t* obj);

object_js_array_t* object_js_array_cast(tk_object_t* obj);
#define OBJECT_JS_ARRAY(obj) object_js_array_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_JS_ARRAY_H*/
