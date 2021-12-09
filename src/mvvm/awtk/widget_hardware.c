/**
 * File:   widget_hardware..c
 * Author: AWTK Develop Team
 * Brief:  wrap hardware device to a widget
 *
 * Copyright (c) 2019 - 2020 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "widget_hardware.h"
#include "mvvm/base/binding_rule.h"

static ret_t widget_hardware_get_prop(widget_t* widget, const char* name, value_t* v) {
  widget_hardware_t* widget_hardware = WIDGET_HARDWARE(widget);
  return_value_if_fail(widget_hardware != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    value_set_str(v, widget_hardware->device->name);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TYPE)) {
    value_set_str(v, widget_hardware->type);
    return RET_OK;
  } else {
    return tk_object_get_prop(widget_hardware->device, name, v);
  }
}

static ret_t widget_hardware_set_prop(widget_t* widget, const char* name, const value_t* v) {
  widget_hardware_t* widget_hardware = WIDGET_HARDWARE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(name, BINDING_RULE_DATA_PREFIX)) {
    return RET_NOT_FOUND;
  } else if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    return tk_object_set_name(widget_hardware->device, value_str(v));
  } else {
    return tk_object_set_prop(widget_hardware->device, name, v);
  }
}

static ret_t widget_hardware_on_destroy(widget_t* widget) {
  widget_hardware_t* widget_hardware = WIDGET_HARDWARE(widget);
  return_value_if_fail(widget != NULL && widget_hardware != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(widget_hardware->type);
  emitter_off_by_ctx(EMITTER(widget_hardware->device), widget);
  tk_object_unref(widget_hardware->device);

  return RET_OK;
}

static ret_t widget_hardware_on_paint_self(widget_t* widget, canvas_t* c) {
  widget_hardware_t* widget_hardware = WIDGET_HARDWARE(widget);

  (void)widget_hardware;

  return RET_OK;
}

const char* s_widget_hardware_properties[] = {NULL};
widget_t* widget_hardware_create_internal(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

TK_DECL_VTABLE(widget_hardware) = {.size = sizeof(widget_hardware_t),
                                   .type = WIDGET_TYPE_WIDGET_HARDWARE,
                                   .clone_properties = s_widget_hardware_properties,
                                   .persistent_properties = s_widget_hardware_properties,
                                   .parent = TK_PARENT_VTABLE(widget),
                                   .create = widget_hardware_create_internal,
                                   .on_paint_self = widget_hardware_on_paint_self,
                                   .set_prop = widget_hardware_set_prop,
                                   .get_prop = widget_hardware_get_prop,
                                   .on_destroy = widget_hardware_on_destroy};

widget_t* widget_hardware_create_internal(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(widget_hardware), x, y, w, h);
  widget_hardware_t* widget_hardware = WIDGET_HARDWARE(widget);
  return_value_if_fail(widget_hardware != NULL, NULL);

  return widget;
}

#include "mvvm/hardware/device_factory.h"

static ret_t on_device_value_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  e->target = widget;

  return widget_dispatch(widget, e);
}

widget_t* widget_hardware_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h, const char* type,
                                 const char* args) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(widget_hardware), x, y, w, h);
  widget_hardware_t* widget_hardware = WIDGET_HARDWARE(widget);
  return_value_if_fail(widget_hardware != NULL, NULL);

  widget_hardware->type = tk_strdup(type);
  widget_hardware->device = device_factory_create_device(type, args);
  ENSURE(widget_hardware->device != NULL);
  emitter_on(EMITTER(widget_hardware->device), EVT_VALUE_CHANGED, on_device_value_changed, widget);
  emitter_on(EMITTER(widget_hardware->device), EVT_VALUE_CHANGING, on_device_value_changed, widget);

  return widget;
}

widget_t* widget_hardware_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, widget_hardware), NULL);

  return widget;
}
