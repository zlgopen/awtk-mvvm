/**
 * File:   object_js_base.h
 * Author: AWTK Develop Team
 * Brief:  object_js_base
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_OBJECT_JS_BASE_H
#define TK_OBJECT_JS_BASE_H

#include "tkc/str.h"
#include "tkc/object.h"
#include "mvvm/jerryscript/jsobj.h"

BEGIN_C_DECLS

/**
 * @class object_js_base_t
 * @parent tk_object_t
 *
 * 将object_js相关的公共行为进行抽象，以便重用。
 *
 */
typedef struct _object_js_base_t {
  tk_object_t object;

  /**
   * @property {jsvalue_t} jsobj
   * @annotation ["readable"]
   * jerry script object对象。
   *
   */
  jsvalue_t jsobj;

  /*private*/
  str_t temp;
  bool_t free_handle;
} object_js_base_t;

ret_t object_js_base_init(tk_object_t* obj, jsvalue_t jsobj, bool_t free_handle);
ret_t object_js_base_deinit(tk_object_t* obj);
int32_t object_js_base_compare(tk_object_t* obj, tk_object_t* other);
ret_t object_js_base_set_prop(tk_object_t* obj, const char* name, const value_t* v);
ret_t object_js_base_get_prop(tk_object_t* obj, const char* name, value_t* v);
ret_t object_js_base_remove_prop(tk_object_t* obj, const char* name);
ret_t object_js_base_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx);
bool_t object_js_base_can_exec(tk_object_t* obj, const char* name, const char* args);
ret_t object_js_base_exec(tk_object_t* obj, const char* name, const char* args);
bool_t object_js_base_is_listener_registered(tk_object_t* obj, tk_object_t* listener);
ret_t object_js_base_register_listener(tk_object_t* obj, tk_object_t* listener);
bool_t object_is_object_js(tk_object_t* obj);

#define OBJECT_JS_BASE(obj) (object_js_base_t*)(obj)

END_C_DECLS

#endif /*TK_OBJECT_JS_BASE_H*/
