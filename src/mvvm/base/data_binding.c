﻿/**
 * File:   data_binding.c
 * Author: AWTK Develop Team
 * Brief:  data binding rule
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
 * 2019-01-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/locale_info.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/binding_context.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/value_converter.h"
#include "mvvm/base/value_validator.h"
#include "mvvm/base/view_model_array.h"

#define BINDING_STR_CHANGED "Changed"
#define BINDING_STR_CHANGING "Changing"
#define BINDING_STR_EXPLICIT "Explicit"

#define equal tk_str_ieq

static data_binding_t* data_binding_cast(void* rule);

static ret_t data_binding_on_destroy(object_t* obj) {
  data_binding_t* rule = data_binding_cast(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rule->path);
  TKMEM_FREE(rule->prop);
  TKMEM_FREE(rule->validator);
  TKMEM_FREE(rule->converter);
  fscript_destroy(rule->expr);
  if (rule->to_view_expr != NULL) {
    fscript_destroy(rule->to_view_expr);
  }
  if (rule->to_model_expr != NULL) {
    fscript_destroy(rule->to_model_expr);
  }
  if (rule->props != NULL) {
    object_unref(rule->props);
  }
  TKMEM_FREE(rule->to_view);
  TKMEM_FREE(rule->to_model);

  return RET_OK;
}

static ret_t data_binding_object_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  const char* value = value_str(v);
  data_binding_t* rule = DATA_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (BINDING_RULE(rule)->inited) {
    view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
    return view_model_set_prop(view_model, name, v);
  }

  if (rule->path == NULL && value == NULL) {
    value = name;
    rule->path = tk_str_copy(rule->path, value);
    if (tk_str_start_with(value, DATA_BINDING_ERROR_OF) || !tk_is_valid_prop_name(value)) {
      rule->mode = BINDING_ONE_WAY;
    }
  } else if (equal(DATA_BINDING_TO_VIEW, name)) {
    rule->to_view = tk_str_copy(rule->to_view, value);
  } else if (equal(DATA_BINDING_TO_MODEL, name)) {
    rule->to_model = tk_str_copy(rule->to_model, value);
  } else if (equal(DATA_BINDING_MODE, name)) {
    binding_mode_t mode = BINDING_TWO_WAY;

    if (equal(value, BINDING_STR_ONCE)) {
      mode = BINDING_ONCE;
    } else if (equal(value, BINDING_STR_TWO_WAY)) {
      mode = BINDING_TWO_WAY;
    } else if (equal(value, BINDING_STR_ONE_WAY)) {
      mode = BINDING_ONE_WAY;
    } else if (equal(value, BINDING_STR_ONE_WAY_TO_VIEW_MODEL)) {
      mode = BINDING_ONE_WAY_TO_VIEW_MODEL;
    }

    rule->mode = mode;
  } else if (equal(DATA_BINDING_TRIGGER, name)) {
    update_model_trigger_t trigger = UPDATE_WHEN_CHANGED;

    if (equal(value, BINDING_STR_CHANGED)) {
      trigger = UPDATE_WHEN_CHANGED;
    } else if (equal(value, BINDING_STR_CHANGING)) {
      trigger = UPDATE_WHEN_CHANGING;
    } else if (equal(value, BINDING_STR_EXPLICIT)) {
      trigger = UPDATE_WHEN_EXPLICIT;
    }

    rule->trigger = trigger;
  } else if (equal(DATA_BINDING_PATH, name)) {
    rule->path = tk_str_copy(rule->path, value);
    if (tk_str_start_with(value, DATA_BINDING_ERROR_OF) || !tk_is_valid_name(value)) {
      rule->mode = BINDING_ONE_WAY;
    }
  } else if (equal(DATA_BINDING_PROP, name)) {
    rule->prop = tk_str_copy(rule->prop, value);
  } else if (equal(DATA_BINDING_CONVERTER, name)) {
    rule->converter = tk_str_copy(rule->converter, value);
  } else if (equal(DATA_BINDING_VALIDATOR, name)) {
    rule->validator = tk_str_copy(rule->validator, value);
  } else if (equal(BINDING_RULE_PROP_INITED, name)) {
    BINDING_RULE(rule)->inited = TRUE;
    rule->expr = fscript_create(obj, rule->path);
    if (rule->to_view != NULL) {
      rule->to_view_expr = fscript_create(obj, rule->to_view);
    }
    if (rule->to_model != NULL) {
      rule->to_model_expr = fscript_create(obj, rule->to_model);
    }
  } else {
    if (rule->props == NULL) {
      rule->props = object_default_create();
    }
    ret = object_set_prop(rule->props, name, v);
  }

  return ret;
}

static ret_t data_binding_object_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  data_binding_t* rule = DATA_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (BINDING_RULE(rule)->inited) {
    if (tk_str_eq(name, STR_PROP_SELF)) {
      value_set_pointer(v, BINDING_RULE(rule)->widget);
      return RET_OK;
    }

    if (tk_str_eq(name, rule->prop) && rule->value != NULL) {
      value_copy(v, rule->value);
      return RET_OK;
    }

    if (BINDING_RULE_CONTEXT(rule) != NULL) {
      view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);
      return view_model_get_prop(view_model, name, v);
    } else {
      return RET_NOT_FOUND;
    }
  }

  if (equal(DATA_BINDING_MODE, name)) {
    value_set_int(v, rule->mode);
  } else if (equal(DATA_BINDING_TRIGGER, name)) {
    value_set_int(v, rule->trigger);
  } else if (equal(DATA_BINDING_PATH, name)) {
    value_set_str(v, rule->path);
  } else if (equal(DATA_BINDING_PROP, name)) {
    value_set_str(v, rule->prop);
  } else if (equal(DATA_BINDING_TO_VIEW, name)) {
    value_set_str(v, rule->to_view);
  } else if (equal(DATA_BINDING_TO_MODEL, name)) {
    value_set_str(v, rule->to_model);
  } else if (equal(DATA_BINDING_CONVERTER, name)) {
    value_set_str(v, rule->converter);
  } else if (equal(DATA_BINDING_VALIDATOR, name)) {
    value_set_str(v, rule->validator);
  } else {
    ret = object_get_prop(rule->props, name, v);
  }

  return ret;
}

static ret_t data_binding_object_exec(object_t* obj, const char* name, const char* args) {
  data_binding_t* rule = (data_binding_t*)(obj);
  binding_context_t* context = BINDING_RULE_CONTEXT(rule);
  view_model_t* view_model = BINDING_RULE_VIEW_MODEL(rule);

  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);
  if (binding_context_exec(context, name, args) == RET_OK) {
    return RET_OK;
  }

  if (object_is_collection(OBJECT(view_model))) {
    uint32_t cursor = BINDING_RULE(rule)->cursor;
    view_model_array_set_cursor(view_model, cursor);
  }

  return view_model_exec(view_model, name, args);
}

static const object_vtable_t s_data_binding_vtable = {.type = "data_binding",
                                                      .desc = "data_binding",
                                                      .size = sizeof(data_binding_t),
                                                      .is_collection = FALSE,
                                                      .exec = data_binding_object_exec,
                                                      .on_destroy = data_binding_on_destroy,
                                                      .get_prop = data_binding_object_get_prop,
                                                      .set_prop = data_binding_object_set_prop};

static data_binding_t* data_binding_cast(void* rule) {
  object_t* obj = OBJECT(rule);
  return_value_if_fail(obj != NULL && obj->vt == &s_data_binding_vtable, NULL);

  return (data_binding_t*)rule;
}

data_binding_t* data_binding_create(void) {
  object_t* obj = object_create(&s_data_binding_vtable);
  data_binding_t* rule = DATA_BINDING(obj);
  return_value_if_fail(obj != NULL, NULL);

  rule->mode = BINDING_ONE_WAY;
  rule->props = object_default_create();

  if (rule->props == NULL) {
    object_unref(obj);
    rule = NULL;
  }

  return rule;
}

static ret_t value_to_model(const char* name, const value_t* from, value_t* to) {
  if (name != NULL) {
    value_converter_t* c = value_converter_create(name);
    if (c != NULL) {
      if (value_converter_to_model(c, from, to) == RET_OK) {
        object_unref(OBJECT(c));
        return RET_OK;
      } else {
        log_debug("value_converter_to_model %s failed\n", name);
        object_unref(OBJECT(c));
        return RET_FAIL;
      }
    } else {
      log_debug("not found value_converter %s\n", name);
      return RET_FAIL;
    }
  } else {
    *to = *from;
    return RET_OK;
  }
}

static ret_t value_to_view(const char* name, value_t* from, value_t* to) {
  if (name != NULL) {
    value_converter_t* c = value_converter_create(name);
    if (c != NULL) {
      if (value_converter_to_view(c, from, to) == RET_OK) {
        value_reset(from);
        object_unref(OBJECT(c));
        return RET_OK;
      } else {
        log_debug("value_converter_to_model %s failed\n", name);
        object_unref(OBJECT(c));
        return RET_FAIL;
      }
    } else {
      log_debug("not found value_converter %s\n", name);
      return RET_FAIL;
    }
  } else {
    *to = *from;
    return RET_OK;
  }
}

static bool_t value_is_valid(view_model_t* view_model, const char* name, const value_t* value,
                             str_t* msg) {
  bool_t ret = FALSE;
  value_validator_t* validator = NULL;

  if (name == NULL) {
    return TRUE;
  }

  validator = value_validator_create(name);
  if (validator != NULL) {
    value_validator_set_context(validator, OBJECT(view_model));
    ret = value_validator_is_valid(validator, value, msg);
    value_validator_set_context(validator, NULL);
    object_unref(OBJECT(validator));
  } else {
    log_debug("not found validator\n");
  }

  return ret;
}

static ret_t value_fix(view_model_t* view_model, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  value_validator_t* validator = NULL;

  if (name == NULL) {
    return RET_OK;
  }

  validator = value_validator_create(name);
  if (validator != NULL) {
    value_validator_set_context(validator, OBJECT(view_model));
    ret = value_validator_fix(validator, value);
    value_validator_set_context(validator, NULL);
    object_unref(OBJECT(validator));
  } else {
    log_debug("not found validator\n");
  }

  return ret;
}

ret_t data_binding_get_prop(data_binding_t* rule, value_t* v) {
  value_t raw;
  view_model_t* view_model = NULL;
  return_value_if_fail(rule != NULL && v != NULL, RET_BAD_PARAMS);

  view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  if (object_is_collection(OBJECT(view_model))) {
    uint32_t cursor = BINDING_RULE(rule)->cursor;
    view_model_array_set_cursor(view_model, cursor);

    if (tk_str_eq(rule->path, VIEW_MODEL_PROP_CURSOR)) {
      value_set_int(v, cursor);

      return RET_OK;
    }
  }

  if (rule->to_view_expr != NULL) {
    if (fscript_exec(rule->to_view_expr, &raw) == RET_OK) {
      *v = raw;
      return RET_OK;
    }
    return RET_FAIL;
  }

  if (fscript_exec(rule->expr, &raw) != RET_OK) {
    log_debug("view_model_eval fail: %s\n", rule->path);
    return RET_FAIL;
  }

  return value_to_view(rule->converter, &raw, v);
}

static ret_t vm_set_prop(view_model_t* vm, const char* converter, const char* path,
                         const value_t* raw) {
  if (converter == NULL) {
    return view_model_set_prop(vm, path, raw);
  } else {
    value_t v;
    if (value_to_model(converter, raw, &v) == RET_OK) {
      return view_model_set_prop(vm, path, &v);
    } else {
      return RET_FAIL;
    }
  }
}

ret_t data_binding_set_prop(data_binding_t* rule, const value_t* raw) {
  view_model_t* view_model = NULL;
  return_value_if_fail(rule != NULL && raw != NULL, RET_BAD_PARAMS);

  view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  str_clear(&(view_model->last_error));
  if (object_is_collection(OBJECT(view_model))) {
    uint32_t cursor = BINDING_RULE(rule)->cursor;
    view_model_array_set_cursor(view_model, cursor);
  }

  if (rule->to_model_expr != NULL) {
    value_t v;
    rule->value = raw;
    fscript_exec(rule->to_model_expr, &v);
    rule->value = NULL;
    return view_model_set_prop(view_model, rule->path, &v);
  }

  if (!value_is_valid(view_model, rule->validator, raw, &(view_model->last_error))) {
    value_t fix_value;
    value_set_int(&fix_value, 0);
    value_deep_copy(&fix_value, raw);

    if (value_fix(view_model, rule->validator, &fix_value) == RET_OK) {
      ret_t ret = vm_set_prop(view_model, rule->converter, rule->path, &fix_value);
      value_reset(&fix_value);

      return ret;
    }
    value_reset(&fix_value);

    return RET_BAD_PARAMS;
  }

  return vm_set_prop(view_model, rule->converter, rule->path, raw);
}
