/**
 * File:   shape.h
 * Author: AWTK Develop Team
 * Brief:  shape
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef SHAPE_H
#define SHAPE_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class shape_t
 *
 * @annotation ["model"]
 * 温度控制器。
 *
 */
typedef struct _shape_t {
  /**
   * @property {int32_t} type
   * @annotation ["readable", "writable"]
   * 类型。
   */
  int32_t type;
  
  /**
   * @property {int32_t} x
   * @annotation ["readable", "writable"]
   * x。
   */
  int32_t x;
  
  /**
   * @property {int32_t} y
   * @annotation ["readable", "writable"]
   * y。
   */
  int32_t y;
  
  /**
   * @property {int32_t} w
   * @annotation ["readable", "writable"]
   * w。
   */
  int32_t w;
  
  /**
   * @property {int32_t} h
   * @annotation ["readable", "writable"]
   * h。
   */
  int32_t h;
  
  /**
   * @property {int32_t} opacity
   * @annotation ["readable", "writable"]
   * 不透明度。
   */
  int32_t opacity;
  
  /**
   * @property {int32_t} text_align
   * @annotation ["readable", "writable"]
   * 文本对齐。
   */
  int32_t text_align;
  
  /**
   * @property {str_t} name 
   * @annotation ["readable", "writable"]
   * name。
   */
  str_t name;

  /**
   * @property {str_t} overview
   * @annotation ["readable", "writable"]
   * overview。
   */
  str_t overview;

  /*private*/
  int32_t saved_type;
} shape_t;

/**
 * @method shape_create
 * 创建shape对象。
 *
 * @annotation ["constructor"]
 * @return {shape_t*} 返回shape对象。 
 */ 
shape_t* shape_create(void);

/**
 * @method shape_destroy
 * 销毁shape对象。
 *
 * @annotation ["destructor"]
 * @param {shape_t*} shape shape对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t shape_destroy(shape_t* shape);

/**
 * @method shape_set_name
 * 设置名称。
 *
 * @param {shape_t*} shape shape对象。
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t shape_set_name(shape_t* shape, const char* name);

/**
 * @method shape_get_overview
 * 获取overview。
 *
 * @param {shape_t*} shape shape对象。
 *
 * @return {const char*} 返回overview
 */ 
const char* shape_get_overview(shape_t* shape);

/**
 * @method shape_change_type
 * 改变shape的类型。
 *
 * @annotation ["command"]
 * @param {shape_t*} shape shape对象。
 * @param {int32_t} type 类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t shape_change_type(shape_t* shape, int32_t type); 

/**
 * @method shape_save
 * 保存设置。
 *
 * @annotation ["command"]
 * @param {shape_t*} shape shape对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */ 
ret_t shape_save(shape_t* shape);

/**
 * @method shape_can_save
 * 检查是否可以保存。
 *
 * @param {shape_t*} shape shape对象。
 *
 * @return {bool_t} 返回FALSE表示不能保存，否则表示可以保存。
 */ 
bool_t shape_can_save(shape_t* shape);

END_C_DECLS

#endif /*SHAPE_H*/

