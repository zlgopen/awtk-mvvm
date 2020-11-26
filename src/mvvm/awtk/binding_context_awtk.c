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
#include "ui_loader/ui_serializer.h"
#include "mvvm/base/view_model_dummy.h"
#include "mvvm/base/view_model_array.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/custom_binder.h"
#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "mvvm/base/view_model_compositor.h"

#define STR_SUB_VIEW_MODEL "sub_view_model:"
#define STR_SUB_VIEW_MODEL_ARRAY "sub_view_model_array:"
#define WIDGET_PROP_TEMPLATE_WIDGET "template_widget"

static ret_t binding_context_put_widget(binding_context_t* ctx, widget_t* widget) {
  darray_t* cache = &(ctx->cache_widgets);

  if (darray_push(cache, widget) != RET_OK) {
    widget_unref(widget);
  }

  return RET_OK;
}

static widget_t* binding_context_get_widget(binding_context_t* ctx, widget_t* container) {
  darray_t* cache = &(ctx->cache_widgets);
  widget_t* widget = darray_pop(cache);
  if (widget == NULL) {
    widget_t* template_widget =
        WIDGET(widget_get_prop_pointer(container, WIDGET_PROP_TEMPLATE_WIDGET));
    widget = widget_clone(template_widget, NULL);
  }

  return widget;
}

#define EVENT_TAG 0x11223300
static ret_t binding_context_bind_for_widget(widget_t* widget, binding_context_t* parent_ctx,
                                             navigator_request_t* req);

static const char* widget_get_prop_vmodel(widget_t* widget) {
  value_t v;
  value_set_str(&v, NULL);

  return (widget_get_prop(widget, WIDGET_PROP_V_MODEL, &v) == RET_OK) ? value_str(&v) : NULL;
}

static ret_t view_model_on_window_close(void* ctx, event_t* e) {
  view_model_on_will_unmount(VIEW_MODEL(ctx));

  return RET_OK;
}

static ret_t view_model_on_window_destroy(void* ctx, event_t* e) {
  view_model_on_unmount(VIEW_MODEL(ctx));

  return RET_OK;
}

static ret_t visit_data_binding_update_error_of(void* ctx, const void* data) {
  data_binding_t* rule = DATA_BINDING(data);
  data_binding_t* trigger_rule = DATA_BINDING(ctx);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(trigger_rule);

  if (tk_str_start_with(rule->path, DATA_BINDING_ERROR_OF)) {
    const char* path = rule->path + sizeof(DATA_BINDING_ERROR_OF) - 1;
    if (tk_str_eq(path, trigger_rule->path)) {
      widget_t* widget = WIDGET(BINDING_RULE(rule)->widget);
      widget_set_tr_text(widget, view_model->last_error.str);
    }
  }

  return RET_OK;
}

static ret_t binding_context_update_error_of(data_binding_t* rule) {
  binding_context_t* ctx = BINDING_RULE(rule)->binding_context;
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(ctx != NULL && view_model != NULL, RET_BAD_PARAMS);

  if (view_model->last_error.size > 0) {
    darray_foreach(&(ctx->data_bindings), visit_data_binding_update_error_of, rule);
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

static ret_t binding_context_bind_data(binding_context_t* ctx, const char* name,
                                       const char* value) {
  widget_t* widget = WIDGET(ctx->current_widget);
  data_binding_t* rule = (data_binding_t*)binding_rule_parse(name, value, widget->vt->inputable);
  return_value_if_fail(rule != NULL, RET_FAIL);

  BINDING_RULE(rule)->widget = widget;
  BINDING_RULE(rule)->binding_context = ctx;

  if (object_is_collection(OBJECT(ctx->view_model))) {
    uint32_t cursor = object_get_prop_int(OBJECT(ctx->view_model), VIEW_MODEL_PROP_CURSOR, 0);
    BINDING_RULE(rule)->cursor = cursor;
  }

  goto_error_if_fail(darray_push(&(ctx->data_bindings), rule) == RET_OK);

  if (rule->trigger != UPDATE_WHEN_EXPLICIT) {
    if (rule->mode == BINDING_TWO_WAY || rule->mode == BINDING_ONE_WAY_TO_VIEW_MODEL) {
      bool_t inputable = widget->vt->inputable;

      if (tk_str_eq(rule->prop, WIDGET_PROP_VALUE) ||
          (tk_str_eq(rule->prop, WIDGET_PROP_TEXT) && inputable)) {
        if (rule->trigger == UPDATE_WHEN_CHANGING) {
          widget_on_with_tag(widget, EVT_VALUE_CHANGING, on_widget_value_change, rule, EVENT_TAG);
        }
        widget_on_with_tag(widget, EVT_VALUE_CHANGED, on_widget_value_change, rule, EVENT_TAG);
      } else {
        widget_on_with_tag(widget, EVT_PROP_CHANGED, on_widget_prop_change, rule, EVENT_TAG);
      }
    }
  }

  return RET_OK;
error:
  object_unref(OBJECT(rule));

  return RET_FAIL;
}

#define STR_GLOBAL "global"
#define STR_GLOBAL_KEY_UP "global_key_up"
#define STR_GLOBAL_KEY_DOWN "global_key_down"
#define STR_VALUE_CHANGED_BY_UI "value_changed_by_ui"
#define STR_GLOBAL_KEY_LONG_PRESS "global_key_long_press"

#define STR_VALUE_CHANGED_BY_UI "value_changed_by_ui"
/*TODO: add more event*/
static int_str_t s_event_map[] = {{EVT_CLICK, "click"},
                                  {EVT_WINDOW_CLOSE, "window_close"},
                                  {EVT_WINDOW_OPEN, "window_open"},
                                  {EVT_POINTER_DOWN, "pointer_down"},
                                  {EVT_POINTER_UP, "pointer_up"},
                                  {EVT_KEY_DOWN, "key_down"},
                                  {EVT_KEY_LONG_PRESS, "key_long_press"},
                                  {EVT_KEY_UP, "key_up"},
                                  {EVT_VALUE_CHANGED, "value_changed"},
                                  {EVT_VALUE_CHANGED, STR_VALUE_CHANGED_BY_UI},
                                  {EVT_KEY_DOWN_BEFORE_CHILDREN, "key_down_before_children"},
                                  {EVT_KEY_UP_BEFORE_CHILDREN, "key_up_before_children"},
                                  {EVT_KEY_DOWN, STR_GLOBAL_KEY_DOWN},
                                  {EVT_KEY_LONG_PRESS, STR_GLOBAL_KEY_LONG_PRESS},
                                  {EVT_KEY_UP, STR_GLOBAL_KEY_UP},

                                  {EVT_NONE, NULL}};

static bool_t command_binding_filter(command_binding_t* rule, event_t* e) {
  return_value_if_fail(rule != NULL && e != NULL, TRUE);
  if (!(rule->filter.is_valid)) {
    return FALSE;
  }

  if (e->type == EVT_KEY_DOWN || e->type == EVT_KEY_UP || e->type == EVT_KEY_LONG_PRESS || 
      e->type == EVT_KEY_DOWN_BEFORE_CHILDREN|| e->type == EVT_KEY_UP_BEFORE_CHILDREN) {
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
      binding_context_update_to_model(BINDING_RULE(rule)->binding_context);
    }

    ret = command_binding_exec(rule);

    if (rule->close_window) {
      widget_t* win = widget_get_window(BINDING_RULE(rule)->widget);
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
  binding_context_t* ctx = BINDING_RULE(rule)->binding_context;

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

static ret_t binding_context_bind_command(binding_context_t* ctx, const char* name,
                                          const char* value) {
  int32_t event = 0;
  widget_t* widget = WIDGET(ctx->current_widget);
  command_binding_t* rule = (command_binding_t*)binding_rule_parse(name, value, BINDING_ONCE);
  return_value_if_fail(rule != NULL, RET_FAIL);

  BINDING_RULE(rule)->widget = widget;
  BINDING_RULE(rule)->binding_context = ctx;

  if (object_is_collection(OBJECT(ctx->view_model))) {
    uint32_t cursor = object_get_prop_int(OBJECT(ctx->view_model), VIEW_MODEL_PROP_CURSOR, 0);
    BINDING_RULE(rule)->cursor = cursor;
  }

  goto_error_if_fail(darray_push(&(ctx->command_bindings), rule) == RET_OK);

  event = int_str_name(s_event_map, rule->event, EVT_NONE);
  if (event != EVT_NONE) {
    if (strstr(rule->event, STR_GLOBAL) != NULL) {
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
    log_debug("not found event %s\n", rule->event);
  }

  return RET_OK;
error:
  object_unref(OBJECT(rule));

  return RET_FAIL;
}

static ret_t visit_bind_one_prop(void* ctx, const void* data) {
  binding_context_t* bctx = (binding_context_t*)(ctx);
  named_value_t* nv = (named_value_t*)data;

  if (tk_str_start_with(nv->name, BINDING_RULE_DATA_PREFIX)) {
    binding_context_bind_data(bctx, nv->name, value_str(&(nv->value)));
  } else if (tk_str_start_with(nv->name, BINDING_RULE_COMMAND_PREFIX)) {
    binding_context_bind_command(bctx, nv->name, value_str(&(nv->value)));
  }

  return RET_OK;
}

static ret_t on_view_model_prop_change(void* ctx, event_t* e) {
  binding_context_update_to_view((binding_context_t*)ctx);

  return RET_OK;
}

static view_model_t* binding_context_awtk_create_view_model_one(view_model_t* parent,
                                                                const char* vmodel,
                                                                navigator_request_t* req) {
  view_model_t* view_model = NULL;

  if (tk_str_start_with(vmodel, STR_SUB_VIEW_MODEL)) {
    const char* name = strchr(vmodel, ':');
    return_value_if_fail(name != NULL, NULL);
    view_model = view_model_create_sub_view_model(parent, name + 1);
  } else if (tk_str_start_with(vmodel, STR_SUB_VIEW_MODEL_ARRAY)) {
    const char* name = strchr(vmodel, ':');
    return_value_if_fail(name != NULL, NULL);
    view_model = view_model_create_sub_view_model_array(parent, name + 1);
  } else {
    char* name = tk_strdup(vmodel);
    char* ext_name = strrchr(name, '.');
    if (ext_name != NULL) {
      *ext_name = '\0';
      view_model = view_model_factory_create_model(name, req);
      if (view_model == NULL) {
        *ext_name = '.';
        view_model = view_model_factory_create_model(ext_name, req);
      }
      TKMEM_FREE(name);
      return_value_if_fail(view_model != NULL, NULL);
    } else {
      view_model = view_model_factory_create_model(name, req);
      TKMEM_FREE(name);
      return_value_if_fail(view_model != NULL, NULL);
    }
  }

  return view_model;
}

#define IS_COMPOSITOR_VIEW_MODEL(type) strchr(type, '+') != NULL

static view_model_t* binding_context_awtk_create_view_model(widget_t* widget,
                                                            binding_context_t* parent_ctx,
                                                            navigator_request_t* req) {
  view_model_t* view_model = NULL;
  widget_t* win = widget_get_window(widget);
  const char* vmodel = widget_get_prop_vmodel(widget);
  view_model_t* parent_view_model = parent_ctx != NULL ? parent_ctx->view_model : NULL;

  if (vmodel != NULL) {
    if (req != NULL) {
      object_set_prop_pointer(OBJECT(req), NAVIGATOR_ARG_VIEW, widget);
    }

    if (IS_COMPOSITOR_VIEW_MODEL(vmodel)) {
      tokenizer_t t;
      view_model_t* compositor = view_model_compositor_create(req);
      return_value_if_fail(compositor != NULL, NULL);

      tokenizer_init(&t, vmodel, strlen(vmodel), "+");
      while (tokenizer_has_more(&t)) {
        const char* type1 = tokenizer_next(&t);
        view_model_t* vm =
            binding_context_awtk_create_view_model_one(parent_view_model, type1, req);
        if (vm != NULL) {
          if (view_model_compositor_add(compositor, vm) != RET_OK) {
            log_warn("view_model_compositor_add failed\n");
            OBJECT_UNREF(vm);
          }
        } else {
          log_warn("create %s view_model failed\n", type1);
        }
      }
      tokenizer_deinit(&t);
      view_model = compositor;
    } else {
      view_model = binding_context_awtk_create_view_model_one(parent_view_model, vmodel, req);
    }
  }

  if (view_model == NULL) {
    if (vmodel != NULL) {
      log_warn("%s not found view_model %s\n", __FUNCTION__, vmodel);
    }
    view_model = view_model_dummy_create(req);
  }

  widget_on(win, EVT_DESTROY, view_model_on_window_destroy, view_model);
  widget_on(win, EVT_WINDOW_CLOSE, view_model_on_window_close, view_model);

  return view_model;
}

static ret_t binding_context_awtk_bind_widget(binding_context_t* ctx, widget_t* widget) {
  view_model_t* view_model = NULL;
  const char* type = widget_get_type(widget);
  const char* vmodel = widget_get_prop_vmodel(widget);

  if (vmodel != NULL && ctx->widget != widget) {
    return binding_context_bind_for_widget(widget, ctx, ctx->navigator_request);
  } else {
    view_model = ctx->view_model;
  }

  if (custom_binder_exist(type)) {
    return custom_binder_bind(type, widget, ctx);
  }

  if (view_model != NULL) {
    if (widget->custom_props != NULL) {
      ctx->current_widget = widget;
      object_foreach_prop(widget->custom_props, visit_bind_one_prop, ctx);
    }
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  binding_context_awtk_bind_widget(ctx, iter);
  WIDGET_FOR_EACH_CHILD_END();

  if (vmodel != NULL) {
    view_model_on_mount(view_model);
    emitter_on(EMITTER(view_model), EVT_PROP_CHANGED, on_view_model_prop_change, ctx);
    emitter_on(EMITTER(view_model), EVT_PROPS_CHANGED, on_view_model_prop_change, ctx);
  }

  return RET_OK;
}

static ret_t widget_trim_children(binding_context_t* ctx, widget_t* widget, uint32_t nr) {
  int32_t i = 0;
  int32_t real_nr = widget_count_children(widget);

  widget->need_relayout_children = TRUE;
  if (real_nr <= nr || widget->children == NULL) {
    return RET_OK;
  }

  for (i = nr; i < real_nr; i++) {
    widget_t* child = WIDGET(widget->children->elms[i]);

    child->parent = NULL;
    if (widget->target == child) {
      widget->target = NULL;
    }

    if (widget->grab_widget == child) {
      widget->grab_widget = NULL;
    }

    if (widget->key_target == child) {
      widget->key_target = NULL;
    }

    binding_context_put_widget(ctx, child);
  }

  widget->children->size = nr;

  return RET_OK;
}

static ret_t binding_context_on_container_destroy(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* template_widget = WIDGET(widget_get_prop_pointer(widget, WIDGET_PROP_TEMPLATE_WIDGET));
  widget_destroy(template_widget);

  return RET_REMOVE;
}

static ret_t binding_context_prepare_children(binding_context_t* ctx, widget_t* widget) {
  uint32_t i = 0;
  view_model_t* view_model = ctx->view_model;
  uint32_t items = object_get_prop_int(OBJECT(view_model), VIEW_MODEL_PROP_ITEMS, 0);
  widget_t* template_widget = WIDGET(widget_get_prop_pointer(widget, WIDGET_PROP_TEMPLATE_WIDGET));

  if (template_widget == NULL) {
    template_widget = widget_get_child(widget, 0);
    widget_set_prop_pointer(widget, WIDGET_PROP_TEMPLATE_WIDGET, template_widget);
    widget_on(widget, EVT_DESTROY, binding_context_on_container_destroy, widget);

    widget_remove_child(widget, template_widget);
  }

  widget_trim_children(ctx, widget, items);
  for (i = widget_count_children(widget); i < items; i++) {
    widget_add_child(widget, binding_context_get_widget(ctx, widget));
  }
  return_value_if_fail(items == widget_count_children(widget), RET_OOM);

  return RET_OK;
}

static bool_t widget_is_for_items(widget_t* widget) {
  value_t v;

  value_set_bool(&v, FALSE);
  widget_get_prop(widget, WIDGET_PROP_V_FOR_ITEMS, &v);

  return value_bool(&v);
}

static ret_t on_reset_emitter(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);
  if (widget->emitter != NULL) {
    widget_off_by_tag(widget, EVENT_TAG);
  }

  return RET_OK;
}

static ret_t binding_context_awtk_bind_widget_array(binding_context_t* ctx, widget_t* widget) {
  const char* type = widget_get_type(widget);
  view_model_t* view_model = ctx->view_model;

  ctx->request_rebind = 0;
  return_value_if_fail(object_is_collection(OBJECT(view_model)), RET_BAD_PARAMS);

  if (widget->custom_props != NULL) {
    ctx->current_widget = widget;
    object_foreach_prop(widget->custom_props, visit_bind_one_prop, ctx);
  }

  if (custom_binder_exist(type)) {
    return custom_binder_bind(type, widget, ctx);
  }

  if (widget_is_for_items(widget)) {
    return_value_if_fail(binding_context_prepare_children(ctx, widget) == RET_OK, RET_FAIL);

    view_model_array_set_cursor(view_model, 0);
    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    binding_context_awtk_bind_widget_array(ctx, iter);
    view_model_array_inc_cursor(view_model);
    WIDGET_FOR_EACH_CHILD_END();
    widget_layout(widget);
  } else {
    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    binding_context_awtk_bind_widget_array(ctx, iter);
    WIDGET_FOR_EACH_CHILD_END();
  }

  return RET_OK;
}

static ret_t binding_context_rebind_in_idle(const idle_info_t* info) {
  binding_context_t* ctx = BINDING_CONTEXT(info->ctx);

  log_debug("start_rebind\n");
  binding_context_clear_bindings(ctx);
  widget_foreach(ctx->widget, on_reset_emitter, NULL);
  binding_context_awtk_bind_widget_array(ctx, ctx->widget);
  binding_context_update_to_view(ctx);
  log_debug("end_rebind\n");

  return RET_REMOVE;
}

static ret_t binding_context_on_rebind(void* c, event_t* e) {
  binding_context_t* ctx = BINDING_CONTEXT(c);

  if (ctx->request_rebind == 0) {
    idle_add(binding_context_rebind_in_idle, ctx);
  }

  ctx->request_rebind++;
  if (ctx->update_view_idle_id != TK_INVALID_ID) {
    idle_remove(ctx->update_view_idle_id);
    ctx->update_view_idle_id = TK_INVALID_ID;
    ctx->updating_view = FALSE;
  }

  return RET_OK;
}

static ret_t binding_context_awtk_do_bind(binding_context_t* ctx, void* widget) {
  ret_t ret = RET_OK;

  if (object_is_collection(OBJECT(ctx->view_model))) {
    ret = binding_context_awtk_bind_widget_array(ctx, WIDGET(widget));

    if (!emitter_exist(EMITTER(ctx->view_model), EVT_PROP_CHANGED, on_view_model_prop_change,
                       ctx)) {
      emitter_on(EMITTER(ctx->view_model), EVT_PROP_CHANGED, on_view_model_prop_change, ctx);
      emitter_on(EMITTER(ctx->view_model), EVT_PROPS_CHANGED, on_view_model_prop_change, ctx);
      emitter_on(EMITTER(ctx->view_model), EVT_ITEMS_CHANGED, binding_context_on_rebind, ctx);
    }
  } else {
    ret = binding_context_awtk_bind_widget(ctx, WIDGET(widget));
  }

  return_value_if_fail(ret == RET_OK, RET_FAIL);
  return_value_if_fail(binding_context_update_to_view(ctx) == RET_OK, RET_FAIL);
  ctx->bound = TRUE;

  return RET_OK;
}

static ret_t binding_context_awtk_bind(binding_context_t* ctx, void* widget) {
  return binding_context_awtk_do_bind(ctx, WIDGET(widget));
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

static ret_t visit_data_binding_update_to_view(void* ctx, const void* data) {
  value_t v;
  data_binding_t* rule = DATA_BINDING(data);
  widget_t* widget = WIDGET(BINDING_RULE(rule)->widget);
  binding_context_t* bctx = BINDING_RULE(rule)->binding_context;

  value_set_int(&v, 0);
  if (tk_str_start_with(rule->path, DATA_BINDING_ERROR_OF)) {
    return RET_OK;
  }

  if ((rule->mode == BINDING_ONCE && !(bctx->bound)) || rule->mode == BINDING_ONE_WAY ||
      rule->mode == BINDING_TWO_WAY) {
    if (data_binding_get_prop(rule, &v) == RET_OK) {
      ENSURE(widget_set_prop_if_diff(widget, rule->prop, &v, !bctx->bound) != RET_FAIL);
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

static ret_t visit_command_binding(void* ctx, const void* data) {
  command_binding_t* rule = COMMAND_BINDING(data);
  widget_t* widget = WIDGET(BINDING_RULE(rule)->widget);

  if (rule->auto_disable && !widget_is_window(widget)) {
    bool_t can_exec = command_binding_can_exec(rule);
    widget_set_enable(widget, can_exec);
  }

  return RET_OK;
}

static ret_t widget_visit_data_binding_update_to_view(void* ctx, const void* rule) {
  widget_t* widget = WIDGET(ctx);
  widget_t* iter = WIDGET(BINDING_RULE(rule)->widget);

  if (widget_is_parent_of(widget, iter)) {
    visit_data_binding_update_to_view(NULL, rule);
  }

  return RET_OK;
}

static ret_t widget_visit_command_binding(void* ctx, const void* rule) {
  widget_t* widget = WIDGET(ctx);
  widget_t* iter = WIDGET(BINDING_RULE(rule)->widget);

  if (widget_is_parent_of(widget, iter)) {
    visit_command_binding(NULL, rule);
  }

  return RET_OK;
}

static ret_t binding_context_awtk_update_widget(binding_context_t* ctx, void* widget) {
  darray_foreach(&(ctx->data_bindings), widget_visit_data_binding_update_to_view, widget);
  darray_foreach(&(ctx->command_bindings), widget_visit_command_binding, widget);

  return RET_OK;
}

static ret_t binding_context_awtk_update_to_view_sync(binding_context_t* ctx) {
  darray_foreach(&(ctx->data_bindings), visit_data_binding_update_to_view, NULL);
  darray_foreach(&(ctx->command_bindings), visit_command_binding, NULL);
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
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

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

static ret_t visit_data_binding_update_to_model(void* ctx, const void* data) {
  value_t v;
  data_binding_t* rule = DATA_BINDING(data);
  widget_t* widget = WIDGET(BINDING_RULE(rule)->widget);

  if (rule->trigger == UPDATE_WHEN_EXPLICIT) {
    if (rule->mode == BINDING_TWO_WAY || rule->mode == BINDING_ONE_WAY_TO_VIEW_MODEL) {
      return_value_if_fail(widget_get_prop(widget, rule->prop, &v) == RET_OK, RET_OK);
      return_value_if_fail(data_binding_set_prop(rule, &v) == RET_OK, RET_OK);
    }
  }

  return RET_OK;
}

static ret_t binding_context_awtk_update_to_model(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  darray_foreach(&(ctx->data_bindings), visit_data_binding_update_to_model, ctx);

  return RET_OK;
}

static ret_t binding_context_awtk_destroy(binding_context_t* ctx) {
  darray_deinit(&(ctx->cache_widgets));
  if (ctx->update_view_idle_id != TK_INVALID_ID) {
    idle_remove(ctx->update_view_idle_id);
    ctx->update_view_idle_id = TK_INVALID_ID;
  }
  memset(ctx, 0x00, sizeof(*ctx));
  TKMEM_FREE(ctx);

  return RET_OK;
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

static bool_t binding_context_awtk_can_exec(binding_context_t* ctx, const char* cmd,
                                            const char* args) {
  if (tk_str_ieq(COMMAND_BINDING_CMD_SEND_KEY, cmd)) {
    return TRUE;
  } else if (tk_str_ieq(COMMAND_BINDING_CMD_SET_WIDGET_PROP, cmd)) {
    return TRUE;
  }

  return FALSE;
}

static const binding_context_vtable_t s_binding_context_vtable = {
    .update_to_view = binding_context_awtk_update_to_view,
    .update_to_model = binding_context_awtk_update_to_model,
    .exec = binding_context_awtk_exec,
    .bind = binding_context_awtk_bind,
    .update_widget = binding_context_awtk_update_widget,
    .can_exec = binding_context_awtk_can_exec,
    .destroy = binding_context_awtk_destroy};

binding_context_t* binding_context_awtk_create(widget_t* widget, binding_context_t* parent_ctx,
                                               navigator_request_t* req) {
  view_model_t* view_model = NULL;
  binding_context_t* ctx = NULL;
  return_value_if_fail(widget != NULL && req != NULL, NULL);

  view_model = binding_context_awtk_create_view_model(widget, parent_ctx, req);

  if (view_model != NULL) {
    ctx = TKMEM_ZALLOC(binding_context_t);
    if (ctx != NULL) {
      ctx->widget = widget;
      ctx->vt = &s_binding_context_vtable;

      if (binding_context_init(ctx, req, view_model) == RET_OK) {
        view_model_on_will_mount(view_model, req);
      } else {
        binding_context_destroy(ctx);
        ctx = NULL;
      }
    }
    object_unref(OBJECT(view_model));
  }

  if (ctx != NULL) {
    darray_init(&(ctx->cache_widgets), 10, (tk_destroy_t)widget_unref, NULL);
    binding_context_set_parent(ctx, parent_ctx);
  }

  return ctx;
}

static ret_t binding_context_destroy_async(const idle_info_t* info) {
  binding_context_destroy((binding_context_t*)(info->ctx));

  return RET_REMOVE;
}

static ret_t binding_context_on_widget_destroy(void* ctx, event_t* e) {
  binding_context_t* bctx = BINDING_CONTEXT(ctx);

  binding_context_set_parent(ctx, NULL);
  idle_add(binding_context_destroy_async, ctx);
  if (bctx->update_view_idle_id != TK_INVALID_ID) {
    idle_remove(bctx->update_view_idle_id);
    bctx->update_view_idle_id = TK_INVALID_ID;
  }

  return RET_REMOVE;
}

static ret_t binding_context_bind_for_widget(widget_t* widget, binding_context_t* parent_ctx,
                                             navigator_request_t* req) {
  binding_context_t* ctx = NULL;
  return_value_if_fail(widget != NULL && req != NULL, RET_BAD_PARAMS);

  ctx = binding_context_awtk_create(widget, parent_ctx, req);
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(binding_context_awtk_do_bind(ctx, widget) == RET_OK);
  widget_on(widget, EVT_DESTROY, binding_context_on_widget_destroy, ctx);

  return RET_OK;
error:
  binding_context_destroy(ctx);

  return RET_FAIL;
}

ret_t binding_context_bind_for_window(widget_t* widget, navigator_request_t* req) {
  return binding_context_bind_for_widget(widget, NULL, req);
}

static ret_t window_dump(widget_t* win) {
  value_t v;

  value_set_bool(&v, FALSE);
  if (widget_get_prop(win, "debug", &v) == RET_OK && value_bool(&v)) {
    str_t str;
    str_init(&str, 100000);
    widget_to_xml(win, &str);
    log_debug("%s\n", str.str);
    str_reset(&str);
  }

  return RET_OK;
}

ret_t awtk_open_window(navigator_request_t* req) {
  widget_t* win = NULL;
  return_value_if_fail(req != NULL && req->target != NULL, RET_BAD_PARAMS);

  if (req->close_current) {
    widget_t* current = window_manager_get_top_main_window(window_manager());
    log_debug("close current window: %s\n", current->name);
    win = window_open_and_close(req->target, current);
  } else {
    win = window_open(req->target);
  }
  return_value_if_fail(win != NULL, RET_NOT_FOUND);

  binding_context_bind_for_window(win, req);
  window_dump(win);

  return RET_OK;
}
