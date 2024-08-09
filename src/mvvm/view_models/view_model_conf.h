/**
 * File:   view_model_conf.h
 * Author: AWTK Develop Team
 * Brief:  view_model_conf
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_VIEW_MODEL_CONF_H
#define TK_VIEW_MODEL_CONF_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_conf_t;
typedef struct _model_conf_t view_model_conf_t;

/**
 * @class view_model_conf_t
 * @parent view_model_t
 *
 * 提供XML/JSON/UBJSON/INI等配置信息的访问。
 *
 */
struct _model_conf_t {
  view_model_t view_model;

  /**
   * @property {str_t} url
   * @annotation ["readable"]
   * 文件名。
   */
  str_t url;

  /**
   * @property {bool_t} is_dirty
   * @annotation ["readable"]
   * 文件内容是否已经被修改。
   */
  bool_t is_dirty;

  /**
   * @property {char*} prefix
   * @annotation ["readable"]
   * 读取的配置信息的前缀。
   */
  char* prefix;

  /**
   * @property {char*} type
   * @annotation ["readable"]
   * 配置信息的类型。
   */
  char* type;

  /**
   * @property {bool_t} create_if_not_exist
   * @annotation ["readable"]
   * 如果文件不存在，是否创建。
   */
  bool_t create_if_not_exist;

  /*private*/
  tk_object_t* obj;
};

/**
 * @method view_model_conf_create
 * 创建conf模型对象。
 *
 * 读取文件，访问文件信息(可用于读写设备文件)。
 * @param {navigator_request_t*} req 请求参数。
 *
 * ```xml
 * v-model="json(url='${app_dir}/confs/test.json')"
 * v-model="json(url='https://www.example.com/confs/test.json')"
 * v-model="json_array(url='${app_dir}/confs/test.json', prefix='network')"
 * v-model="json_array(url='https://www.example.com/confs/test.json', prefix='network')"
 * ```
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_conf_create(navigator_request_t* req);

#define VIEW_MODEL_CONF(view_model) ((view_model_conf_t*)(view_model))

#define STR_VIEW_MODEL_CONF_JSON "json"
#define STR_VIEW_MODEL_CONF_JSON_ARRAY "json_array"

#define STR_VIEW_MODEL_CONF_UBJSON "ubjson"
#define STR_VIEW_MODEL_CONF_UBJSON_ARRAY "ubson_array"

#define STR_VIEW_MODEL_CONF_XML "xml"
#define STR_VIEW_MODEL_CONF_XML_ARRAY "xml_array"

#define STR_VIEW_MODEL_CONF_INI "ini"

#define STR_VIEW_MODEL_CONF_CSV "csv"

#define STR_VIEW_MODEL_CONF_ADD "conf_add"
#define STR_VIEW_MODEL_CONF_EDIT "conf_edit"
#define STR_VIEW_MODEL_CONF_DETAIL "conf_detail"

END_C_DECLS

#endif /*TK_VIEW_MODEL_CONF_H*/
