/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "mvvm/base/binding_context.h"

ret_t binding_context_init(binding_context_t* ctx, navigator_request_t* navigator_request) {
  return_value_if_fail(ctx != NULL, RET_BAD_PARAMS);

  darray_init(&(ctx->command_bindings), 10, (tk_destroy_t)object_unref,
              (tk_compare_t)object_compare);
  darray_init(&(ctx->data_bindings), 10, (tk_destroy_t)object_unref, (tk_compare_t)object_compare);

  if (navigator_request != NULL) {
    ctx->navigator_request = navigator_request;
    object_ref(OBJECT(ctx->navigator_request));
  }

  return RET_OK;
}

ret_t binding_context_update_to_view(binding_context_t* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(ctx != NULL && ctx->vt != NULL && ctx->vt->update_to_view != NULL,
                       RET_BAD_PARAMS);

  if (ctx->updating_view) {
    return RET_BUSY;
  }

  ctx->updating_view = TRUE;
  ret = ctx->vt->update_to_view(ctx);
  ctx->updating_view = FALSE;

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

ret_t binding_context_destroy(binding_context_t* ctx) {
  return_value_if_fail(ctx != NULL && ctx->vt != NULL, RET_BAD_PARAMS);

  darray_deinit(&(ctx->data_bindings));
  darray_deinit(&(ctx->command_bindings));

  if (ctx->navigator_request != NULL) {
    object_unref(OBJECT(ctx->navigator_request));
  }
  
  if (ctx->view_model != NULL) {
    object_unref(OBJECT(ctx->view_model));
  }

  if (ctx->vt->destroy != NULL) {
    ctx->vt->destroy(ctx);
  }

  return RET_OK;
}
