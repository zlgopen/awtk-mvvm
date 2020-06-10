/**
 * File:   application.c
 * Author: AWTK Develop Team
 * Brief:  application
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
#include "conf_io/app_conf.h"
#include "conf_io/app_conf_init_json.h"
#include "mvvm/base/view_model_object_wrapper.h"

view_model_t* app_conf_create_view_model(navigator_request_t* req) {
  object_t* obj = app_conf_get_instance();

  return view_model_object_wrapper_create(obj);
}

ret_t application_init() {
  app_conf_init_json("demo22");

  view_model_factory_register("app_conf", app_conf_create_view_model);

  return navigator_to("app_conf_sub_vm");
}

#include "../awtk_main.c"
