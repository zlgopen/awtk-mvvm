/**
 * File:   view_model_jerryscript.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented view_model
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_JERRYSCRIPT_H
#define TK_VIEW_MODEL_JERRYSCRIPT_H

#include "tkc/str.h"
#include "jerryscript.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @method view_model_jerryscript_create
 * 通过一段JS代码创建一个view_model对象。
 *
 * @param {const char*} name 名称(通常是文件名)。
 * @param {const char*} code 代码字符字符串(UTF8)。
 * @param {uint32_t} code_size 代码的长度。
 * @param {navigator_request_t*} req 请求的参数(可选)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
view_model_t* view_model_jerryscript_create(const char* name, const char* code, uint32_t code_size,
                                            navigator_request_t* req);

END_C_DECLS

#endif /*TK_VIEW_MODEL_JERRYSCRIPT_H*/
