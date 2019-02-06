/**
 * File:   value_converter_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented value_converter
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "jerryscript-port.h"
#include "jerryscript-ext/handler.h"
#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"

static ret_t value_converter_jerryscript_on_destroy(object_t* obj) {
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
  object_t* obj = OBJECT(c);
  value_converter_jerryscript_t* jsconverter = VALUE_CONVERTER_JERRYSCRIPT(c);

  return jsvalue_converter_to_view(obj->name, from, to, &(jsconverter->temp));
}

static ret_t value_converter_jerryscript_to_model(value_converter_t* c, const value_t* from,
                                                  value_t* to) {
  object_t* obj = OBJECT(c);
  value_converter_jerryscript_t* jsconverter = VALUE_CONVERTER_JERRYSCRIPT(c);

  return jsvalue_converter_to_model(obj->name, from, to, &(jsconverter->temp));
}

static value_converter_t* value_converter_jerryscript_create(const char* name) {
  object_t* obj = NULL;
  value_converter_t* value_convert = NULL;
  value_converter_jerryscript_t* jsconverter = NULL;
  return_value_if_fail(jsvalue_converter_exist(name), NULL);

  obj = object_create(&s_value_converter_jerryscript_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_CONVERTER(obj);
  value_convert->to_view = value_converter_jerryscript_to_view;
  value_convert->to_model = value_converter_jerryscript_to_model;

  jsconverter = VALUE_CONVERTER_JERRYSCRIPT(obj);
  str_init(&(jsconverter->temp), 0);

  object_set_name(obj, name);

  return value_convert;
}

ret_t value_converter_jerryscript_init(void) {
  return value_converter_init(value_converter_jerryscript_create);
}

ret_t value_converter_jerryscript_deinit(void) {
  return value_converter_deinit();
}
