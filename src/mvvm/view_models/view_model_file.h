/**
 * File:   view_model_file.h
 * Author: AWTK Develop Team
 * Brief:  view_model_file
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-12-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_FILE_H
#define TK_VIEW_MODEL_FILE_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

struct _model_file_t;
typedef struct _model_file_t view_model_file_t;

/**
 * @class view_model_file_t
 * @parent view_model_t
 *
 * 提供读写文件，访问文件信息(可用于读写设备文件)，浏览(选择)文件等功能。
 *
 */
struct _model_file_t {
  view_model_t view_model;

  /**
   * @property {str_t} filename
   * @annotation ["readable"]
   * 文件名。
   */
  str_t filename;

  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 文件大小。
   */
  uint32_t size;

  /**
   * @property {char*} content;
   * @annotation ["readable"]
   * 文件内容。
   */
  char* content;

  /**
   * @property {bool_t} auto_load
   * @annotation ["readable"]
   * 是否自动加载文件内容。
   */
  bool_t auto_load;

  /**
   * @property {bool_t} is_dirty
   * @annotation ["readable"]
   * 文件内容是否已经被修改。
   */
  bool_t is_dirty;
};

/**
 * @method view_model_file_create
 * 创建file模型对象。
 *
 * 读取文件，访问文件信息(可用于读写设备文件)。
 * @param {navigator_request_t*} req 请求参数。
 *
 * ```xml
 * v-model='file(path="/data/data/com.example.app/files/test.txt" auto_load=true)'
 * v-model='file(path="${temp_dir}/files/test.txt" auto_load=true)'
 * v-model='file(path="${app_dir}/files/test.txt" auto_load=true)'
 * v-model='file(path="${user_dir}/files/test.txt" auto_load=true)'
 * ```
 * @return {view_model_t*} 返回view_model对象。
 */
view_model_t* view_model_file_create(navigator_request_t* req);

#define VIEW_MODEL_FILE(view_model) ((view_model_file_t*)(view_model))

#define STR_VIEW_MODEL_FILE "file"

#define STR_VIEW_MODEL_FILE_PROP_SIZE "size"
#define STR_VIEW_MODEL_FILE_PROP_CONTENT "content"
#define STR_VIEW_MODEL_FILE_PROP_FILENAME "filename"
#define STR_VIEW_MODEL_FILE_PROP_AUTO_LOAD "auto_load"
#define STR_VIEW_MODEL_FILE_PROP_IS_DIRTY "is_dirty"

#define STR_VIEW_MODEL_FILE_CMD_BROWSE "browse"

END_C_DECLS

#endif /*TK_VIEW_MODEL_FILE_H*/
