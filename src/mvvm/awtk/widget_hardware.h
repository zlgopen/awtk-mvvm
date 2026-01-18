/**
 * File:   widget_hardware.h
 * Author: AWTK Develop Team
 * Brief:  wrap hardware device to a widget
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-07-12 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_WIDGET_HARDWARE_H
#define TK_WIDGET_HARDWARE_H

#include "base/widget.h"

BEGIN_C_DECLS
/**
 * @class widget_hardware_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * wrap hardware device to a widget
 */
typedef struct _widget_hardware_t {
  widget_t widget;

  tk_object_t* device;
  char* type;
} widget_hardware_t;

/**
 * @method widget_hardware_create
 * @annotation ["constructor", "scriptable"]
 * 创建widget_hardware对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 * @param {const char*} type 设备类型。
 * @param {const char*} args 创建参数。
 *
 * @return {widget_t*} 对象。
 */
widget_t* widget_hardware_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h, const char* type,
                                 const char* args);

/**
 * @method widget_hardware_cast
 * 转换为widget_hardware对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget widget_hardware对象。
 *
 * @return {widget_t*} widget_hardware对象。
 */
widget_t* widget_hardware_cast(widget_t* widget);

#define WIDGET_TYPE_WIDGET_HARDWARE "widget_hardware"

#define WIDGET_HARDWARE(widget) ((widget_hardware_t*)(widget_hardware_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(widget_hardware);

END_C_DECLS

#endif /*TK_WIDGET_HARDWARE_H*/
