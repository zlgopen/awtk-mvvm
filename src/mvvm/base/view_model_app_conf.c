/**
 * File:   view_model_app_conf.c
 * Author: AWTK Develop Team
 * Brief:  view_model_app_conf
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/func_call_parser.h"

#include "conf_io/app_conf.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/view_model.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_object_wrapper.h"

#define VIEW_MODEL_APP_CONF "app_conf"
#define VIEW_MODEL_APP_CONF_ADD "app_conf_add"
#define VIEW_MODEL_APP_CONF_EDIT "app_conf_edit"
#define VIEW_MODEL_APP_CONF_DETAIL "app_conf_detail"

static view_model_t* app_conf_create_view_model(navigator_request_t* req) {
  tk_object_t* args = NULL;
  tk_object_t* obj = app_conf_get_instance();
  const char* type_and_args = NULL;
  return_value_if_fail(req != NULL, NULL);
  return_value_if_fail(obj != NULL, NULL);
  type_and_args = tk_object_get_prop_str(req->args, NAVIGATOR_ARG_VIEW_MODEL_TYPE);
  args = func_call_parse(type_and_args, tk_strlen(type_and_args));
  return_value_if_fail(args != NULL, NULL);
  
  if (tk_str_eq(args->name, VIEW_MODEL_APP_CONF_ADD)) {
    const char* path = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_PREFIX);
    model_init_sub_object_with_args(obj, path, args);
  }
  TK_OBJECT_UNREF(args);

  return view_model_object_wrapper_create(obj);
}

ret_t view_model_app_conf_register(void) {
  view_model_factory_register(VIEW_MODEL_APP_CONF_ADD, app_conf_create_view_model);
  view_model_factory_register(VIEW_MODEL_APP_CONF_EDIT, app_conf_create_view_model);
  view_model_factory_register(VIEW_MODEL_APP_CONF_DETAIL, app_conf_create_view_model);
  return view_model_factory_register(VIEW_MODEL_APP_CONF, app_conf_create_view_model);
}
