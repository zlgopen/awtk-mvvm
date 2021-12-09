/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
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
 * 2019-01-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/darray.h"
#include "tkc/tokenizer.h"
#include "base/idle.h"
#include "mvvm/base/navigator.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/command_binding.h"

static int32_t bindings_node_compare(const darray_t* node, const void* widget) {
  binding_rule_t* rule = NULL;
  return_value_if_fail(node != NULL && node->size > 0 && widget != NULL, -1);
  rule = BINDING_RULE(node->elms[0]);
  return (uint8_t*)(rule->widget) - (uint8_t*)(widget);
}

static ret_t binding_context_init_bindings(binding_context_t* ctx) {
  tk_destroy_t destroy = (tk_destroy_t)darray_destroy;
  tk_compare_t compare = (tk_compare_t)bindings_node_compare;
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  slist_init(&(ctx->data_bindings), destroy, compare);
  slist_init(&(ctx->command_bindings), destroy, compare);
  slist_init(&(ctx->dynamic_bindings), destroy, compare);

  return RET_OK;
}

static ret_t binding_context_push_binding_rule(binding_context_t* ctx, slist_t* bindings,
                                               binding_rule_t* rule) {
  darray_t* node = NULL;
  return_value_if_fail(rule != NULL && rule->widget != NULL, RET_BAD_PARAMS);

  if (bindings->first != NULL) {
    node = (darray_t*)(slist_find(bindings, rule->widget));
  }

  if (node == NULL) {
    node = darray_create(4, (tk_destroy_t)tk_object_unref, (tk_compare_t)tk_object_compare);
    return_value_if_fail(node != NULL, RET_OOM);
    return_value_if_fail(slist_append(bindings, node) == RET_OK, RET_FAIL);
  }

  if (darray_push(node, rule) == RET_OK) {
    rule->binding_context = ctx;
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t binding_context_push_data_binding(binding_context_t* ctx, binding_rule_t* rule) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  return binding_context_push_binding_rule(ctx, &(ctx->data_bindings), rule);
}

static ret_t binding_context_push_command_binding(binding_context_t* ctx, binding_rule_t* rule) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  return binding_context_push_binding_rule(ctx, &(ctx->command_bindings), rule);
}

static ret_t binding_context_push_dynamic_binding(binding_context_t* ctx, binding_rule_t* rule) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  return binding_context_push_binding_rule(ctx, &(ctx->dynamic_bindings), rule);
}

ret_t binding_context_init(binding_context_t* ctx, navigator_request_t* req) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  binding_context_init_bindings(ctx);

  str_init(&(ctx->temp), 0);

  if (req != NULL) {
    tk_object_ref(TK_OBJECT(req));
    ctx->navigator_request = req;
  }

  return RET_OK;
}

ret_t binding_context_destroy(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  if (ctx->vt->destroy != NULL) {
    ctx->vt->destroy(ctx);
  }

  binding_context_set_parent(ctx, NULL);
  binding_context_set_view_model(ctx, NULL);
  binding_context_clear_bindings(ctx);

  if (ctx->navigator_request != NULL) {
    TK_OBJECT_UNREF(ctx->navigator_request);
  }

  str_reset(&(ctx->temp));

  memset(ctx, 0x00, sizeof(*ctx));
  TKMEM_FREE(ctx);

  return RET_OK;
}

binding_context_t* binding_context_get_root(binding_context_t* ctx) {
  binding_context_t* root = ctx;
  return_value_if_fail(ctx != NULL, NULL);

  while (root->parent != NULL) {
    root = root->parent;
  }

  return root;
}

static ret_t binding_context_on_prop_changed(void* ctx, event_t* e) {
  binding_context_t* bctx = BINDING_CONTEXT(ctx);
  binding_context_update_to_view(bctx);
  (void)e;
  return RET_OK;
}

static ret_t binding_context_on_items_changed(void* c, event_t* e) {
  binding_context_t* bctx = BINDING_CONTEXT(c);
  tk_object_t* target = TK_OBJECT(e->target);
  if (target == NULL) {
    target = TK_OBJECT(bctx->view_model);
  }
  binding_context_notify_items_changed(bctx, target);
  return RET_OK;
}

ret_t binding_context_set_view_model(binding_context_t* ctx, view_model_t* vm) {
  view_model_t* view_model = NULL;
  view_model_t* parent_view_model = NULL;
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  if (ctx->parent != NULL) {
    parent_view_model = ctx->parent->view_model;
  }

  view_model = ctx->view_model;

  if (view_model != NULL) {
    if (parent_view_model != NULL) {
      emitter_off_by_ctx(EMITTER(parent_view_model), view_model);
    }
    view_model->parent = NULL;
    emitter_off_by_ctx(EMITTER(view_model), ctx);
    view_model_on_unmount(view_model);
    TK_OBJECT_UNREF(view_model);
  }

  view_model = ctx->view_model = vm;

  if (view_model != NULL) {
    tk_object_ref(TK_OBJECT(view_model));
    view_model_on_will_mount(view_model, ctx->navigator_request);

    emitter_on(EMITTER(view_model), EVT_ITEMS_CHANGED, binding_context_on_items_changed, ctx);
    emitter_on(EMITTER(view_model), EVT_PROPS_CHANGED, binding_context_on_prop_changed, ctx);
    emitter_on(EMITTER(view_model), EVT_PROP_CHANGED, binding_context_on_prop_changed, ctx);

    view_model->parent = parent_view_model;
    if (parent_view_model != NULL) {
      tk_object_ref(TK_OBJECT(parent_view_model));
      emitter_on(EMITTER(parent_view_model), EVT_ITEMS_CHANGED, emitter_forward, view_model);
      emitter_on(EMITTER(parent_view_model), EVT_PROPS_CHANGED, emitter_forward, view_model);
      emitter_on(EMITTER(parent_view_model), EVT_PROP_CHANGED, emitter_forward, view_model);
    }
  } else {
    if (parent_view_model != NULL) {
      tk_object_ref(TK_OBJECT(parent_view_model));
    }
  }

  return RET_OK;
}

ret_t binding_context_set_bound(binding_context_t* ctx, bool_t bound) {
  view_model_t* view_model = NULL;
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  view_model = ctx->view_model;

  if (ctx->bound && !bound) {
    if (view_model != NULL) {
      view_model_on_will_unmount(view_model);
    }
    ctx->bound = bound;
  } else if (!ctx->bound && bound) {
    if (view_model != NULL) {
      binding_context_update_to_view(ctx);
      view_model_on_mount(view_model);
    }
    ctx->bound = bound;
  }

  return RET_OK;
}

ret_t binding_context_set_parent(binding_context_t* ctx, binding_context_t* parent) {
  view_model_t* view_model;
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  view_model = ctx->view_model;

  if (ctx->parent != NULL) {
    view_model_t* parent_view_model = ctx->parent->view_model;

    if (view_model != NULL) {
      if (parent_view_model != NULL) {
        emitter_off_by_ctx(EMITTER(parent_view_model), view_model);
        TK_OBJECT_UNREF(parent_view_model);
      }
      view_model->parent = NULL;
    }
    ctx->parent = NULL;
  }

  ctx->parent = parent;

  if (parent != NULL) {
    view_model_t* parent_view_model = parent->view_model;

    if (view_model != NULL) {
      view_model->parent = parent_view_model;
      if (parent_view_model != NULL) {
        tk_object_ref(TK_OBJECT(parent_view_model));
        emitter_on(EMITTER(parent_view_model), EVT_ITEMS_CHANGED, emitter_forward, view_model);
        emitter_on(EMITTER(parent_view_model), EVT_PROPS_CHANGED, emitter_forward, view_model);
        emitter_on(EMITTER(parent_view_model), EVT_PROP_CHANGED, emitter_forward, view_model);
      }
    }
  }

  return RET_OK;
}

ret_t binding_context_update_to_view(binding_context_t* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->update_to_view != NULL,
                       RET_BAD_PARAMS);

  if (!ctx->updating_view) {
    ret = ctx->vt->update_to_view(ctx);
  }

  return ret;
}

ret_t binding_context_update_to_model(binding_context_t* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->update_to_model != NULL,
                       RET_BAD_PARAMS);

  if (ctx->updating_model) {
    return RET_BUSY;
  }

  ctx->updating_model = TRUE;
  ret = ctx->vt->update_to_model(ctx);
  ctx->updating_model = FALSE;

  return ret;
}

ret_t binding_context_exec(binding_context_t* ctx, const char* cmd, const char* args) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && cmd != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(cmd, COMMAND_BINDING_CMD_NOTHING)) {
    return RET_OK;
  } else if (tk_str_ieq(cmd, COMMAND_BINDING_CMD_DEBUG)) {
    log_debug("debug:%s\n", args);
    return RET_OK;
  } else if (tk_str_ieq(cmd, COMMAND_BINDING_CMD_NAVIGATE)) {
    return navigator_to(args);
  }

  if (ctx->vt->exec != NULL) {
    return ctx->vt->exec(ctx, cmd, args);
  }

  return RET_NOT_IMPL;
}

bool_t binding_context_can_exec(binding_context_t* ctx, const char* cmd, const char* args) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && cmd != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(cmd, COMMAND_BINDING_CMD_NOTHING) ||
      tk_str_ieq(cmd, COMMAND_BINDING_CMD_FSCRIPT) ||
      tk_str_ieq(cmd, COMMAND_BINDING_CMD_NAVIGATE) || tk_str_ieq(cmd, COMMAND_BINDING_CMD_DEBUG)) {
    return TRUE;
  }

  if (ctx->vt->can_exec != NULL) {
    return ctx->vt->can_exec(ctx, cmd, args);
  }

  return FALSE;
}

ret_t binding_context_clear_bindings(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  slist_remove_all(&(ctx->data_bindings));
  slist_remove_all(&(ctx->command_bindings));
  slist_remove_all(&(ctx->dynamic_bindings));

  return RET_OK;
}

ret_t binding_context_clear_bindings_of_widget(binding_context_t* ctx, void* widget) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && widget != NULL, RET_BAD_PARAMS);

  slist_remove(&(ctx->data_bindings), widget);
  slist_remove(&(ctx->command_bindings), widget);
  slist_remove(&(ctx->dynamic_bindings), widget);

  return RET_OK;
}

ret_t binding_context_bind_data(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->bind_data != NULL, RET_BAD_PARAMS);

  ret = ctx->vt->bind_data(ctx, rule);
  if (ret == RET_OK) {
    ret = binding_context_push_data_binding(ctx, rule);
  }

  return ret;
}

ret_t binding_context_bind_command(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->bind_command != NULL, RET_BAD_PARAMS);

  ret = ctx->vt->bind_command(ctx, rule);
  if (ret == RET_OK) {
    ret = binding_context_push_command_binding(ctx, rule);
  }

  return ret;
}

ret_t binding_context_bind_condition(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->bind_conditon != NULL, RET_BAD_PARAMS);

  ret = ctx->vt->bind_conditon(ctx, rule);
  if (ret == RET_OK) {
    ret = binding_context_push_dynamic_binding(ctx, rule);
  }

  return ret;
}

ret_t binding_context_bind_items(binding_context_t* ctx, binding_rule_t* rule) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->bind_items != NULL, RET_BAD_PARAMS);

  ret = ctx->vt->bind_items(ctx, rule);
  if (ret == RET_OK) {
    ret = binding_context_push_dynamic_binding(ctx, rule);
  }

  return ret;
}

static ret_t binding_context_notify_items_changed_internal(binding_context_t* ctx,
                                                           tk_object_t* items, bool_t sync) {
  return_value_if_fail(ctx != NULL && items != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->notify_items_changed != NULL, RET_BAD_PARAMS);

  if (ctx->bound) {
    return ctx->vt->notify_items_changed(ctx, items, sync);
  }
  return RET_OK;
}

ret_t binding_context_notify_items_changed(binding_context_t* ctx, tk_object_t* items) {
  return binding_context_notify_items_changed_internal(ctx, items, FALSE);
}

ret_t binding_context_notify_items_changed_sync(binding_context_t* ctx, tk_object_t* items) {
  return binding_context_notify_items_changed_internal(ctx, items, TRUE);
}

uint32_t binding_context_get_items_cursor_of_rule(binding_context_t* ctx, binding_rule_t* rule) {
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->get_items_cursor_of_rule != NULL,
                       RET_BAD_PARAMS);

  return ctx->vt->get_items_cursor_of_rule(ctx, rule);
}

uint32_t binding_context_calc_widget_index_of_rule(binding_context_t* ctx, binding_rule_t* rule) {
  return_value_if_fail(ctx != NULL && rule != NULL, RET_BAD_PARAMS);
  return_value_if_fail(ctx->vt != NULL && ctx->vt->calc_widget_index_of_rule != NULL,
                       RET_BAD_PARAMS);

  return ctx->vt->calc_widget_index_of_rule(ctx, rule);
}

const char* binding_context_resolve_path_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                                 const char* path, bool_t* is_cursor) {
  return_value_if_fail(ctx->vt != NULL && ctx->vt->resolve_path_by_rule != NULL, NULL);

  return ctx->vt->resolve_path_by_rule(ctx, rule, path, is_cursor);
}

ret_t binding_context_get_prop_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                       const char* name, value_t* v) {
  view_model_t* view_model;
  bool_t is_cursor = FALSE;
  return_value_if_fail(ctx != NULL && ctx->view_model != NULL, RET_BAD_PARAMS);

  view_model = ctx->view_model;
  if (name == NULL || *name == '\0') {
    value_set_object(v, TK_OBJECT(view_model));
    return RET_OK;
  }

  name = binding_context_resolve_path_by_rule(ctx, rule, name, &is_cursor);
  if (is_cursor) {
    value_set_uint32(v, tk_atoi(name));
    return RET_OK;
  } else {
    return view_model_get_prop(view_model, name, v);
  }
}

ret_t binding_context_set_prop_by_rule(binding_context_t* ctx, binding_rule_t* rule,
                                       const char* name, const value_t* v) {
  view_model_t* view_model;
  bool_t is_cursor = FALSE;
  return_value_if_fail(ctx != NULL && ctx->view_model != NULL && name != NULL, RET_BAD_PARAMS);

  view_model = ctx->view_model;
  name = binding_context_resolve_path_by_rule(ctx, rule, name, &is_cursor);
  if (is_cursor) {
    return RET_FAIL;
  } else {
    return view_model_set_prop(view_model, name, v);
  }
}
