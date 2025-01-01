/**
 * File:   object_js_default.c
 * Author: AWTK Develop Team
 * Brief:  wrap jerry script generic object to an tkc object.
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/value.h"
#include "mvvm/jerryscript/object_js_default.h"

static ret_t object_js_default_on_destroy(tk_object_t* obj) {
  object_js_default_t* o = OBJECT_JS_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return object_js_base_deinit(obj);
}

static const object_vtable_t s_object_js_default_vtable = {
    .type = "object_js_default",
    .desc = "object_js_default",
    .size = sizeof(object_js_default_t),
    .is_collection = FALSE,
    .on_destroy = object_js_default_on_destroy,
    .compare = object_js_base_compare,
    .get_prop = object_js_base_get_prop,
    .set_prop = object_js_base_set_prop,
    .remove_prop = object_js_base_remove_prop,
    .foreach_prop = object_js_base_foreach_prop,
    .can_exec = object_js_base_can_exec,
    .exec = object_js_base_exec};

tk_object_t* object_js_default_create(jsvalue_t jsobj, bool_t free_handle) {
  tk_object_t* obj = tk_object_create(&s_object_js_default_vtable);
  object_js_default_t* o = OBJECT_JS_DEFAULT(obj);
  return_value_if_fail(o != NULL, NULL);

  object_js_base_init(obj, jsobj, free_handle);

  return obj;
}

bool_t object_is_object_js_default(tk_object_t* obj) {
  return_value_if_fail(obj != NULL, FALSE);
  return obj->vt == &s_object_js_default_vtable;
}

object_js_default_t* object_js_default_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_js_default_vtable, NULL);

  return (object_js_default_t*)(obj);
}
