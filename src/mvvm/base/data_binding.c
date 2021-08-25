/**
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
#include "tkc/object_default.h"
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
  const char* value;
  data_binding_t* rule = DATA_BINDING(obj);
  return_value_if_fail(rule != NULL, RET_BAD_PARAMS);

  if (BINDING_RULE(rule)->inited) {
    binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
    ret = binding_context_set_prop_by_rule(ctx, BINDING_RULE(rule), name, v);
    if (ret == RET_NOT_FOUND) {
      if (rule->props == NULL) {
        rule->props = object_default_create();
      }
      return object_set_prop(rule->props, name, v);
    }
    return ret;
  }

  value = value_str(v);

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
      value_set_pointer(v, BINDING_RULE_WIDGET(rule));
      return RET_OK;
    } else if (tk_str_eq(name, rule->prop) && rule->value != NULL) {
      value_copy(v, rule->value);
      return RET_OK;
    } else {
      binding_context_t* ctx = BINDING_RULE_CONTEXT(rule);
      ret = binding_context_get_prop_by_rule(ctx, BINDING_RULE(rule), name, v);
      if (ret == RET_NOT_FOUND && rule->props != NULL) {
        return object_get_prop(rule->props, name, v);
      }
      return ret;
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
    if (rule->props != NULL) {
      ret = object_get_prop(rule->props, name, v);
    }
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
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
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

data_binding_t* data_binding_create(void) {
  object_t* obj = object_create(&s_data_binding_vtable);
  data_binding_t* rule = DATA_BINDING(obj);
  return_value_if_fail(obj != NULL, NULL);

  rule->mode = BINDING_ONE_WAY;

  return rule;
}

data_binding_t* data_binding_cast(void* rule) {
  return_value_if_fail(binding_rule_is_data_binding(rule), NULL);

  return (data_binding_t*)rule;
}

bool_t binding_rule_is_data_binding(binding_rule_t* rule) {
  object_t* obj = OBJECT(rule);
  return obj != NULL && obj->vt == &s_data_binding_vtable;
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

static bool_t value_is_valid(object_t* ctx, const char* name, const value_t* value, str_t* msg) {
  bool_t ret = FALSE;
  value_validator_t* validator = NULL;

  if (name == NULL) {
    return TRUE;
  }

  validator = value_validator_create(name);
  if (validator != NULL) {
    value_validator_set_context(validator, ctx);
    ret = value_validator_is_valid(validator, value, msg);
    value_validator_set_context(validator, NULL);
    object_unref(OBJECT(validator));
  } else {
    log_debug("not found validator\n");
  }

  return ret;
}

static ret_t value_fix(object_t* ctx, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  value_validator_t* validator = NULL;

  if (name == NULL) {
    return RET_OK;
  }

  validator = value_validator_create(name);
  if (validator != NULL) {
    value_validator_set_context(validator, ctx);
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
    binding_context_t* context = BINDING_RULE_CONTEXT(rule);
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
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

ret_t data_binding_set_prop(data_binding_t* rule, const value_t* raw) {
  ret_t ret = RET_OK;
  object_t* obj = OBJECT(rule);
  view_model_t* view_model = NULL;
  return_value_if_fail(rule != NULL && raw != NULL, RET_BAD_PARAMS);

  view_model = BINDING_RULE_VIEW_MODEL(rule);
  return_value_if_fail(view_model != NULL, RET_BAD_PARAMS);

  str_clear(&(view_model->last_error));

  if (object_is_collection(OBJECT(view_model))) {
    binding_context_t* context = BINDING_RULE_CONTEXT(rule);
    uint32_t cursor = binding_context_get_items_cursor_of_rule(context, BINDING_RULE(rule));
    view_model_array_set_cursor(view_model, cursor);
  }

  if (rule->to_model_expr != NULL) {
    value_t v;
    rule->value = raw;
    fscript_exec(rule->to_model_expr, &v);
    rule->value = NULL;
    ret = object_set_prop(obj, rule->path, &v);
  } else {
    const value_t* temp = raw;
    value_t fix_value;
    value_t v;

    value_set_int(&fix_value, 0);

    if (!value_is_valid(obj, rule->validator, raw, &(view_model->last_error))) {
      value_deep_copy(&fix_value, raw);
      ret = value_fix(obj, rule->validator, &fix_value);
      temp = &fix_value;
    }

    if (ret == RET_OK && rule->converter != NULL) {
      ret = value_to_model(rule->converter, temp, &v);
      temp = &v;
    }

    if (ret == RET_OK) {
      ret = object_set_prop(obj, rule->path, temp);
    }

    value_reset(&fix_value);
  }

  return ret;
}
