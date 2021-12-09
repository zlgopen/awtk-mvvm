/**
 * File:   value_converter_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented value_converter
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"

static ret_t value_converter_jerryscript_on_destroy(tk_object_t* obj) {
  value_converter_jerryscript_t* jsconverter = VALUE_CONVERTER_JERRYSCRIPT(obj);

  str_reset(&(jsconverter->temp));

  return RET_OK;
}

static const object_vtable_t s_value_converter_jerryscript_vtable = {
    .type = "value_converter_jerryscript",
    .desc = "value_converter_jerryscript",
    .size = sizeof(value_converter_jerryscript_t),
    .on_destroy = value_converter_jerryscript_on_destroy};

static ret_t value_converter_jerryscript_to_view(value_converter_t* c, const value_t* from,
                                                 value_t* to) {
  ret_t ret = RET_OK;
  value_converter_jerryscript_t* jsconverter = VALUE_CONVERTER_JERRYSCRIPT(c);
  str_t* temp = &(jsconverter->temp);
  jsvalue_t converter = jsvalue_get_value_converter(TK_OBJECT(c)->name);

  ret = js_value_converter_to_view(converter, from, to, temp);
  jsvalue_unref(converter);

  return ret;
}

static ret_t value_converter_jerryscript_to_model(value_converter_t* c, const value_t* from,
                                                  value_t* to) {
  ret_t ret = RET_OK;
  value_converter_jerryscript_t* jsconverter = VALUE_CONVERTER_JERRYSCRIPT(c);
  str_t* temp = &(jsconverter->temp);
  jsvalue_t converter = jsvalue_get_value_converter(TK_OBJECT(c)->name);

  ret = js_value_converter_to_model(converter, from, to, temp);
  jsvalue_unref(converter);

  return ret;
}

static value_converter_t* value_converter_jerryscript_create(const char* name) {
  tk_object_t* obj = NULL;
  jsvalue_t jsobj = jsvalue_get_value_converter(name);
  return_value_if_fail(jsvalue_check(jsobj) == RET_OK, NULL);

  if (jsvalue_is_object(jsobj)) {
    obj = tk_object_create(&s_value_converter_jerryscript_vtable);
    if (obj != NULL) {
      value_converter_t* converter = VALUE_CONVERTER(obj);
      value_converter_jerryscript_t* jsconverter = VALUE_CONVERTER_JERRYSCRIPT(obj);

      converter->to_view = value_converter_jerryscript_to_view;
      converter->to_model = value_converter_jerryscript_to_model;
      str_init(&(jsconverter->temp), 0);
      tk_object_set_name(obj, name);
    }
  }

  jsvalue_unref(jsobj);

  return VALUE_CONVERTER(obj);
}

ret_t value_converter_jerryscript_init(void) {
  return value_converter_register_generic(value_converter_jerryscript_create);
}

ret_t value_converter_jerryscript_deinit(void) {
  return RET_OK;
}
