/**
 * File:   xml_mvvm_prop.h
 * Author: AWTK Develop Team
 * Brief:  xml parser for mvvm prop
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-05-22 GaoY <gaoyuan@zlg.cn> created
 *
 */

#include "tkc/types_def.h"
#include "mvvm_prop_gen.h"

#ifndef XML_MVVM_PROP_H
#define XML_MVVM_PROP_H

BEGIN_C_DECLS

/**
 * @method xml_mvvm_prop_file_to_array
 * 通过标准xml文件生成已用属性路径组。
 *
 * @param  {const char*} 标准xml文件。
 * @param  {uint32_t*} 已用属性路径组条数。
 *
 * @return {mvvm_prop_result_t**} 已用属性路径组对象。
 */
mvvm_prop_result_t** xml_mvvm_prop_file_to_array(const char* in_filename, uint32_t* result_size);

/**
 * @method xml_mvvm_prop_file_to_packfile
 * 通过标准xml文件打包生成已用属性路径组文件。
 *
 * @param  {const char*} 标准xml文件。
 * @param  {const char*} 已用属性路径组文件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t xml_mvvm_prop_file_to_packfile(const char* in_filename, const char* out_filename);

/**
 * @method xml_mvvm_prop_packfile_to_array
 * 通过已用属性路径组文件解包生成已用属性路径组。
 *
 * @param  {const char*} 已用属性路径组文件。
 * @param  {uint32_t*} 已用属性路径组条数。
 *
 * @return {mvvm_prop_result_t**} 已用属性路径组对象。
 */
mvvm_prop_result_t** xml_mvvm_prop_packfile_to_array(const char* in_filename, uint32_t* result_size);

/**
 * @method xml_mvvm_prop_xml_to_array
 * 通过原始xml字串生成已用属性路径组。
 *
 * @param  {const char*} 已用属性路径组文件。
 * @param  {uint32_t*} 已用属性路径组条数。
 *
 * @return {mvvm_prop_result_t**} 已用属性路径组对象。
 */
mvvm_prop_result_t** xml_mvvm_prop_xml_to_array(const char* xml, uint32_t* result_size);

END_C_DECLS

#endif /*XML_MVVM_PROP_H*/