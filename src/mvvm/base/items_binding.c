/**
 * File:   items_binding.c
 * Author: AWTK Develop Team
 * Brief:  list binding rule
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
#include "mvvm/base/items_binding.h"
#include "base/idle.h"

#define equal tk_str_ieq

static ret_t items_binding_on_destroy(tk_object_t* obj) {
  items_binding_t* rule = ITEMS_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (rule->rebind_idle_id != TK_INVALID_ID) {
    idle_remove(rule->rebind_idle_id);
    rule->rebind_idle_id = TK_INVALID_ID;
  }

  TKMEM_FREE(rule->items_name);
  TKMEM_FREE(rule->item_name);
  TKMEM_FREE(rule->index_name);
  TKMEM_FREE(rule->id_name);

  if (rule->props != NULL) {
    tk_object_unref(rule->props);
  }

  return RET_OK;
}

static ret_t items_binding_object_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  const char* value = value_str(v);
  items_binding_t* rule = ITEMS_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (rule->items_name == NULL && value == NULL) {
    value = name;
    rule->items_name = tk_str_copy(rule->items_name, value);
  } else if (equal(ITEMS_BINDING_ITEMS_NAME, name)) {
    rule->items_name = tk_str_copy(rule->items_name, value);
  } else if (equal(ITEMS_BINDING_ITEM_NAME, name)) {
    rule->item_name = tk_str_copy(rule->item_name, value);
  } else if (equal(ITEMS_BINDING_INDEX_NAME, name)) {
    rule->index_name = tk_str_copy(rule->index_name, value);
  } else if (equal(ITEMS_BINDING_ID_NAME, name)) {
    rule->id_name = tk_str_copy(rule->id_name, value);
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

static ret_t items_binding_object_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  items_binding_t* rule = items_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (equal(ITEMS_BINDING_ITEMS_NAME, name)) {
    value_set_str(v, rule->items_name);
  } else if (equal(ITEMS_BINDING_ITEM_NAME, name)) {
    value_set_str(v, rule->item_name);
  } else if (equal(ITEMS_BINDING_INDEX_NAME, name)) {
    value_set_str(v, rule->index_name);
  } else if (equal(ITEMS_BINDING_ID_NAME, name)) {
    value_set_str(v, rule->id_name);
  } else if (tk_str_eq(name, STR_PROP_SELF)) {
    value_set_pointer(v, BINDING_RULE_WIDGET(rule));
  } else {
    ret = tk_object_get_prop(rule->props, name, v);
  }

  return ret;
}

static bool_t items_binding_object_can_exec(tk_object_t* obj, const char* name, const char* args) {
  items_binding_t* rule = (items_binding_t*)(obj);
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

static ret_t items_binding_object_exec(tk_object_t* obj, const char* name, const char* args) {
  items_binding_t* rule = (items_binding_t*)(obj);
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

static const object_vtable_t s_items_binding_vtable = {
    .type = "items_binding",
    .desc = "items_binding",
    .size = sizeof(items_binding_t),
    .is_collection = FALSE,
    .can_exec = items_binding_object_can_exec,
    .exec = items_binding_object_exec,
    .on_destroy = items_binding_on_destroy,
    .get_prop = items_binding_object_get_prop,
    .set_prop = items_binding_object_set_prop,
};

items_binding_t* items_binding_create(void) {
  tk_object_t* obj = tk_object_create(&s_items_binding_vtable);
  items_binding_t* rule = ITEMS_BINDING(obj);
  return_value_if_fail(obj != NULL, NULL);

  rule->fixed_widget_count = -1;

  return rule;
}

items_binding_t* items_binding_cast(void* rule) {
  return_value_if_fail(binding_rule_is_items_binding(rule), NULL);

  return (items_binding_t*)rule;
}

bool_t binding_rule_is_items_binding(binding_rule_t* rule) {
  tk_object_t* obj = TK_OBJECT(rule);
  return obj != NULL && obj->vt == &s_items_binding_vtable;
}
