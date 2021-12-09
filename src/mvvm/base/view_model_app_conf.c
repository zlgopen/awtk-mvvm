/**
 * File:   view_model_app_conf.c
 * Author: AWTK Develop Team
 * Brief:  view_model_app_conf
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
 * 2020-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "conf_io/app_conf.h"
#include "mvvm/base/view_model.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_object_wrapper.h"

static view_model_t* app_conf_create_view_model(navigator_request_t* req) {
  tk_object_t* obj = app_conf_get_instance();
  return_value_if_fail(obj != NULL, NULL);

  return view_model_object_wrapper_create(obj);
}

ret_t view_model_app_conf_register(void) {
  return view_model_factory_register("app_conf", app_conf_create_view_model);
}
