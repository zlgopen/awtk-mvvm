/**
 * File:   command_binding.c
 * Author: AWTK Develop Team
 * Brief:  command binding
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "tkc/tokenizer.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/navigator.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/view_model_array.h"
#include "mvvm/base/command_binding.h"

#define equal tk_str_ieq

static ret_t command_binding_on_destroy(tk_object_t* obj) {
  command_binding_t* rule = COMMAND_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rule->args);
  TKMEM_FREE(rule->command);
  TKMEM_FREE(rule->event);
  TKMEM_FREE(rule->key_filter);

  if (rule->props != NULL) {
    tk_object_unref(rule->props);
  }

  str_reset(&(rule->temp));

  return RET_OK;
}

static ret_t command_binding_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  const char* value = v->type == VALUE_TYPE_STRING ? value_str(v) : NULL;
  command_binding_t* rule = COMMAND_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (BINDING_RULE(rule)->inited) {
    binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
    ret = binding_context_set_prop_by_rule(ctx, BINDING_RULE(rule), name, v);
    if (ret == RET_NOT_FOUND) {
      if (rule->props == NULL) {
        rule->props = object_default_create();
      }
      return tk_object_set_prop(rule->props, name, v);
    }
    return ret;
  }

  if (rule->command == NULL && value == NULL) {
    value = name;
    rule->command = tk_str_copy(rule->command, value);
  } else if (equal(COMMAND_BINDING_COMMAND, name)) {
    rule->command = tk_str_copy(rule->command, value);
  } else if (equal(COMMAND_BINDING_ARGS, name)) {
    uint32_t len = strlen(value);
    bool_t is_fscript = tk_str_eq(rule->command, COMMAND_BINDING_CMD_FSCRIPT);
    if (is_fscript && value[0] == '{' && value[len - 1] == '}') {
      if (rule->args != NULL) {
        TKMEM_FREE(rule->args);
      }
      rule->args = tk_strndup(value + 1, len - 2);
    } else {
      rule->args = tk_str_copy(rule->args, value);
    }
  } else if (equal(COMMAND_BINDING_EVENT, name)) {
    rule->event = tk_str_copy(rule->event, value);
  } else if (equal(COMMAND_BINDING_KEY_FILTER, name)) {
    rule->key_filter = tk_str_copy(rule->key_filter, value);
    if (value != NULL) {
      shortcut_init_with_str(&(rule->filter), value);
    }
  } else if (equal(COMMAND_BINDING_CLOSE_WINDOW, name)) {
    rule->close_window = value != NULL ? tk_atob(value) : TRUE;
  } else if (equal(COMMAND_BINDING_AUTO_DISABLE, name)) {
    rule->auto_disable = value != NULL ? tk_atob(value) : TRUE;
  } else if (equal(COMMAND_BINDING_QUIT_APP, name)) {
    rule->quit_app = value != NULL ? tk_atob(value) : TRUE;
  } else if (equal(COMMAND_BINDING_UPDATE_VIEW_MODEL, name)) {
    rule->update_model = value != NULL ? tk_atob(value) : TRUE;
  } else if (equal(BINDING_RULE_PROP_INITED, name)) {
    BINDING_RULE(rule)->inited = TRUE;
  } else {
    if (rule->props == NULL) {
      rule->props = object_default_create();
    }
    ret = tk_object_set_prop(rule->props, name, v);
  }

  return ret;
}

static ret_t command_binding_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  command_binding_t* rule = command_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (BINDING_RULE(rule)->inited) {
    if (tk_str_eq(name, STR_PROP_SELF)) {
      value_set_pointer(v, BINDING_RULE_WIDGET(rule));
      return RET_OK;
    } else {
      binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
      ret = binding_context_get_prop_by_rule(ctx, BINDING_RULE(rule), name, v);
      if (ret == RET_NOT_FOUND && rule->props != NULL) {
        return tk_object_get_prop(rule->props, name, v);
      }
      return ret;
    }
  }

  if (equal(COMMAND_BINDING_COMMAND, name)) {
    value_set_str(v, rule->command);
  } else if (equal(COMMAND_BINDING_ARGS, name)) {
    value_set_str(v, rule->args);
  } else if (equal(COMMAND_BINDING_EVENT, name)) {
    value_set_str(v, rule->event);
  } else if (equal(COMMAND_BINDING_CLOSE_WINDOW, name)) {
    value_set_bool(v, rule->close_window);
  } else if (equal(COMMAND_BINDING_QUIT_APP, name)) {
    value_set_bool(v, rule->quit_app);
  } else if (equal(COMMAND_BINDING_UPDATE_VIEW_MODEL, name)) {
    value_set_bool(v, rule->update_model);
  } else {
    if (rule->props != NULL) {
      ret = tk_object_get_prop(rule->props, name, v);
    }
  }

  return ret;
}

static const char* command_binding_resolve_arguments(tk_object_t* obj, const char* args,
                                                     str_t* temp) {
  const char* ret = args;

  if (args != NULL && tk_str_start_with(args, COMMAND_ARGS_FSCRIPT_PREFIX)) {
    tk_object_t* objargs = object_default_create();
    return_value_if_fail(objargs != NULL, args);

    str_set(temp, COMMAND_ARGS_STRING_PREFIX);

    if (tk_command_arguments_to_object(args, objargs) == RET_OK &&
        tk_command_arguments_fscript(objargs, obj) == RET_OK &&
        tk_command_arguments_from_object(objargs, temp) == RET_OK) {
      ret = temp->str;
    }

    tk_object_unref(objargs);
  }

  return ret;
}

static ret_t command_binding_object_exec(tk_object_t* obj, const char* name, const char* args) {
  command_binding_t* rule = (command_binding_t*)(obj);
  binding_context_t* context = BINDING_RULE_CONTEXT(rule);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  args = command_binding_resolve_arguments(obj, args, &(rule->temp));

  if (binding_context_exec(context, name, args) == RET_OK) {
    return RET_OK;
  }

  if (tk_object_is_collection(TK_OBJECT(view_model))) {
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
    view_model_array_set_cursor(view_model, cursor);
  }

  return view_model_exec(view_model, name, args);
}

static const object_vtable_t s_command_binding_vtable = {.type = "command_binding",
                                                         .desc = "command_binding",
                                                         .size = sizeof(command_binding_t),
                                                         .is_collection = FALSE,
                                                         .on_destroy = command_binding_on_destroy,
                                                         .exec = command_binding_object_exec,
                                                         .get_prop = command_binding_get_prop,
                                                         .set_prop = command_binding_set_prop};

command_binding_t* command_binding_create(void) {
  command_binding_t* rule = (command_binding_t*)tk_object_create(&s_command_binding_vtable);
  return_value_if_fail(rule != NULL, NULL);

  rule->auto_disable = TRUE;
  str_init(&(rule->temp), 0);

  return rule;
}

command_binding_t* command_binding_cast(void* rule) {
  return_value_if_fail(binding_rule_is_command_binding(rule), NULL);

  return (command_binding_t*)rule;
}

bool_t binding_rule_is_command_binding(binding_rule_t* rule) {
  tk_object_t* obj = TK_OBJECT(rule);
  return obj != NULL && obj->vt == &s_command_binding_vtable;
}

bool_t command_binding_can_exec(command_binding_t* rule) {
  view_model_t* view_model = NULL;
  binding_context_t* context = NULL;
  const char* args = NULL;
  return_value_if_fail(rule != NULL, FALSE);

  view_model = BINDING_RULE_VIEW_MODEL(rule);
  context = BINDING_RULE_CONTEXT(rule);
  args = rule->args;
  return_value_if_fail(view_model != NULL, FALSE);

  args = command_binding_resolve_arguments(TK_OBJECT(rule), args, &(rule->temp));

  if (binding_context_can_exec(context, rule->command, args)) {
    return TRUE;
  }

  if (tk_object_is_collection(TK_OBJECT(view_model))) {
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
    view_model_array_set_cursor(view_model, cursor);
  }

  return view_model_can_exec(view_model, rule->command, args);
}

ret_t command_binding_exec(command_binding_t* rule) {
  view_model_t* view_model = NULL;
  binding_context_t* context = NULL;
  const char* args = NULL;
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  view_model = BINDING_RULE_VIEW_MODEL(rule);
  context = BINDING_RULE_CONTEXT(rule);
  args = rule->args;
  return_value_if_fail(view_model != NULL && context != NULL, RET_BAD_PARAMS);

  args = command_binding_resolve_arguments(TK_OBJECT(rule), args, &(rule->temp));

  if (tk_str_eq(rule->command, COMMAND_BINDING_CMD_FSCRIPT)) {
    value_t v;
    fscript_eval(TK_OBJECT(rule), rule->args, &v);
    value_reset(&v);
    return RET_OK;
  }

  if (binding_context_exec(context, rule->command, args) == RET_OK) {
    return RET_OK;
  }

  if (tk_object_is_collection(TK_OBJECT(view_model))) {
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
    view_model_array_set_cursor(view_model, cursor);
  }

  return view_model_exec(view_model, rule->command, args);
}
