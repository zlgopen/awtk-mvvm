/**
 * File:   view_model_jerryscript.c
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented view_model
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/assets_manager.h"
#include "mvvm/base/view_model.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/object_js_factory.h"
#include "mvvm/jerryscript/object_js_array.h"
#include "mvvm/jerryscript/view_model_jerryscript_object_wrapper.h"
#include "mvvm/jerryscript/view_model_jerryscript.h"

static view_model_t* view_model_jerryscript_create(const char* type, navigator_request_t* req) {
  view_model_t* view_model = NULL;
  jsvalue_t jsobj = jsvalue_get_view_model(type);

  if (jsvalue_is_undefined(jsobj)) {
    assets_manager_t* am = assets_manager();
    const asset_info_t* info;

    info = assets_manager_ref_ex(am, ASSET_TYPE_SCRIPT, ASSET_TYPE_SCRIPT_JS, type);
    if (info != NULL) {
      if (info->size > 0) {
        if (jerry_script_eval_buff((const char*)(info->data), info->size, type, TRUE) == RET_OK) {
          jsvalue_unref(jsobj);
          jsobj = jsvalue_get_view_model(type);
        }
      }
      assets_manager_unref(am, info);
    }
  }

  if (jsvalue_check(jsobj) == RET_OK) {
    if (jsvalue_is_object(jsobj)) {
      if (jsobj_has_prop(jsobj, "$wrappedObj")) {
        value_t v;
        str_t temp;
        str_init(&temp, 0);
        if (jsobj_get_prop(jsobj, "$wrappedObj", &v, &temp) == RET_OK) {
          if (v.type == VALUE_TYPE_STRING) {
            const char* vm_prefix = "view_model?";
            if (tk_str_start_with(temp.str, vm_prefix)) {
              view_model = view_model_factory_create_model(temp.str + strlen(vm_prefix), req);
            }
          }
        }
        str_reset(&temp);
      } else {
        view_model = view_model_jerryscript_object_wrapper_create(jsobj, req);
        if (view_model != NULL) {
          tk_object_set_name(TK_OBJECT(view_model), type);
        }
      }
    }
    jsvalue_unref(jsobj);
  }

  return view_model;
}

ret_t view_model_jerryscript_init(void) {
  object_js_factory_set(object_js_factory_create());
  object_js_factory_register(object_js_factory(), JERRY_OBJECT_TYPE_ARRAY, object_js_array_create);
  view_model_factory_register_generic(view_model_jerryscript_create);

  return RET_OK;
}

ret_t view_model_jerryscript_deinit(void) {
  object_js_factory_destroy(object_js_factory());
  object_js_factory_set(NULL);

  return RET_OK;
}
