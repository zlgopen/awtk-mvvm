/**
 * File:   command_binding.c
 * Author: AWTK Develop Team
 * Brief:  command binding rule
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
 * 2019-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMMAND_BINDING_H
#define TK_COMMAND_BINDING_H

#include "base/shortcut.h"
#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

/**
 * @class command_binding_t
 * @parent binding_rule_t
 * 命令绑定规则。
 *
 */
typedef struct _command_binding_t {
  binding_rule_t binding_rule;
  tk_object_t* props;

  /**
   * @property {bool_t} close_window
   * @annotation ["readable"]
   * 执行命令之后，是否关闭当前窗口。
   */
  bool_t close_window;

  /**
   * @property {bool_t} quit_app
   * @annotation ["readable"]
   * 执行命令之后，是否退出应用程序。
   */
  bool_t quit_app;

  /**
   * @property {bool_t} update_model
   * @annotation ["readable"]
   * 执行命令之前，是否更新数据到模型。
   */
  bool_t update_model;

  /**
   * @property {char*} command
   * @annotation ["readable"]
   * 命令名称。
   */
  char* command;

  /**
   * @property {char*} args
   * @annotation ["readable"]
   * 命令参数。
   */
  char* args;

  /**
   * @property {char*} event
   * @annotation ["readable"]
   * 事件名称。
   */
  char* event;

  /**
   * @property {char*} key_filter
   * @annotation ["readable"]
   * 按键过滤(主要用于按键事件，相当于快捷键)。
   */
  char* key_filter;

  /**
   * @property {bool_t} auto_disable
   * @annotation ["readable"]
   * 是否根据can_exec自动禁用控件(缺省为TRUE)。
   */
  bool_t auto_disable;

  /*private*/
  str_t temp;
  shortcut_t filter;
} command_binding_t;

/**
 * @method command_binding_create
 * 创建数据绑定对象。
 * @annotation ["constructor"]
 *
 * @return {binding_rule_t*} 返回数据绑定对象。
 */
command_binding_t* command_binding_create(void);

/**
 * @method command_binding_can_exec
 * 检查当前的命令是否可以执行。
 *
 * @param {command_binding_t*} rule 绑定规则对象。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t command_binding_can_exec(command_binding_t* rule);

/**
 * @method command_binding_exec
 * 执行当前的命令。
 *
 * @param {command_binding_t*} rule 绑定规则对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t command_binding_exec(command_binding_t* rule);

/**
 * @method binding_rule_is_command_binding
 * 判断当前规则是否为命令绑定规则。
 *
 * @param {binding_rule_t*} rule 绑定规则对象。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t binding_rule_is_command_binding(binding_rule_t* rule);

/**
 * @method command_binding_cast
 * 转换为command_binding对象。
 * @annotation ["cast"]
 * @param {void*} rule 绑定规则对象。
 *
 * @return {data_binding_t*} 返回绑定规则对象。
 */
command_binding_t* command_binding_cast(void* rule);

#define COMMAND_BINDING(rule) command_binding_cast((void*)rule)

#define COMMAND_BINDING_CMD_DEBUG "debug"
#define COMMAND_BINDING_CMD_FSCRIPT "fscript"
#define COMMAND_BINDING_CMD_NOTHING "nothing"
#define COMMAND_BINDING_CMD_NAVIGATE "navigate"
#define COMMAND_BINDING_CMD_SEND_KEY "SendKey"
#define COMMAND_BINDING_CMD_SET_WIDGET_PROP "SetWidgetProp"

#define COMMAND_BINDING_ARGS "Args"

#define COMMAND_BINDING_COMMAND "Command"
#define COMMAND_BINDING_QUIT_APP "QuitApp"
#define COMMAND_BINDING_AUTO_DISABLE "AutoDisable"
#define COMMAND_BINDING_CLOSE_WINDOW "CloseWindow"
#define COMMAND_BINDING_UPDATE_VIEW_MODEL "UpdateModel"

#define COMMAND_BINDING_EVENT "Event"
#define COMMAND_BINDING_KEY_FILTER "KeyFilter"

END_C_DECLS

#endif /*TK_COMMAND_BINDING_H*/
