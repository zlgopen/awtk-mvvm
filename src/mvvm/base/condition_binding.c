/**
 * File:   condition_binding.c
 * Author: AWTK Develop Team
 * Brief:  condition binding rule
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/view_model_array.h"
#include "mvvm/base/condition_binding.h"

#define equal tk_str_ieq

static ret_t condition_binding_on_destroy(tk_object_t* obj) {
  condition_binding_t* rule = condition_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (rule->props != NULL) {
    tk_object_unref(rule->props);
  }

  return RET_OK;
}

static ret_t condition_binding_object_set_prop(tk_object_t* obj, const char* name,
                                               const value_t* v) {
  ret_t ret = RET_OK;
  const char* value = value_str(v);
  condition_binding_t* rule = condition_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (value == NULL) {
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

static ret_t condition_binding_object_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  condition_binding_t* rule = condition_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (BINDING_RULE(rule)->inited) {
    if (tk_str_eq(name, STR_PROP_SELF)) {
      value_set_pointer(v, BINDING_RULE_WIDGET(rule));
      return RET_OK;
    } else {
      binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
      return binding_context_get_prop_by_rule(ctx, BINDING_RULE(rule), name, v);
    }
  }

  if (tk_str_eq(name, STR_PROP_SELF)) {
    value_set_pointer(v, BINDING_RULE_WIDGET(rule));
  } else {
    ret = tk_object_get_prop(rule->props, name, v);
  }

  return ret;
}

static bool_t condition_binding_object_can_exec(tk_object_t* obj, const char* name,
                                                const char* args) {
  condition_binding_t* rule = (condition_binding_t*)(obj);
  binding_context_t* context = BINDING_RULE_CONTEXT(rule);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);

  return_value_if_fail(obj != NULL && name != NULL, FALSE);
  if (binding_context_can_exec(context, name, args)) {
    return TRUE;
  }

  if (tk_object_is_collection(TK_OBJECT(view_model))) {
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
    view_model_array_set_cursor(view_model, cursor);
  }

  return view_model_can_exec(view_model, name, args);
}

static ret_t condition_binding_object_exec(tk_object_t* obj, const char* name, const char* args) {
  condition_binding_t* rule = (condition_binding_t*)(obj);
  binding_context_t* context = BINDING_RULE_CONTEXT(rule);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);

  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);
  if (binding_context_exec(context, name, args) == RET_OK) {
    return RET_OK;
  }

  if (tk_object_is_collection(TK_OBJECT(view_model))) {
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
    view_model_array_set_cursor(view_model, cursor);
  }

  return view_model_exec(view_model, name, args);
}

static const object_vtable_t s_condition_binding_vtable = {
    .type = "condition_binding",
    .desc = "condition_binding",
    .size = sizeof(condition_binding_t),
    .is_collection = FALSE,
    .can_exec = condition_binding_object_can_exec,
    .exec = condition_binding_object_exec,
    .on_destroy = condition_binding_on_destroy,
    .get_prop = condition_binding_object_get_prop,
    .set_prop = condition_binding_object_set_prop,
};

condition_binding_t* condition_binding_create(void) {
  tk_object_t* obj = tk_object_create(&s_condition_binding_vtable);
  condition_binding_t* rule = CONDITION_BINDING(obj);
  return_value_if_fail(obj != NULL, NULL);

  return rule;
}

condition_binding_t* condition_binding_cast(void* rule) {
  return_value_if_fail(binding_rule_is_condition_binding(rule), NULL);

  return (condition_binding_t*)rule;
}

bool_t binding_rule_is_condition_binding(binding_rule_t* rule) {
  tk_object_t* obj = TK_OBJECT(rule);
  return obj != NULL && obj->vt == &s_condition_binding_vtable;
}
