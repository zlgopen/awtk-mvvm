/**
 * File:   view_model.h
 * Author: AWTK Develop Team
 * Brief:  view_model
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/str.h"
#include "tkc/utils.h"
#include "tkc/expr_eval.h"
#include "base/locale_info.h"
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
  return_value_if_fail(view_model != NULL && name != NULL, FALSE);
  name = view_model_preprocess_prop(view_model, name);

  return object_has_prop(OBJECT(view_model), name);
}

ret_t view_model_get_prop(view_model_t* view_model, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  object_t* obj = OBJECT(view_model);
  return_value_if_fail(view_model != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);
  name = view_model_preprocess_prop(view_model, name);

  if (object_is_collection(obj)) {
    ret = object_get_prop(obj, name, value);
  } else {
    ret = object_get_prop_by_path(obj, name, value);
  }

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
  object_t* obj = OBJECT(view_model);

  if (object_is_collection(obj)) {
    ret = object_set_prop(obj, name, v);
  } else {
    ret = object_set_prop_by_path(obj, name, v);
  }

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
  return_value_if_fail(view_model != NULL && name != NULL, FALSE);

  if (object_is_collection(OBJECT(view_model))) {
    char cursor[TK_NUM_MAX_LEN + 1];
    int32_t index = object_get_prop_int(OBJECT(view_model), VIEW_MODEL_PROP_CURSOR, 0);

    tk_itoa(cursor, TK_NUM_MAX_LEN, index);
    ret = object_can_exec(OBJECT(view_model), name, cursor);
  } else {
    ret = object_can_exec_by_path(OBJECT(view_model), name, args);
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
  return_value_if_fail(view_model != NULL && name != NULL, RET_BAD_PARAMS);

  if (object_is_collection(OBJECT(view_model)) && args == NULL) {
    char cursor[TK_NUM_MAX_LEN + 1];
    int32_t index = object_get_prop_int(OBJECT(view_model), VIEW_MODEL_PROP_CURSOR, 0);

    tk_itoa(cursor, TK_NUM_MAX_LEN, index);
    ret = object_exec(OBJECT(view_model), name, cursor);
  } else {
    ret = object_exec_by_path(OBJECT(view_model), name, args);
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
    emitter_dispatch_simple_event(EMITTER(view_model), EVT_ITEMS_CHANGED);
  }

  return ret;
}

static EvalResult func_tr(const ExprValue* input, void* user_data, ExprValue* output) {
  if (input->type == EXPR_VALUE_TYPE_STRING) {
    const char* str = input->v.str.str;
    str = locale_info_tr(locale_info(), str);
    expr_value_set_string(output, str, strlen(str));
    return EVAL_RESULT_OK;
  } else {
    expr_value_set_string(output, "", 0);
    return EVAL_RESULT_BAD_PARAMS;
  }
}

static EvalFunc vm_get_func(const char* name, void* user_data) {
  const EvalHooks* hooks = eval_default_hooks();

  if (tk_str_eq(name, "tr")) {
    return func_tr;
  }

  return hooks->get_func(name, user_data);
}

static EvalResult vm_get_variable(const char* name, void* user_data, ExprValue* output) {
  value_t value;
  view_model_t* view_model = (view_model_t*)user_data;
  const EvalHooks* hooks = eval_default_hooks();

  if (view_model_get_prop(view_model, name, &value) == RET_OK) {
    if (value.type == VALUE_TYPE_STRING) {
      const char* str = value_str(&value);
      if (str != NULL) {
        expr_value_set_string(output, str, strlen(str));
      } else {
        expr_value_set_string(output, "", 0);
      }
    } else {
      expr_value_set_number(output, value_double(&value));
    }

    return EVAL_RESULT_OK;
  }

  return hooks->get_variable(name, user_data, output);
}

ret_t view_model_eval(view_model_t* view_model, const char* expr, value_t* v) {
  object_t* obj = OBJECT(view_model);
  return_value_if_fail(expr != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  expr = view_model_preprocess_expr(view_model, expr);
  if (tk_is_valid_prop_name(expr)) {
    return view_model_get_prop(view_model, expr, v);
  } else {
    EvalHooks hooks;
    ExprValue result;
    EvalResult ret;

    hooks.get_func = vm_get_func;
    hooks.get_variable = vm_get_variable;

    ret = eval_execute(expr, &hooks, (void*)view_model, &result);
    if (ret == EVAL_RESULT_OK) {
      if (result.type == EXPR_VALUE_TYPE_STRING) {
        value_dup_str(v, result.v.str.str);
      } else {
        double res = result.v.val;
        if (res > (int64_t)res) {
          value_set_double(v, res);
        } else {
          value_set_int64(v, (int64_t)res);
        }
      }
      expr_value_clear(&result);

      return RET_OK;
    } else {
      log_warn("expr error: %s\n", eval_result_to_string(ret));
      value_set_int(v, 0);
      return RET_FAIL;
    }
  }
}

ret_t view_model_notify_props_changed(view_model_t* view_model) {
  return emitter_dispatch_simple_event(EMITTER(view_model), EVT_PROPS_CHANGED);
}

view_model_t* view_model_create_sub_view_model(view_model_t* view_model, const char* name) {
  return_value_if_fail(view_model != NULL && name != NULL, NULL);

  return view_model->vt->create_sub_view_model(view_model, name);
}

view_model_t* view_model_create_sub_view_model_array(view_model_t* view_model, const char* name) {
  return_value_if_fail(view_model != NULL && name != NULL, NULL);

  return view_model->vt->create_sub_view_model_array(view_model, name);
}
