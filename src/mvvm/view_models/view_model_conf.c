/**
 * File:   view_model_conf.c
 * Author: AWTK Develop Team
 * Brief:  view_model_conf
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License conf for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-12-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/func_call_parser.h"
#include "tkc/data_reader.h"
#include "tkc/data_reader_factory.h"
#include "csv/csv_file_object.h"

#include "conf_io/conf_json.h"
#include "conf_io/conf_xml.h"
#include "conf_io/conf_ubjson.h"
#include "conf_io/conf_ini.h"

#include "mvvm/base/utils.h"
#include "mvvm/base/navigator.h"
#include "mvvm/view_models/view_model_conf.h"
#include "mvvm/base/view_model_object_wrapper.h"
#include "mvvm/base/view_model_array_object_wrapper.h"

view_model_t* view_model_conf_create(navigator_request_t* req) {
  const char* type = NULL;
  tk_object_t* args = NULL;
  tk_object_t* obj = NULL;
  const char* url = NULL;
  bool_t is_array = FALSE;
  bool_t use_req_args = FALSE;
  view_model_t* vm = NULL;
  const char* prefix = NULL;
  const char* name = NULL;
  char surl[MAX_PATH + 1] = {0};
  const char* type_and_args = NULL;
  bool_t create_if_not_exist = FALSE;
  return_value_if_fail(req != NULL, NULL);
  type_and_args = tk_object_get_prop_str(req->args, NAVIGATOR_ARG_VIEW_MODEL_TYPE);
  return_value_if_fail(type_and_args != NULL, NULL);
  obj = tk_object_get_prop_object(req->args, NAVIGATOR_ARG_MODEL);

  args = func_call_parse(type_and_args, tk_strlen(type_and_args));
  return_value_if_fail(args != NULL, NULL);

  type = args->name;
  return_value_if_fail(type != NULL, NULL);

  if (req->args != NULL && tk_object_get_prop_str(req->args, "url") != NULL) {
    /*优先使用请求参数*/
    use_req_args = TRUE;
    url = tk_object_get_prop_str(req->args, "url");
  } else {
    url = tk_object_get_prop_str(args, "url");
  }

  name = tk_object_get_prop_str(args, "name");
  prefix = tk_object_get_prop_str(args, "prefix");
  create_if_not_exist = tk_object_get_prop_bool(args, "create_if_not_exist", FALSE);

  if (obj == NULL) { 
    return_value_if_fail(url != NULL, NULL);
  } 

  if (url != NULL) {
    path_expand_vars(url, surl, MAX_PATH);
    url = surl;
  }

  if (obj != NULL && url == NULL) {
    if (tk_str_eq(type, STR_VIEW_MODEL_CONF_ADD)) {
      /*init*/
      const char* path = tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_PREFIX);
      model_init_sub_object_with_args(obj, path, args);
    }
    log_debug("reuse model: %s %s\n", obj->name, obj->vt->type);
    TK_OBJECT_REF(obj);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_JSON)) {
    obj = conf_json_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_XML)) {
    obj = conf_xml_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_UBJSON)) {
    obj = conf_ubjson_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_JSON_ARRAY)) {
    is_array = TRUE;
    obj = conf_json_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_XML_ARRAY)) {
    is_array = TRUE;
    obj = conf_xml_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_UBJSON_ARRAY)) {
    is_array = TRUE;
    obj = conf_ubjson_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_INI)) {
    obj = conf_ini_load(url, create_if_not_exist);
  } else if (tk_str_eq(type, STR_VIEW_MODEL_CONF_CSV)) {
    const char* sep = tk_object_get_prop_str(use_req_args ? req->args : args, "sep");
    const char* col_names = tk_object_get_prop_str(use_req_args ? req->args : args, "col_names");
    csv_file_t* csv = csv_file_create(url, sep != NULL ? sep[0] : ',');
    obj = csv_file_object_create(csv);
    if (col_names != NULL) {
      tk_object_set_prop_str(obj, CSV_PROP_COL_NAMES, col_names);
    }
    is_array = TRUE;
  } else {
    log_debug("unknown type:%s\n", type);
  }
  return_value_if_fail(obj != NULL, NULL);

  if (obj->name == NULL && name != NULL) {
    tk_object_set_name(obj, name);
    log_debug("set object name as %s\n", obj->name);
  }

  if (is_array) {
    vm = view_model_array_object_wrapper_create_ex(obj, prefix);
  } else {
    vm = view_model_object_wrapper_create_ex(obj, prefix);
  }

  TK_OBJECT_UNREF(args);

  return vm;
}

