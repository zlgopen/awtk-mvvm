/**
 * File:   mvvm_prop_gen.h
 * Author: AWTK Develop Team
 * Brief:  proto mvvm prop string
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
#include "conf_io/conf_json.h"

#ifndef MVVM_PROP_GEN_H
#define MVVM_PROP_GEN_H

#define PROP_MAX_LEN 127
#define MVVM_PROP_RESULT_INIT_SIZE 5

#define json_PROTO_SIZE "size"
#define JSON_PROTO_PATH "path"
#define JSON_PROTO_IS_ARRAY "is_array"

BEGIN_C_DECLS

struct _path_item_t;
typedef struct _path_item_t path_item_t;

struct _mvvm_prop_gen_t;
typedef struct _mvvm_prop_gen_t mvvm_prop_gen_t;

/**
 * @class path_item_t
 *
 * 已用属性路径项。
 */
struct _path_item_t {
  /**
   * @property {char*} path
   * @annotation ["readable"]
   * 使用属性路径。
   */
  char* path;
  /**
   * @property {bool_t} is_array
   * @annotation ["readable"]
   * 是否是数组形式。
   */
  bool_t is_array;
  /**
   * @property {uint32_t} path_len
   * @annotation ["readable"]
   * 使用属性路径字串长度。
   */
  uint32_t path_len;
  /**
   * @property {path_item_t*} next
   * @annotation ["readable"]
   * 下一个路径项。
   */
  path_item_t* next;
};

/**
 * @class mvvm_prop_result_t
 *
 * 已用属性路径组。
 */
typedef struct _mvvm_prop_result_t {
  /**
   * @property {char*} prop
   * @annotation ["readable"]
   * 已用属性名称。
   */
  char* prop;
  /**
   * @property {uint32_t} prop_len
   * @annotation ["readable"]
   * 已用属性名称字串长度。
   */
  uint32_t prop_len;
  /**
   * @property {uint32_t} group_size
   * @annotation ["readable"]
   * 属性可能存在的路径条数。
   */
  uint32_t group_size;
  /**
   * @property {path_item_t*} group_head
   * @annotation ["readable"]
   * 可能存在的路径链表头指针。
   */
  path_item_t* group_head;

  /* private */
  path_item_t* group_tail;
} mvvm_prop_result_t;

/**
 * @method mvvm_prop_gen_create
 * @annotation ["constructor"]
 * 构造mvvm_prop_gen_t对象。
 *
 * @return {mvvm_prop_gen_t*} mvvm_prop_gen_t对象。
 */
mvvm_prop_gen_t* mvvm_prop_gen_create(void);

/**
 * @method mvvm_prop_gen_destory
 * @annotation ["destructor"]
 * 析构mvvm_prop_gen_t对象。
 *
 * @param {mvvm_prop_gen_t*} mvvm_prop_gen_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_prop_gen_destory(mvvm_prop_gen_t* mvvm_prop_gen);

/**
 * @method mvvm_prop_gen_exec_start
 * 执行器执行开始事件(scope_start)。
 *
 * @param {mvvm_prop_gen_t*} mvvm_prop_gen_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_prop_gen_exec_start(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag);

/**
 * @method mvvm_prop_gen_exec_end
 * 执行器执行完成事件(scope_end)。
 *
 * @param {mvvm_prop_gen_t*} mvvm_prop_gen_t对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_prop_gen_exec_end(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag);

/**
 * @method mvvm_prop_gen_try_gen_exec_by_keyword
 * 通过关键字尝试生成对应函数到执行器堆栈。如果关键字和属性值同时为NULL，则调用堆栈全部函数。
 *
 * @param {mvvm_prop_gen_t*} mvvm_prop_gen_t对象。
 * @param {const char*} 关键字。
 * @param {const char*} 属性值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_prop_gen_try_gen_exec_by_keyword(mvvm_prop_gen_t* mvvm_prop_gen, const char* keyword,
                                            const char* prop);

/**
 * @method mvvm_prop_gen_result_create
 * 生成已用属性路径组对象，从解析器中将结果转移出来，需要使用 mvvm_prop_gen_result_destory 析构。
 *
 * @param {mvvm_prop_gen_t*} mvvm_prop_gen_t对象。
 * @param {uint32_t*} 已用属性路径组对象条数。
 * @return {mvvm_prop_result_t**} 返回已用属性路径组对象。
 */
mvvm_prop_result_t** mvvm_prop_gen_result_create(mvvm_prop_gen_t* mvvm_prop_gen,
                                                 uint32_t* result_size);

/**
 * @method mvvm_prop_gen_result_destory
 * 析构生成已用属性路径组对象。
 *
 * @param {mvvm_prop_result_t**} mvvm_prop_result_t对象。
 * @param {uint32_t} 已用属性路径组对象条数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_prop_gen_result_destory(mvvm_prop_result_t** results, uint32_t result_size);

/**
 * @method mvvm_prop_gen_proto_result
 * 将json字符串反序列化为已用属性路径组对象。
 *
 * @param {str_t*} 序列化后的已用属性路径组对象json字符串。
 * @param {uint32_t*} 已用属性路径组对象条数。
 * @return {mvvm_prop_result_t**} 返回已用属性路径组对象。
 */
mvvm_prop_result_t** mvvm_prop_gen_proto_result(str_t* json, uint32_t* result_size);

/**
 * @method mvvm_prop_gen_save_result
 * 将已用属性路径组对象转化为json字符串。
 *
 * @param {mvvm_prop_result_t**} mvvm_prop_result_t对象。
 * @param {uint32_t} 已用属性路径组对象条数。
 * @param {str_t*} 用于存放解析结果的json字符串。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t mvvm_prop_gen_save_result(mvvm_prop_result_t** results, uint32_t result_size, str_t* json);

END_C_DECLS

#endif /*MVVM_PROP_GEN_H*/