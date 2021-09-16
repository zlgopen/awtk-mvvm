/**
 * File:   view_model_compositor.c
 * Author: AWTK Develop Team
 * Brief:  view_model_compositor
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
 * 2020-06-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_compositor.h"

static ret_t view_model_compositor_on_destroy(object_t* obj) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(obj);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    emitter_off_by_ctx(EMITTER(iter), compositor);
    OBJECT_UNREF(iter);
  }
  darray_deinit(&(compositor->view_models));

  return RET_OK;
}

static int32_t view_model_compositor_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_compositor_set_prop(object_t* obj, const char* name, const value_t* v) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(obj);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    if (view_model_set_prop(iter, name, v) == RET_OK) {
      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

static ret_t view_model_compositor_get_prop(object_t* obj, const char* name, value_t* v) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(obj);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    if (view_model_get_prop(iter, name, v) == RET_OK) {
      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

static bool_t view_model_compositor_can_exec(object_t* obj, const char* name, const char* args) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(obj);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    if (view_model_can_exec(iter, name, args)) {
      return TRUE;
    }
  }

  return FALSE;
}

static ret_t view_model_compositor_exec(object_t* obj, const char* name, const char* args) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(obj);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    ret_t ret = view_model_exec(iter, name, args);
    if (ret != RET_NOT_FOUND && ret != RET_NOT_IMPL) {
      return ret;
    }
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_object_vtable_compositor = {
    .type = "view_model_compositor",
    .desc = "view_model_compositor",
    .size = sizeof(view_model_compositor_t),
    .is_collection = FALSE,
    .on_destroy = view_model_compositor_on_destroy,
    .compare = view_model_compositor_compare,
    .get_prop = view_model_compositor_get_prop,
    .set_prop = view_model_compositor_set_prop,
    .can_exec = view_model_compositor_can_exec,
    .exec = view_model_compositor_exec};

static ret_t view_model_compositor_on_will_mount(view_model_t* view_model,
                                                 navigator_request_t* req) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    view_model_on_will_mount(iter, req);
  }

  return RET_OK;
}

static ret_t view_model_compositor_on_mount(view_model_t* view_model) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    view_model_on_mount(iter);
  }

  return RET_OK;
}

static ret_t view_model_compositor_on_will_unmount(view_model_t* view_model) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    view_model_on_will_unmount(iter);
  }

  return RET_OK;
}

static ret_t view_model_compositor_on_unmount(view_model_t* view_model) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    view_model_on_unmount(iter);
  }

  return RET_OK;
}

static view_model_t* view_model_compositor_create_sub_view_model(view_model_t* view_model,
                                                                 const char* name) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    view_model_t* sub = view_model_create_sub_view_model(iter, name);
    if (sub != NULL) {
      return sub;
    }
  }

  return NULL;
}

static view_model_t* view_model_compositor_create_sub_view_model_array(view_model_t* view_model,
                                                                       const char* name) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  view_model_t** all = (view_model_t**)(compositor->view_models.elms);
  uint32_t nr = compositor->view_models.size;
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    view_model_t* iter = all[i];
    view_model_t* sub = view_model_create_sub_view_model_array(iter, name);
    if (sub != NULL) {
      return sub;
    }
  }

  return NULL;
}

static const view_model_vtable_t s_view_model_vtable_compositor = {
    .on_will_mount = view_model_compositor_on_will_mount,
    .on_mount = view_model_compositor_on_mount,
    .on_will_unmount = view_model_compositor_on_will_unmount,
    .on_unmount = view_model_compositor_on_unmount,
    .create_sub_view_model = view_model_compositor_create_sub_view_model,
    .create_sub_view_model_array = view_model_compositor_create_sub_view_model_array};

view_model_t* view_model_compositor_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_object_vtable_compositor);
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(obj);
  view_model_t* view_model = VIEW_MODEL(obj);
  return_value_if_fail(compositor != NULL, NULL);

  view_model->vt = &s_view_model_vtable_compositor;
  darray_init(&(compositor->view_models), 4, NULL, NULL);

  return view_model;
}

static ret_t view_model_compositor_forward_events(void* ctx, event_t* e) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(ctx);

  e->target = ctx;
  emitter_dispatch(EMITTER(compositor), e);

  return RET_OK;
}

ret_t view_model_compositor_add(view_model_t* view_model, view_model_t* vm) {
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(view_model);
  return_value_if_fail(view_model != NULL && vm != NULL, RET_BAD_PARAMS);

  if (darray_push(&(compositor->view_models), vm) == RET_OK) {
    emitter_on(EMITTER(vm), EVT_ITEMS_CHANGED, emitter_forward, view_model);
    emitter_on(EMITTER(vm), EVT_PROPS_CHANGED, view_model_compositor_forward_events, view_model);
    emitter_on(EMITTER(vm), EVT_PROP_CHANGED, view_model_compositor_forward_events, view_model);

    return RET_OK;
  }

  return RET_OOM;
}
