/**
 * File:   view_model.h
 * Author: AWTK Develop Team
 * Brief:  view_model
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/view_model.h"

view_model_t* view_model_init(view_model_t* view_model) {
  return_value_if_fail(view_model != NULL, NULL);

  str_init(&(view_model->temp), 0);
  str_init(&(view_model->last_error), 0);
  view_model->preprocess_expr = NULL;
  view_model->preprocess_prop = NULL;

  return view_model;
}

ret_t view_model_deinit(view_model_t* view_model) {
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  str_reset(&(view_model->temp));
  str_reset(&(view_model->last_error));

  return RET_OK;
}

ret_t view_model_on_will_mount(view_model_t* view_model, navigator_request_t* req) {
  view_model_will_mount_event_t e;
  return_value_if_fail(view_model != NULL && req != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  e.req = req;
  e.e = event_init(EVT_VIEW_MODEL_WILL_MOUNT, view_model);
  emitter_dispatch(EMITTER(view_model), (event_t*)&e);
  if (view_model->vt != NULL && view_model->vt->on_will_mount != NULL) {
    return view_model->vt->on_will_mount(view_model, req);
  }

  return RET_OK;
}

ret_t view_model_on_mount(view_model_t* view_model) {
  event_t e = event_init(EVT_VIEW_MODEL_MOUNT, view_model);
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  emitter_dispatch(EMITTER(view_model), (event_t*)&e);
  if (view_model->vt != NULL && view_model->vt->on_mount != NULL) {
    return view_model->vt->on_mount(view_model);
  }

  return RET_OK;
}

ret_t view_model_on_will_unmount(view_model_t* view_model) {
  event_t e = event_init(EVT_VIEW_MODEL_WILL_UNMOUNT, view_model);
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  emitter_dispatch(EMITTER(view_model), (event_t*)&e);
  if (view_model->vt != NULL && view_model->vt->on_will_unmount != NULL) {
    return view_model->vt->on_will_unmount(view_model);
  }

  return RET_OK;
}

ret_t view_model_on_unmount(view_model_t* view_model) {
  event_t e = event_init(EVT_VIEW_MODEL_UNMOUNT, view_model);
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  emitter_dispatch(EMITTER(view_model), (event_t*)&e);
  if (view_model->vt != NULL && view_model->vt->on_unmount != NULL) {
    return view_model->vt->on_unmount(view_model);
  }
  str_reset(&(view_model->temp));
  return RET_OK;
}

const char* view_model_preprocess_expr(view_model_t* view_model, const char* expr) {
  return_value_if_fail(view_model != NULL && expr != NULL, NULL);

  if (view_model->preprocess_expr != NULL) {
    return view_model->preprocess_expr(view_model, expr);
  } else {
    return expr;
  }
}

const char* view_model_preprocess_prop(view_model_t* view_model, const char* prop) {
  return_value_if_fail(view_model != NULL && prop != NULL, NULL);

  if (view_model->preprocess_prop != NULL) {
    return view_model->preprocess_prop(view_model, prop);
  } else {
    return prop;
  }
}

bool_t view_model_has_prop(view_model_t* view_model, const char* name) {
  tk_object_t* obj = TK_OBJECT(view_model);
  return_value_if_fail(view_model != NULL && name != NULL, FALSE);
  name = view_model_preprocess_prop(view_model, name);

  return tk_object_has_prop(obj, name);
}

ret_t view_model_get_prop(view_model_t* view_model, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  tk_object_t* obj = TK_OBJECT(view_model);
  return_value_if_fail(view_model != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);
  name = view_model_preprocess_prop(view_model, name);

  ret = tk_object_get_prop(obj, name, value);

  if (ret == RET_NOT_FOUND) {
    if (view_model->parent != NULL) {
      ret = view_model_get_prop(view_model->parent, name, value);
    }
  }

  return ret;
}

static ret_t view_model_set_prop_recursive(view_model_t* view_model, const char* name,
                                           const value_t* v) {
  ret_t ret = RET_OK;
  tk_object_t* obj = TK_OBJECT(view_model);

  ret = tk_object_set_prop(obj, name, v);

  if (ret == RET_NOT_FOUND) {
    if (view_model->parent != NULL) {
      ret = view_model_set_prop_recursive(view_model->parent, name, v);
    }
  }

  return ret;
}

static ret_t view_model_set_prop_if_diff(view_model_t* view_model, const char* name,
                                         const value_t* v) {
  value_t t;
  value_t old;

  value_set_int(&old, 0);
  if (view_model_get_prop(view_model, name, &old) == RET_OK) {
    if (value_equal(&old, v)) {
      return RET_OK;
    }
  }

  if (v->type == VALUE_TYPE_WSTRING) {
    str_t* str = &(view_model->temp);
    return_value_if_fail(str_from_value(str, v) == RET_OK, RET_OOM);
    value_set_str(&t, str->str);

    v = &t;
  }

  return view_model_set_prop_recursive(view_model, name, v);
}

ret_t view_model_set_prop(view_model_t* view_model, const char* name, const value_t* value) {
  return_value_if_fail(view_model != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);

  if (!tk_is_valid_prop_name(name)) {
    return RET_OK;
  }

  name = view_model_preprocess_prop(view_model, name);
  return view_model_set_prop_if_diff(view_model, name, value);
}

bool_t view_model_can_exec(view_model_t* view_model, const char* name, const char* args) {
  bool_t ret = TRUE;
  tk_object_t* obj = TK_OBJECT(view_model);
  return_value_if_fail(view_model != NULL && name != NULL, FALSE);

  if (tk_object_is_collection(obj)) {
    if (args != NULL) {
      ret = tk_object_can_exec(obj, name, args);
    } else {
      char cursor[TK_NUM_MAX_LEN + 1];
      int32_t index = tk_object_get_prop_int(obj, VIEW_MODEL_PROP_CURSOR, 0);

      tk_itoa(cursor, TK_NUM_MAX_LEN, index);
      ret = tk_object_can_exec(obj, name, cursor);
    }
  } else {
    ret = tk_object_can_exec(obj, name, args);
  }

  if (!ret) {
    if (view_model->parent != NULL) {
      ret = view_model_can_exec(view_model->parent, name, args);
    }
  }

  return ret;
}

ret_t view_model_exec(view_model_t* view_model, const char* name, const char* args) {
  ret_t ret = RET_OK;
  tk_object_t* obj = TK_OBJECT(view_model);
  return_value_if_fail(view_model != NULL && name != NULL, RET_BAD_PARAMS);

  if (tk_object_is_collection(obj)) {
    if (args != NULL) {
      ret = tk_object_exec(obj, name, args);
    } else {
      char cursor[TK_NUM_MAX_LEN + 1];
      int32_t index = tk_object_get_prop_int(obj, VIEW_MODEL_PROP_CURSOR, 0);

      tk_itoa(cursor, TK_NUM_MAX_LEN, index);
      ret = tk_object_exec(obj, name, cursor);
    }
  } else {
    ret = tk_object_exec(obj, name, args);
  }

  if (ret == RET_NOT_FOUND || ret == RET_NOT_IMPL) {
    if (view_model->parent != NULL) {
      ret = view_model_exec(view_model->parent, name, args);
      log_debug("exec cmd in view_model->parent:%d\n", ret);
    }
  }

  if (ret == RET_OBJECT_CHANGED) {
    emitter_dispatch_simple_event(EMITTER(view_model), EVT_PROP_CHANGED);
  } else if (ret == RET_ITEMS_CHANGED) {
    view_model_notify_items_changed(view_model, TK_OBJECT(view_model));
  }

  return ret;
}

ret_t view_model_notify_props_changed(view_model_t* view_model) {
  return emitter_dispatch_simple_event(EMITTER(view_model), EVT_PROPS_CHANGED);
}

ret_t view_model_notify_items_changed(view_model_t* view_model, tk_object_t* target) {
  emitter_t* emitter = EMITTER(view_model);
  event_t e = event_init(EVT_ITEMS_CHANGED, target != NULL ? target : TK_OBJECT(view_model));

  return emitter_dispatch(emitter, &e);
}

view_model_t* view_model_create_sub_view_model(view_model_t* view_model, const char* name) {
  return_value_if_fail(view_model != NULL && name != NULL, NULL);

  return view_model->vt->create_sub_view_model(view_model, name);
}

view_model_t* view_model_create_sub_view_model_array(view_model_t* view_model, const char* name) {
  return_value_if_fail(view_model != NULL && name != NULL, NULL);

  return view_model->vt->create_sub_view_model_array(view_model, name);
}
