/**
 * File:   binding_context_awtk.c
 * Author: AWTK Develop Team
 * Brief:  binding context awtk
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
 * 2019-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/int_str.h"
#include "tkc/darray.h"
#include "tkc/tokenizer.h"
#include "awtk_global.h"
#include "base/idle.h"
#include "base/enums.h"
#include "base/widget.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/items_binding.h"
#include "mvvm/base/condition_binding.h"
#include "mvvm/base/view_model_dummy.h"
#include "mvvm/base/view_model_array.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_compositor.h"
#include "mvvm/base/custom_binder.h"
#include "mvvm/awtk/ui_loader_mvvm.h"
#include "mvvm/awtk/binding_context_awtk.h"

#define STR_SUB_VIEW_MODEL "sub_view_model:"
#define STR_SUB_VIEW_MODEL_ARRAY "sub_view_model_array:"
#define IS_COMPOSITOR_VIEW_MODEL(type) strchr(type, '+') != NULL

static view_model_t* binding_context_awtk_create_one_view_model(view_model_t* parent,
                                                                const char* type,
                                                                navigator_request_t* req) {
  view_model_t* view_model = NULL;

  if (tk_str_start_with(type, STR_SUB_VIEW_MODEL)) {
    const char* name = strchr(type, ':');
    return_value_if_fail(name != NULL, NULL);
    view_model = view_model_create_sub_view_model(parent, name + 1);
  } else if (tk_str_start_with(type, STR_SUB_VIEW_MODEL_ARRAY)) {
    const char* name = strchr(type, ':');
    return_value_if_fail(name != NULL, NULL);
    view_model = view_model_create_sub_view_model_array(parent, name + 1);
  } else {
    view_model = view_model_factory_create_model(type, req);
  }

  return view_model;
}

static view_model_t* binding_context_awtk_create_view_model(view_model_t* parent, const char* type,
                                                            navigator_request_t* req) {
  view_model_t* view_model = NULL;

  if (type != NULL) {
    if (IS_COMPOSITOR_VIEW_MODEL(type)) {
      tokenizer_t t;
      view_model_t* compositor = view_model_compositor_create(req);
      return_value_if_fail(compositor != NULL, NULL);

      tokenizer_init(&t, type, strlen(type), "+");
      while (tokenizer_has_more(&t)) {
        const char* type1 = tokenizer_next(&t);
        view_model_t* vm = binding_context_awtk_create_one_view_model(parent, type1, req);
        if (vm != NULL) {
          if (view_model_compositor_add(compositor, vm) != RET_OK) {
            log_warn("view_model_compositor_add failed\n");
            TK_OBJECT_UNREF(vm);
          }
        } else {
          log_warn("create \"%s\" view_model failed\n", type1);
        }
      }
      tokenizer_deinit(&t);
      view_model = compositor;
    } else {
      view_model = binding_context_awtk_create_one_view_model(parent, type, req);
    }
  }

  if (view_model == NULL) {
    if (type != NULL) {
      log_warn("%s not found view_model \"%s\"\n", __FUNCTION__, type);
    }
    view_model = view_model_dummy_create(req);
  }

  return view_model;
}

static ret_t visit_data_binding_update_error_of(void* ctx, const void* data) {
  data_binding_t* rule = DATA_BINDING((void*)data);
  data_binding_t* trigger_rule = DATA_BINDING(ctx);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(trigger_rule);

  if (tk_str_start_with(rule->path, DATA_BINDING_ERROR_OF)) {
    const char* path = rule->path + sizeof(DATA_BINDING_ERROR_OF) - 1;
    if (tk_str_eq(path, trigger_rule->path)) {
      widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));
      widget_set_tr_text(widget, view_model->last_error.str);
    }
  }

  return RET_OK;
}

static ret_t widget_visit_data_binding_update_error_of(void* ctx, const void* data) {
  darray_t* node = (darray_t*)data;

  return darray_foreach(node, visit_data_binding_update_error_of, ctx);
}

static ret_t binding_context_update_error_of(data_binding_t* rule) {
  binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(ctx != NULL && view_model != NULL, RET_BAD_PARAMS);

  if (view_model->last_error.size > 0) {
    slist_foreach(&(ctx->data_bindings), widget_visit_data_binding_update_error_of, rule);
  }

  return RET_OK;
}

static ret_t on_widget_prop_change(void* ctx, event_t* e) {
  data_binding_t* rule = DATA_BINDING(ctx);
  prop_change_event_t* evt = prop_change_event_cast(e);

  if (tk_str_eq(evt->name, rule->prop)) {
    data_binding_set_prop(rule, evt->value);
    binding_context_update_error_of(rule);
  }

  return RET_OK;
}

static ret_t on_widget_value_change(void* ctx, event_t* e) {
  value_t v;
  widget_t* widget = WIDGET(e->target);
  data_binding_t* rule = DATA_BINDING(ctx);
  return_value_if_fail(widget_get_prop(widget, rule->prop, &v) == RET_OK, RET_OK);

  data_binding_set_prop(rule, &v);
  binding_context_update_error_of(rule);

  return RET_OK;
}

static ret_t widget_set_prop_if_diff(widget_t* widget, const char* name, const value_t* v,
                                     bool_t set_force) {
  value_t old;

  if (!set_force) {
    if (widget->vt->inputable) {
      if (tk_str_eq(name, WIDGET_PROP_TEXT) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
        value_t inputing;

        if (widget_get_prop(widget, WIDGET_PROP_INPUTING, &inputing) == RET_OK) {
          if (value_bool(&inputing)) {
            log_debug("%s is inputing, skip.\n", widget_get_type(widget));
            return RET_OK;
          }
        }
      }
    }

    value_set_int(&old, 0);
    if (widget_get_prop(widget, name, &old) == RET_OK) {
      if (value_equal(&old, v)) {
        return RET_OK;
      }
    }
  }

  return widget_set_prop(widget, name, v);
}

static ret_t binding_context_awtk_update_data(data_binding_t* rule, bool_t force) {
  binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);

  if (tk_str_start_with(rule->path, DATA_BINDING_ERROR_OF)) {
    return RET_OK;
  }

  if (rule->mode == BINDING_ONE_WAY || rule->mode == BINDING_TWO_WAY ||
      (rule->mode == BINDING_ONCE && !(ctx->bound))) {
    value_t v;
    widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));

    value_set_int(&v, 0);

    if (data_binding_get_prop(rule, &v) == RET_OK) {
      ENSURE(widget_set_prop_if_diff(widget, rule->prop, &v, !(ctx->bound)) != RET_FAIL);
      value_reset(&v);
    } else {
      /*如果Model中对应的属性不存在，用widget中属性的值去初始化Model中的属性*/
      if (rule->mode == BINDING_TWO_WAY) {
        return_value_if_fail(widget_get_prop(widget, rule->prop, &v) == RET_OK, RET_OK);
        data_binding_set_prop(rule, &v);
      }
    }
  }

  return RET_OK;
}

ret_t binding_context_awtk_bind_data(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  widget_t* widget;
  data_binding_t* binding = DATA_BINDING(rule);
  return_value_if_fail(binding != NULL, RET_BAD_PARAMS);

  widget = WIDGET(BINDING_RULE_WIDGET(rule));
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  ret = custom_binder_bind(widget->vt->type, ctx, rule);
  if (ret == RET_DONE) {
    ret = RET_OK;
  } else if (ret == RET_NOT_FOUND || ret == RET_OK) {
    if (binding->trigger != UPDATE_WHEN_EXPLICIT) {
      if (binding->mode == BINDING_TWO_WAY || binding->mode == BINDING_ONE_WAY_TO_VIEW_MODEL) {
        if ((tk_str_eq(binding->prop, WIDGET_PROP_VALUE)) ||
            (tk_str_eq(binding->prop, WIDGET_PROP_TEXT) && widget->vt->inputable)) {
          if (binding->trigger == UPDATE_WHEN_CHANGING) {
            widget_on_with_tag(widget, EVT_VALUE_CHANGING, on_widget_value_change, rule, EVENT_TAG);
          }
          widget_on_with_tag(widget, EVT_VALUE_CHANGED, on_widget_value_change, rule, EVENT_TAG);
        } else {
          widget_on_with_tag(widget, EVT_PROP_CHANGED, on_widget_prop_change, rule, EVENT_TAG);
        }
      }
    }
    ret = RET_OK;
  }

  return ret;
}

static ret_t binding_context_awtk_update_command_stat(command_binding_t* rule) {
  widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));

  if (rule->auto_disable && !widget_is_window(widget)) {
    bool_t can_exec = command_binding_can_exec(rule);
    widget_set_enable(widget, can_exec);
  }

  return RET_OK;
}

static bool_t command_binding_filter(command_binding_t* rule, event_t* e) {
  return_value_if_fail(rule != NULL && e != NULL, TRUE);
  if (!(rule->filter.is_valid)) {
    return FALSE;
  }

  if (e->type == EVT_KEY_DOWN || e->type == EVT_KEY_UP || e->type == EVT_KEY_LONG_PRESS ||
      e->type == EVT_KEY_DOWN_BEFORE_CHILDREN || e->type == EVT_KEY_UP_BEFORE_CHILDREN) {
    shortcut_t shortcut;
    key_event_t* evt = (key_event_t*)e;

    if (e->type == EVT_KEY_LONG_PRESS) {
      log_debug("mvvm long press:%d\n", evt->key);
    }

    if (evt->key == TK_KEY_LCTRL || evt->key == TK_KEY_RCTRL) {
      return TRUE;
    }

    shortcut_init(&shortcut, evt->key);
    shortcut.ctrl = evt->ctrl;
    shortcut.lctrl = evt->lctrl;
    shortcut.rctrl = evt->rctrl;
    shortcut.alt = evt->alt;
    shortcut.lalt = evt->lalt;
    shortcut.ralt = evt->ralt;
    shortcut.shift = evt->shift;
    shortcut.lshift = evt->lshift;
    shortcut.rshift = evt->rshift;
    shortcut.cmd = evt->cmd;
    shortcut.menu = evt->menu;

    if (shortcut_match(&(rule->filter), &shortcut)) {
      return FALSE;
    } else {
      return TRUE;
    }
  }

  return FALSE;
}

static ret_t command_binding_exec_command(command_binding_t* rule) {
  ret_t ret = RET_OK;
  if (command_binding_can_exec(rule)) {
    if (rule->update_model) {
      binding_context_update_to_model(BINDING_RULE_CONTEXT(rule));
    }

    ret = command_binding_exec(rule);

    if (rule->close_window) {
      widget_t* win = widget_get_window(WIDGET(BINDING_RULE_WIDGET(rule)));
      window_close(win);
    }

    if (rule->quit_app) {
      tk_quit();
    }
    log_debug("%s return %d\n", rule->command, ret);
  } else {
    log_debug("%s cannot exec\n", rule->command);
  }

  if (ret == RET_OK || ret == RET_ITEMS_CHANGED || ret == RET_OBJECT_CHANGED) {
    return RET_STOP;
  }

  return RET_OK;
}

static ret_t on_widget_event(void* c, event_t* e) {
  command_binding_t* rule = COMMAND_BINDING(c);
  binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);

  if (!ctx->bound) {
    if (e->type == EVT_VALUE_CHANGED) {
      return RET_OK;
    }
  }

  if (ctx->updating_view) {
    if (e->type == EVT_VALUE_CHANGED) {
      if (tk_str_ieq(rule->event, STR_VALUE_CHANGED_BY_UI)) {
        return RET_OK;
      }
    }
  }

  if (command_binding_filter(rule, e)) {
    return RET_OK;
  }

  return command_binding_exec_command(rule);
}

static ret_t binding_context_off_global_when_widget_destroy(void* ctx, event_t* e) {
  window_manager_t* wm = WINDOW_MANAGER(window_manager());
  uint32_t id = (uint32_t)tk_pointer_to_int(ctx);

  if (wm != NULL && wm->global_emitter != NULL) {
    emitter_off(wm->global_emitter, id);
  }

  return RET_REMOVE;
}

ret_t binding_context_awtk_bind_command(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  widget_t* widget;
  command_binding_t* binding = COMMAND_BINDING(rule);
  return_value_if_fail(binding != NULL, RET_BAD_PARAMS);

  widget = WIDGET(BINDING_RULE_WIDGET(rule));
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  ret = custom_binder_bind(widget->vt->type, ctx, rule);
  if (ret == RET_DONE) {
    ret = RET_OK;
  } else if (ret == RET_NOT_FOUND || ret == RET_OK) {
    int32_t event = event_from_name(binding->event);
    if (event != EVT_NONE) {
      if (strstr(binding->event, STR_GLOBAL_EVENT_PREFIX) != NULL) {
        window_manager_t* wm = WINDOW_MANAGER(widget_get_window_manager(widget));
        uint32_t id = emitter_on(wm->global_emitter, event, on_widget_event, rule);
        if (id != TK_INVALID_ID) {
          widget_on(widget, EVT_DESTROY, binding_context_off_global_when_widget_destroy,
                    tk_pointer_from_int(id));
        }
      } else {
        widget_on_with_tag(widget, event, on_widget_event, rule, EVENT_TAG);
      }
    } else {
      log_debug("not found event %s\n", binding->event);
    }
    ret = RET_OK;
  }

  return ret;
}

ret_t binding_context_awtk_bind_items(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  widget_t* widget;
  items_binding_t* binding = ITEMS_BINDING(rule);
  return_value_if_fail(binding != NULL, RET_BAD_PARAMS);

  widget = WIDGET(BINDING_RULE_WIDGET(rule));
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  ret = custom_binder_bind(widget->vt->type, ctx, rule);
  if (ret == RET_DONE || ret == RET_NOT_FOUND) {
    ret = RET_OK;
  }

  return ret;
}

ret_t binding_context_awtk_bind_condition(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  widget_t* widget;
  condition_binding_t* binding = CONDITION_BINDING(rule);
  return_value_if_fail(binding != NULL, RET_BAD_PARAMS);

  widget = WIDGET(BINDING_RULE_WIDGET(rule));
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  ret = custom_binder_bind(widget->vt->type, ctx, rule);
  if (ret == RET_DONE || ret == RET_NOT_FOUND) {
    ret = RET_OK;
  }

  return ret;
}

static ret_t binding_context_awtk_find_binding_rule(slist_t* slist, tk_compare_t cmp, void* ctx,
                                                    darray_t* matched) {
  darray_t* node = NULL;
  slist_node_t* iter = NULL;
  return_value_if_fail(slist != NULL && matched != NULL, RET_BAD_PARAMS);

  iter = slist->first;
  while (iter != NULL) {
    node = (darray_t*)(iter->data);
    if (darray_find_all(node, cmp, ctx, matched) != RET_OK) {
      return RET_FAIL;
    }

    iter = iter->next;
  }

  return RET_OK;
}

static int32_t binding_context_awtk_compare_items_object(binding_rule_t* rule,
                                                         tk_object_t* target) {
  if (binding_rule_is_items_binding(rule)) {
    items_binding_t* items_binding = ITEMS_BINDING(rule);
    binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
    view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
    binding_rule_t* parent = rule->parent;
    const char* path = items_binding->items_name;
    value_t v;

    path = binding_context_resolve_path_by_rule(ctx, parent, path, NULL);
    if (path == NULL) {
      return TK_OBJECT(view_model) - target;
    } else {
      if (view_model_get_prop(view_model, path, &v) == RET_OK) {
        tk_object_t* obj = value_object(&v);
        return obj - target;
      }
    }
  }
  return -1;
}

static ret_t binding_context_rebind_items_sync(binding_rule_t* rule) {
  log_debug("start_rebind\n");
  ui_loader_mvvm_reload_widget(rule);
  binding_context_update_to_view(BINDING_RULE_CONTEXT(rule));
  log_debug("end_rebind\n");

  return RET_OK;
}

static ret_t binding_context_rebind_items_in_idle(const idle_info_t* info) {
  items_binding_t* rule = ITEMS_BINDING(info->ctx);

  rule->rebind_idle_id = TK_INVALID_ID;
  binding_context_rebind_items_sync(BINDING_RULE(rule));

  return RET_REMOVE;
}

static ret_t visit_items_binding_rebind(void* ctx, const void* data) {
  items_binding_t* rule = ITEMS_BINDING(data);
  if (rule->rebind_idle_id == TK_INVALID_ID) {
    rule->rebind_idle_id = idle_add(binding_context_rebind_items_in_idle, rule);
  }
  return RET_OK;
}

static ret_t visit_items_binding_rebind_sync(void* ctx, const void* data) {
  items_binding_t* rule = ITEMS_BINDING(data);
  if (rule->rebind_idle_id != TK_INVALID_ID) {
    idle_remove(rule->rebind_idle_id);
    rule->rebind_idle_id = TK_INVALID_ID;
  }

  binding_context_rebind_items_sync(BINDING_RULE(rule));
  return RET_OK;
}

static ret_t binding_context_awtk_notify_items_changed(binding_context_t* ctx, tk_object_t* items,
                                                       bool_t sync) {
  darray_t matched;
  tk_compare_t compare = (tk_compare_t)binding_context_awtk_compare_items_object;
  slist_t* bindings = &(ctx->dynamic_bindings);

  if (ctx->update_view_idle_id != TK_INVALID_ID) {
    idle_remove(ctx->update_view_idle_id);
    ctx->update_view_idle_id = TK_INVALID_ID;
    ctx->updating_view = FALSE;
  }

  darray_init(&matched, 1, NULL, NULL);
  if (binding_context_awtk_find_binding_rule(bindings, compare, items, &matched) == RET_OK) {
    if (sync) {
      darray_foreach(&matched, visit_items_binding_rebind_sync, NULL);
    } else {
      darray_foreach(&matched, visit_items_binding_rebind, NULL);
    }
  }
  darray_deinit(&matched);

  return RET_OK;
}

uint32_t binding_context_awtk_calc_widget_index_of_rule(binding_context_t* ctx,
                                                        binding_rule_t* rule) {
  if (!binding_rule_is_items_binding(rule) && !binding_rule_is_condition_binding(rule)) {
    widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));
    return_value_if_fail(widget != NULL, 0);

    return widget_index_of(widget);
  } else {
    value_t v;
    uint32_t index = 0;
    darray_t* node = NULL;
    const char* name = WIDGET_PROP_MVVM_COUNT_OF_WIDGET_BEFORE_FIRST_DYNAMIC_RULE;
    widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));
    return_value_if_fail(widget != NULL, 0);

    if (widget->custom_props != NULL &&
        tk_object_get_prop(widget->custom_props, name, &v) == RET_OK) {
      index = value_uint32(&v);
    } else {
      index = widget_count_children(widget);
      widget_set_prop_int(widget, name, index);
    }

    if (ctx->dynamic_bindings.first != NULL) {
      node = (darray_t*)(slist_find(&(ctx->dynamic_bindings), widget));
      if (node != NULL && node->elms != NULL) {
        uint32_t i = 0;
        binding_rule_t* r = NULL;

        for (i = 0; i < node->size; i++) {
          r = BINDING_RULE(node->elms[i]);
          if (r == rule) {
            break;
          }

          if (binding_rule_is_condition_binding(r)) {
            condition_binding_t* binding = CONDITION_BINDING(r);
            index += 1;
            index += binding->static_widget_before_next_dynamic_binding;
          } else if (binding_rule_is_items_binding(r)) {
            items_binding_t* binding = ITEMS_BINDING(r);
            index += binding->items_count;
            index += binding->static_widget_before_next_dynamic_binding;
          }
        }
      }
    }

    return index;
  }
}

uint32_t binding_context_awtk_get_items_cursor_of_rule(binding_context_t* ctx,
                                                       binding_rule_t* rule) {
  binding_rule_t* temp = rule->parent;

  while (temp != NULL && !binding_rule_is_items_binding(temp)) {
    temp = temp->parent;
  }

  if (temp != NULL) {
    items_binding_t* binding = ITEMS_BINDING(temp);

    if (!binding->bound) {
      return binding->cursor + binding->start_item_index;
    } else {
      uint32_t cursor = 0;
      widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));
      widget_t* parent = WIDGET(BINDING_RULE_WIDGET(temp));

      while (widget != NULL && widget->parent != parent) {
        widget = widget->parent;
      }
      return_value_if_fail(widget != NULL, 0);

      cursor = tk_object_get_prop_uint32(widget->custom_props, WIDGET_PROP_MVVM_DATA_CURSOR, 0);
      cursor += binding->start_item_index;
      return cursor;
    }
  }

  return 0;
}

const char* binding_context_awtk_resolve_path_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                                      const char* path, bool_t* is_cursor) {
  if (path == NULL || *path == '\0' || rule == NULL) {
    return path;
  } else {
    char new_item_name[MAX_PATH];
    char new_index_name[TK_NUM_MAX_LEN + 1];
    const char* items_name;
    const char* old_item_name;
    const char* old_index_name;
    int32_t old_item_name_len;
    int32_t new_item_name_len;
    int32_t delta;
    uint32_t cursor = 0;
    items_binding_t* binding = NULL;
    widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));
    str_t* temp = &(ctx->temp);

    str_set(temp, path);

    do {
      if (binding_rule_is_items_binding(rule)) {
        binding = ITEMS_BINDING(rule);
        items_name = binding->items_name;
        old_index_name = binding->index_name != NULL ? binding->index_name : "index";
        old_item_name = binding->item_name != NULL ? binding->item_name : "item";
        old_item_name_len = strlen(old_item_name);

        if (binding->bound) {
          widget_t* parent = WIDGET(BINDING_RULE_WIDGET(rule));
          while (widget != NULL && widget->parent != parent) {
            widget = widget->parent;
          }
          return_value_if_fail(widget != NULL, NULL);
          binding->cursor =
              tk_object_get_prop_uint32(widget->custom_props, WIDGET_PROP_MVVM_DATA_CURSOR, 0);
        }

        cursor = binding->cursor + binding->start_item_index;

        if ((old_item_name_len <= (int32_t)temp->size) &&
            (temp->str[old_item_name_len] == '.' || temp->str[old_item_name_len] == '\0') &&
            (str_start_with(temp, old_item_name))) {
          if (items_name == NULL) {
            tk_snprintf(new_item_name, sizeof(new_item_name) - 1, "[%u]", cursor);
          } else {
            tk_snprintf(new_item_name, sizeof(new_item_name) - 1, "%s.[%u]", items_name, cursor);
          }

          new_item_name_len = strlen(new_item_name);
          delta = strlen(new_item_name) - old_item_name_len;
          if (delta >= 0) {
            str_insert_with_len(temp, 0, new_item_name, delta);
            memcpy(temp->str + delta, new_item_name + delta, new_item_name_len - delta);
          } else {
            str_remove(temp, 0, -delta);
            memcpy(temp->str, new_item_name, new_item_name_len);
          }

          if (items_name == NULL) {
            break;
          }
        } else if (tk_str_eq(temp->str, old_index_name)) {
          tk_snprintf(new_index_name, sizeof(new_index_name) - 1, "%u", cursor);
          str_set(temp, new_index_name);

          if (is_cursor != NULL) {
            *is_cursor = TRUE;
          }

          break;
        }
      }
      rule = rule->parent;
    } while (rule != NULL);

    return temp->str;
  }
}

static ret_t binding_context_awtk_send_key(widget_t* win, const char* args) {
  int size = 0;
  key_event_t e;
  const char* key = NULL;
  char target_name[64];
  widget_t* target = NULL;
  widget_t* wm = win->parent;
  return_value_if_fail(args != NULL, RET_BAD_PARAMS);

  key = strchr(args, ':');
  if (key != NULL) {
    size = key - args;
    return_value_if_fail(size < sizeof(target_name), RET_BAD_PARAMS);

    tk_strncpy(target_name, args, size);
    target = widget_lookup(win, target_name, TRUE);
    key++;
  } else {
    key = args;
  }

  if (*key) {
    const key_type_value_t* kt = keys_type_find(key);
    int32_t code = kt != NULL ? kt->value : *key;

    key_event_init(&e, EVT_KEY_DOWN, wm, code);
    if (target == NULL) {
      window_manager_dispatch_input_event(wm, (event_t*)&e);
    } else {
      widget_dispatch(target, (event_t*)&e);
    }

    key_event_init(&e, EVT_KEY_UP, wm, code);
    if (target == NULL) {
      window_manager_dispatch_input_event(wm, (event_t*)&e);
    } else {
      widget_dispatch(target, (event_t*)&e);
    }
  }

  return RET_OK;
}

static ret_t binding_context_awtk_set_widget_prop(widget_t* win, const char* args) {
  int size = 0;
  char name[64];
  char key[64];
  const char* p = NULL;
  const char* value = NULL;
  widget_t* target = NULL;
  return_value_if_fail(args != NULL, RET_BAD_PARAMS);

  p = strchr(args, '.');
  return_value_if_fail(p != NULL, RET_BAD_PARAMS);
  size = p - args;
  return_value_if_fail(size < sizeof(name), RET_BAD_PARAMS);
  tk_strncpy(name, args, size);

  args = p + 1;
  p = strchr(args, ':');
  return_value_if_fail(p != NULL, RET_BAD_PARAMS);
  size = p - args;
  return_value_if_fail(size < sizeof(key), RET_BAD_PARAMS);
  tk_strncpy(key, args, size);

  value = p + 1;
  target = widget_lookup(win, name, TRUE);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  return widget_set_prop_str(target, key, value);
}

static bool_t binding_context_awtk_can_exec(binding_context_t* ctx, const char* cmd,
                                            const char* args) {
  if (tk_str_ieq(COMMAND_BINDING_CMD_SEND_KEY, cmd)) {
    return TRUE;
  } else if (tk_str_ieq(COMMAND_BINDING_CMD_SET_WIDGET_PROP, cmd)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t binding_context_awtk_exec(binding_context_t* ctx, const char* cmd, const char* args) {
  if (tk_str_ieq(COMMAND_BINDING_CMD_SEND_KEY, cmd)) {
    widget_t* win = widget_get_window(WIDGET(ctx->widget));

    return binding_context_awtk_send_key(win, args);
  } else if (tk_str_ieq(COMMAND_BINDING_CMD_SET_WIDGET_PROP, cmd)) {
    widget_t* win = widget_get_window(WIDGET(ctx->widget));

    return binding_context_awtk_set_widget_prop(win, args);
  }

  return RET_NOT_IMPL;
}

static ret_t visit_data_binding_update_to_model(void* ctx, const void* data) {
  value_t v;
  data_binding_t* rule = DATA_BINDING(data);
  widget_t* widget = WIDGET(BINDING_RULE_WIDGET(rule));

  if (rule->trigger == UPDATE_WHEN_EXPLICIT) {
    if (rule->mode == BINDING_TWO_WAY || rule->mode == BINDING_ONE_WAY_TO_VIEW_MODEL) {
      return_value_if_fail(widget_get_prop(widget, rule->prop, &v) == RET_OK, RET_OK);
      return_value_if_fail(data_binding_set_prop(rule, &v) == RET_OK, RET_OK);
    }
  }

  return RET_OK;
}

static ret_t widget_visit_data_binding_update_to_model(void* ctx, const void* data) {
  darray_t* node = (darray_t*)data;

  (void)ctx;
  return darray_foreach(node, visit_data_binding_update_to_model, NULL);
}

static ret_t binding_context_awtk_update_to_model(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  return slist_foreach(&(ctx->data_bindings), widget_visit_data_binding_update_to_model, NULL);
}

static ret_t visit_data_binding_update_to_view(void* ctx, const void* data) {
  return binding_context_awtk_update_data(DATA_BINDING(data), FALSE);
}

static ret_t widget_visit_data_binding_update_to_view(void* ctx, const void* data) {
  darray_t* node = (darray_t*)data;
  widget_t* widget = WIDGET(BINDING_RULE_WIDGET(node->elms[0]));

  (void)ctx;
  return darray_foreach(node, visit_data_binding_update_to_view, NULL);
}

static ret_t visit_command_binding_update_to_view(void* ctx, const void* data) {
  return binding_context_awtk_update_command_stat(COMMAND_BINDING(data));
}

static ret_t widget_visit_command_binding_update_to_view(void* ctx, const void* data) {
  darray_t* node = (darray_t*)data;

  (void)ctx;
  return darray_foreach(node, visit_command_binding_update_to_view, NULL);
}

static ret_t binding_context_awtk_update_data_of_widget(binding_context_t* ctx, widget_t* target) {
  widget_t* widget = NULL;
  darray_t* node = NULL;
  binding_rule_t* rule = NULL;
  slist_node_t* iter = ctx->data_bindings.first;

  while (iter != NULL) {
    node = (darray_t*)(iter->data);
    if (node->size > 0 && node->elms != NULL) {
      rule = BINDING_RULE(node->elms[0]);
      widget = WIDGET(BINDING_RULE_WIDGET(rule));

      if (target == widget || widget_is_parent_of(target, widget)) {
        darray_foreach(node, visit_data_binding_update_to_view, NULL);
      }
    }

    iter = iter->next;
  }

  return RET_OK;
}

static ret_t binding_context_awtk_update_command_stat_of_widget(binding_context_t* ctx,
                                                                widget_t* target) {
  widget_t* widget = NULL;
  darray_t* node = NULL;
  binding_rule_t* rule = NULL;
  slist_node_t* iter = ctx->command_bindings.first;

  while (iter != NULL) {
    node = (darray_t*)(iter->data);
    if (node->size > 0 && node->elms != NULL) {
      rule = BINDING_RULE(node->elms[0]);
      widget = WIDGET(BINDING_RULE_WIDGET(rule));

      if (target == widget || widget_is_parent_of(target, widget)) {
        darray_foreach(node, visit_command_binding_update_to_view, NULL);
      }
    }

    iter = iter->next;
  }

  return RET_OK;
}

static ret_t visit_dynamic_binding_update_to_view(void* ctx, const void* data) {
  binding_rule_t* rule = BINDING_RULE(data);
  binding_context_t* bctx = BINDING_RULE_CONTEXT(data);

  if (bctx->bound && binding_rule_is_condition_binding(rule)) {
    uint32_t index = binding_context_calc_widget_index_of_rule(bctx, rule);
    widget_t* parent = WIDGET(BINDING_RULE_WIDGET(rule));
    widget_t* old_widget = widget_get_child(parent, index);
    widget_t* new_widget = NULL;

    ui_loader_mvvm_reload_widget(rule);

    new_widget = widget_get_child(parent, index);
    if (new_widget != NULL && new_widget != old_widget) {
      bool_t should_update = (bool_t)(ctx);

      if (should_update) {
        binding_context_awtk_update_data_of_widget(bctx, new_widget);
        binding_context_awtk_update_command_stat_of_widget(bctx, new_widget);
      }
    }
  }

  return RET_OK;
}

static ret_t widget_visit_dynamic_binding_update_to_view(void* ctx, const void* data) {
  darray_t* node = (darray_t*)data;

  return darray_foreach(node, visit_dynamic_binding_update_to_view, ctx);
}

static ret_t binding_context_awtk_update_to_view_sync(binding_context_t* ctx) {
  slist_foreach(&(ctx->dynamic_bindings), widget_visit_dynamic_binding_update_to_view, FALSE);
  slist_foreach(&(ctx->data_bindings), widget_visit_data_binding_update_to_view, NULL);
  slist_foreach(&(ctx->command_bindings), widget_visit_command_binding_update_to_view, NULL);
  widget_invalidate_force(WIDGET(ctx->widget), NULL);
  ctx->updating_view = FALSE;

  return RET_OK;
}

static ret_t idle_update_to_view(const idle_info_t* info) {
  binding_context_t* ctx = BINDING_CONTEXT(info->ctx);
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  ctx->update_view_idle_id = TK_INVALID_ID;
  binding_context_awtk_update_to_view_sync(ctx);

  return RET_REMOVE;
}

static ret_t binding_context_awtk_update_to_view(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->view_model != NULL, RET_BAD_PARAMS);

  ctx->updating_view = TRUE;

  if (ctx->bound) {
    if (ctx->update_view_idle_id == TK_INVALID_ID) {
      ctx->update_view_idle_id = idle_add(idle_update_to_view, ctx);
    }
  } else {
    binding_context_awtk_update_to_view_sync(ctx);
  }

  return RET_OK;
}

static int32_t binding_context_awtk_compare_items_binding(binding_rule_t* rule, const void* other) {
  (void)other;
  if (binding_rule_is_items_binding(rule)) {
    return 0;
  }
  return -1;
}

static ret_t binding_context_awtk_remove_rebind_idle(void* ctx, const void* data) {
  items_binding_t* rule = ITEMS_BINDING(data);

  if (rule->rebind_idle_id != TK_INVALID_ID) {
    idle_remove(rule->rebind_idle_id);
    rule->rebind_idle_id = TK_INVALID_ID;
  }

  (void)ctx;
  return RET_OK;
}

static ret_t binding_context_awtk_remove_all_rebind_idle(binding_context_t* ctx) {
  darray_t matched;
  tk_compare_t compare = (tk_compare_t)binding_context_awtk_compare_items_binding;
  slist_t* bindings = &(ctx->dynamic_bindings);

  darray_init(&matched, 1, NULL, NULL);
  if (binding_context_awtk_find_binding_rule(bindings, compare, NULL, &matched) == RET_OK) {
    darray_foreach(&matched, binding_context_awtk_remove_rebind_idle, NULL);
  }
  darray_deinit(&matched);

  return RET_OK;
}

static ret_t on_reset_emitter(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);
  if (widget->emitter != NULL) {
    widget_off_by_tag(widget, EVENT_TAG);
  }

  (void)ctx;
  return RET_OK;
}

static ret_t binding_context_awtk_unbind_widget(binding_context_t* ctx) {
  if (ctx->widget != NULL) {
    widget_t* widget = WIDGET(ctx->widget);

    binding_context_awtk_remove_all_rebind_idle(ctx);

    if (ctx->update_view_idle_id != TK_INVALID_ID) {
      idle_remove(ctx->update_view_idle_id);
      ctx->update_view_idle_id = TK_INVALID_ID;
    }

    widget_foreach(widget, on_reset_emitter, NULL);

    if (widget->custom_props != NULL) {
      tk_object_t* props = widget->custom_props;
      darray_t* children =
          (darray_t*)tk_object_get_prop_pointer(props, WIDGET_PROP_V_MODEL_CHILDREN);
      asset_info_t* ui = tk_object_get_prop_pointer(props, WIDGET_PROP_MVVM_ASSETS_INFO);

      if (children != NULL) {
        darray_destroy(children);
      }

      if (ui != NULL) {
        assets_manager_unref(assets_manager(), ui);
      }
    }

    ctx->widget = NULL;
  }

  return RET_OK;
}

static ret_t binding_context_awtk_destroy(binding_context_t* ctx) {
  return binding_context_awtk_unbind_widget(ctx);
}

static const binding_context_vtable_t s_binding_context_vtable = {
    .destroy = binding_context_awtk_destroy,
    .update_to_view = binding_context_awtk_update_to_view,
    .update_to_model = binding_context_awtk_update_to_model,
    .exec = binding_context_awtk_exec,
    .can_exec = binding_context_awtk_can_exec,
    .bind_data = binding_context_awtk_bind_data,
    .bind_command = binding_context_awtk_bind_command,
    .bind_conditon = binding_context_awtk_bind_condition,
    .bind_items = binding_context_awtk_bind_items,
    .notify_items_changed = binding_context_awtk_notify_items_changed,
    .get_items_cursor_of_rule = binding_context_awtk_get_items_cursor_of_rule,
    .calc_widget_index_of_rule = binding_context_awtk_calc_widget_index_of_rule,
    .resolve_path_by_rule = binding_context_awtk_resolve_path_by_rule};

static darray_t* binding_context_awtk_get_children(binding_context_t* ctx) {
  widget_t* widget = WIDGET(ctx->widget);
  tk_object_t* props = widget->custom_props;
  darray_t* children = NULL;

  if (props != NULL && tk_object_has_prop(props, WIDGET_PROP_V_MODEL_CHILDREN)) {
    children = (darray_t*)tk_object_get_prop_pointer(props, WIDGET_PROP_V_MODEL_CHILDREN);
  }

  return children;
}

static ret_t binding_context_awtk_add_child(binding_context_t* ctx, binding_context_t* child) {
  darray_t* children = binding_context_awtk_get_children(ctx);

  if (children == NULL) {
    widget_t* widget = WIDGET(ctx->widget);
    children = darray_create(1, NULL, NULL);
    widget_set_prop_pointer(widget, WIDGET_PROP_V_MODEL_CHILDREN, children);
  }

  darray_push(children, child);

  return RET_OK;
}

static ret_t binding_context_awtk_remove_child(binding_context_t* ctx, binding_context_t* child) {
  darray_t* children = binding_context_awtk_get_children(ctx);

  if (children != NULL) {
    darray_remove(children, child);
  }

  return RET_OK;
}

static ret_t binding_context_awtk_on_widget_remove(void* ctx, event_t* e) {
  binding_context_t* bctx = BINDING_CONTEXT(ctx);
  widget_t* widget = WIDGET(bctx->widget);

  if (widget->parent == NULL || widget_index_of(widget) == -1) {
    binding_context_t* root = binding_context_get_root(bctx);

    if (root != NULL && root != bctx) {
      binding_context_awtk_remove_child(root, ctx);
    }
    binding_context_set_bound(bctx, FALSE);
    return RET_REMOVE;
  }
  return RET_OK;
}

static ret_t binding_context_awtk_destroy_async(const idle_info_t* info) {
  binding_context_destroy(BINDING_CONTEXT(info->ctx));

  return RET_REMOVE;
}

static ret_t binding_context_awtk_on_widget_destroy(void* ctx, event_t* e) {
  binding_context_t* bctx = BINDING_CONTEXT(ctx);

  binding_context_set_bound(bctx, FALSE);

  binding_context_awtk_unbind_widget(bctx);

  if (bctx->parent != NULL) {
    binding_context_set_parent(bctx, NULL);
  }

  idle_add(binding_context_awtk_destroy_async, ctx);
  (void)e;
  return RET_REMOVE;
}

binding_context_t* binding_context_awtk_create(binding_context_t* parent, const char* vmodel,
                                               navigator_request_t* req, widget_t* widget) {
  view_model_t* view_model = NULL;
  view_model_t* parent_view_model = NULL;
  binding_context_t* ctx = NULL;
  return_value_if_fail(req != NULL && widget != NULL, NULL);

  if (parent != NULL) {
    parent_view_model = parent->view_model;
  }

  view_model = binding_context_awtk_create_view_model(parent_view_model, vmodel, req);
  return_value_if_fail(view_model != NULL, NULL);

  ctx = TKMEM_ZALLOC(binding_context_t);
  if (ctx != NULL) {
    ctx->vt = &s_binding_context_vtable;
    ctx->parent = parent;

    if (binding_context_init(ctx, req) == RET_OK &&
        binding_context_set_view_model(ctx, view_model) == RET_OK) {
      ctx->widget = widget;
      ENSURE(widget->parent != NULL);
      widget_on(widget->parent, EVT_WIDGET_REMOVE_CHILD, binding_context_awtk_on_widget_remove,
                ctx);
      widget_on(widget, EVT_DESTROY, binding_context_awtk_on_widget_destroy, ctx);
      widget_set_prop_pointer(widget, WIDGET_PROP_V_MODEL, ctx);

      if (parent != NULL) {
        binding_context_t* root = binding_context_get_root(parent);
        binding_context_awtk_add_child(root, ctx);
      }
    } else {
      binding_context_destroy(ctx);
      ctx = NULL;
    }
  }

  tk_object_unref(TK_OBJECT(view_model));

  return ctx;
}
