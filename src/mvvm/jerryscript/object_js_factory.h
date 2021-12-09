/**
 * File:   object_js_factory.h
 * Author: AWTK Develop Team
 * Brief:  object jerryscript factory
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-02-24 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#ifndef TK_object_js_FACTORY_H
#define TK_object_js_FACTORY_H

#include "tkc/darray.h"
#include "tkc/object.h"
#include "mvvm/jerryscript/jsobj.h"

BEGIN_C_DECLS

typedef tk_object_t* (*object_js_create_t)(jsvalue_t jsobj, bool_t free_handle);

/**
 * @class object_js_factory_t
 * object jerryscript工厂。
 *
 */
typedef struct _object_js_factory_t {
  /*private*/
  darray_t creators;
} object_js_factory_t;

/**
 * @method object_js_factory
 * 获取缺省的object jerryscript工厂对象。
 * @annotation ["constructor"]
 * @return {object_js_factory_t*} 返回object jerryscript工厂对象。
 */
object_js_factory_t* object_js_factory(void);

/**
 * @method object_js_factory_set
 * 设置缺省的object jerryscript工厂对象。
 * @param {object_js_factory_t*} factory object jerryscript工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_js_factory_set(object_js_factory_t* factory);

/**
 * @method object_js_factory_create
 * 创建object jerryscript工厂对象。
 * @annotation ["constructor"]
 *
 * @return {object_js_factory_t*} 返回object jerryscript工厂对象。
 */
object_js_factory_t* object_js_factory_create(void);

/**
 * @method object_js_factory_register
 * 注册object jerryscript创建函数。
 * @param {object_js_factory_t*} factory reader工厂对象。
 * @param {jsobj_type_t} type jerryscript对象的类型。
 * @param {object_js_create_t} create object jerryscript创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_js_factory_register(object_js_factory_t* factory, jsobj_type_t type,
                                 object_js_create_t create);
/**
 * @method object_js_factory_create_object
 * 创建指定类型的object jerryscript对象。
 * @annotation ["constructor"]
 * @param {object_js_factory_t*} factory object jerryscript工厂对象。
 * @param {jsvalue_t} jsobj jerryscript对象。
 * @param {bool_t} free_handle object销毁的同时释放jerryscript对象。
 *
 * @return {object_js_t*} 返回object对象。
 */
tk_object_t* object_js_factory_create_object(object_js_factory_t* factory, jsvalue_t jsobj,
                                             bool_t free_handle);

/**
 * @method object_js_factory_destroy
 * 析构并释放object jerryscript工厂对象。
 * @param {object_js_factory_t*} factory object jerryscript工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t object_js_factory_destroy(object_js_factory_t* factory);

END_C_DECLS

#endif /*TK_object_js_FACTORY_H*/
