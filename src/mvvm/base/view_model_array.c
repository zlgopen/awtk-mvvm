/**
 * File:   view_model_array.c
 * Author: AWTK Develop Team
 * Brief:  view_model_array
 *
 * Copyright (c) 2019 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/view_model_array.h"

static const char* view_model_array_preprocess_expr(view_model_t* view_model, const char* expr) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(view_model != NULL && expr != NULL, NULL);

  str_set(&(vm_array->temp_expr), expr);
  str_replace(&(vm_array->temp_expr), "item.", "$item_");
  str_replace(&(vm_array->temp_expr), "$$", "$");

  return vm_array->temp_expr.str;
}

static const char* view_model_array_preprocess_prop(view_model_t* view_model, const char* prop) {
  char index[TK_NUM_MAX_LEN + 1];
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(view_model != NULL && prop != NULL, NULL);

  if (tk_str_start_with(prop, "item")) {
    tk_snprintf(index, TK_NUM_MAX_LEN, "[%d].", vm_array->cursor);
    str_set(&(vm_array->temp_prop), prop);
    str_replace(&(vm_array->temp_prop), "item_", index);
    str_replace(&(vm_array->temp_prop), "item.", index);

    return vm_array->temp_prop.str;
  } else if(tk_str_start_with(prop, "selected.")) {
    tk_snprintf(index, TK_NUM_MAX_LEN, "[%d].", vm_array->selected_index);
    str_set(&(vm_array->temp_prop), prop);
    str_replace(&(vm_array->temp_prop), "selected.", index);

    return vm_array->temp_prop.str;
  } else {
    return prop;
  }
}

ret_t view_model_array_inc_cursor(view_model_t* view_model) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->cursor++;

  return RET_OK;
}

ret_t view_model_array_set_cursor(view_model_t* view_model, uint32_t cursor) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->cursor = cursor;

  return RET_OK;
}

view_model_t* view_model_array_init(view_model_t* view_model) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, NULL);

  view_model_init(view_model);
  str_init(&(vm_array->temp_expr), 0);
  str_init(&(vm_array->temp_prop), 0);
  view_model->preprocess_expr = view_model_array_preprocess_expr;
  view_model->preprocess_prop = view_model_array_preprocess_prop;

  return view_model;
}

ret_t view_model_array_deinit(view_model_t* view_model) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  str_reset(&(vm_array->temp_expr));
  str_reset(&(vm_array->temp_prop));
  view_model_deinit(view_model);

  return RET_OK;
}

ret_t view_model_array_notify_items_changed(view_model_t* view_model) {
  return view_model_notify_items_changed(view_model, TK_OBJECT(view_model));
}

ret_t view_model_array_default_set_prop(view_model_t* view_model, const char* name,
                                        const value_t* value) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && value != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VIEW_MODEL_PROP_SELECTED_INDEX, name)) {
    view_model_array_set_selected_index(view_model, value_int(value));
    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(view_model, value_int(value));
    return RET_OK;
  } else if (tk_str_ieq(VIEW_MODEL_PROP_ITEMS, name)) {
    return RET_OK;
  }

  return RET_NOT_IMPL;
}

ret_t view_model_array_default_get_prop(view_model_t* view_model, const char* name,
                                        value_t* value) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && value != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VIEW_MODEL_PROP_SELECTED_INDEX, name)) {
    int32_t size = tk_object_get_prop_uint32(TK_OBJECT(view_model), TK_OBJECT_PROP_SIZE, 0);

    value_set_int(value, tk_min_int(vm_array->selected_index, size-1));
    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(value, vm_array->cursor);
    return RET_OK;
  }

  return RET_NOT_IMPL;
}

ret_t view_model_array_set_selected_index(view_model_t* view_model, uint32_t index) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->selected_index = index;

  return RET_OBJECT_CHANGED;
}

ret_t view_model_array_default_exec(view_model_t* view_model, const char* name, const char* args) {
  if (tk_str_eq(name, VIEW_MODEL_CMD_SET_SELECTED)) {
    return view_model_array_set_selected_index(view_model, tk_atoi(args));
  }

  return RET_NOT_FOUND;
}

bool_t view_model_array_default_can_exec(view_model_t* view_model, const char* name,
                                         const char* args) {
  if (tk_str_eq(name, VIEW_MODEL_CMD_SET_SELECTED)) {
    return TRUE;
  }
  return FALSE;
}
