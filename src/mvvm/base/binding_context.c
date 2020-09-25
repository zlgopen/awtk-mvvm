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
#include "mvvm/base/navigator.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/command_binding.h"

ret_t binding_context_init(binding_context_t* ctx, navigator_request_t* req, view_model_t* vm) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  darray_init(&(ctx->command_bindings), 10, (tk_destroy_t)object_unref,
              (tk_compare_t)object_compare);
  darray_init(&(ctx->data_bindings), 10, (tk_destroy_t)object_unref, (tk_compare_t)object_compare);

  if (req != NULL) {
    object_ref(OBJECT(req));
    ctx->navigator_request = req;
  }

  if (vm != NULL) {
    object_ref(OBJECT(vm));
    ctx->view_model = vm;
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

ret_t binding_context_set_parent(binding_context_t* ctx, binding_context_t* parent) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  if (ctx->parent != NULL) {
    view_model_t* view_model = ctx->view_model;
    view_model_t* parent_view_model = ctx->parent->view_model;

    if (parent_view_model != NULL) {
      emitter_off_by_ctx(EMITTER(parent_view_model), view_model);
      OBJECT_UNREF(parent_view_model);
    }
    ctx->parent = NULL;
    view_model->parent = NULL;
  }

  ctx->parent = parent;
  if (parent != NULL) {
    view_model_t* view_model = ctx->view_model;

    view_model->parent = parent->view_model;
    if (view_model->parent != NULL) {
      object_ref(OBJECT(view_model->parent));
      emitter_on(EMITTER(view_model->parent), EVT_PROP_CHANGED, emitter_forward, view_model);
      emitter_on(EMITTER(view_model->parent), EVT_PROPS_CHANGED, emitter_forward, view_model);
    }
  }

  return RET_OK;
}

ret_t binding_context_destroy(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  darray_deinit(&(ctx->data_bindings));
  darray_deinit(&(ctx->command_bindings));
  binding_context_set_parent(ctx, NULL);

  if (ctx->navigator_request != NULL) {
    OBJECT_UNREF(ctx->navigator_request);
  }

  if (ctx->view_model != NULL) {
    OBJECT_UNREF(ctx->view_model);
  }

  if (ctx->vt->destroy != NULL) {
    ctx->vt->destroy(ctx);
  }

  return RET_OK;
}

ret_t binding_context_clear_bindings(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  darray_clear(&(ctx->data_bindings));
  darray_clear(&(ctx->command_bindings));

  return RET_OK;
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
      tk_str_ieq(cmd, COMMAND_BINDING_CMD_NAVIGATE) || tk_str_ieq(cmd, COMMAND_BINDING_CMD_DEBUG)) {
    return TRUE;
  }

  if (ctx->vt->can_exec != NULL) {
    return ctx->vt->can_exec(ctx, cmd, args);
  }

  return FALSE;
}

ret_t binding_context_bind(binding_context_t* ctx, void* widget) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->bind != NULL, RET_BAD_PARAMS);

  return ctx->vt->bind(ctx, widget);
}

ret_t binding_context_update_widget(binding_context_t* ctx, void* widget) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->update_widget != NULL,
                       RET_BAD_PARAMS);

  return ctx->vt->update_widget(ctx, widget);
}
