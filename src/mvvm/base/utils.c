/**
 * File:   utils.c
 * Author: AWTK Develop Team
 * Brief:  utils
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
 * 2019-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/named_value.h"
#include "tkc/fscript.h"
#include "tkc/tokenizer.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/navigator_request.h"

const char* tk_destruct_array_prop_name(const char* name, uint32_t* index) {
  const char* prop = NULL;
  return_value_if_fail(name != NULL, NULL);

  prop = strstr(name, "].");
  if (prop != NULL && name[0] == '[') {
    if (index != NULL) {
      *index = tk_atoi(name + 1);
    }

    return prop + 2;
  } else {
    return name;
  }
}

ret_t tk_command_arguments_to_object(const char* args, tk_object_t* obj) {
  tokenizer_t t;
  const char* params = NULL;
  const char* k = NULL;
  const char* v = NULL;
  char key[MAX_PATH + 1];
  bool_t is_expr = FALSE;
  return_value_if_fail(args != NULL && obj != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(args, COMMAND_ARGS_STRING_PREFIX)) {
    params = args + strlen(COMMAND_ARGS_STRING_PREFIX);
  } else if (tk_str_start_with(args, COMMAND_ARGS_FSCRIPT_PREFIX)) {
    params = args + strlen(COMMAND_ARGS_FSCRIPT_PREFIX);
    is_expr = TRUE;
  } else {
    params = strchr(args, '?');
    if (params != NULL) {
      char target[TK_NAME_LEN+1] = {0};

      tk_strncpy(target, args, params - args);
      tk_object_set_prop_str(obj, NAVIGATOR_ARG_TARGET, target);
      params++;
    }
  }
  return_value_if_fail(params != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tokenizer_init(&t, params, strlen(params), "=") != NULL, RET_BAD_PARAMS);

  do {
    k = tokenizer_next(&t);
    while (k != NULL && *k == '&') {
      k += 1;
    }

    tk_strncpy(key, k, MAX_PATH);

    if (is_expr) {
      v = tokenizer_next_expr_until(&t, "&,}");
    } else {
      v = tokenizer_next_str_until(&t, "&,");
    }
    if (v != NULL && *v == '=') {
      v += 1;
    }

    ENSURE(tk_object_set_prop_str(obj, key, v) == RET_OK);
  } while (tokenizer_has_more(&t));

  tokenizer_deinit(&t);

  return RET_OK;
}

static ret_t tk_command_arguments_str_extend(str_t* str, value_t* v) {
  uint32_t size = 0;

  switch (v->type) {
    case VALUE_TYPE_DOUBLE:
    case VALUE_TYPE_FLOAT32:
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_BINARY: {
      size = 64;
      break;
    }

    case VALUE_TYPE_OBJECT: {
      tk_object_t* obj = value_object(v);
      if (obj != NULL) {
        size = strlen(obj->vt->type) + 32;
      }
      break;
    }
    default:
      size = 32;
  }

  if (str->size + size > str->capacity) {
    return str_extend(str, str->size + tk_max(MAX_PATH, size));
  }

  return RET_OK;
}

static ret_t tk_command_arguments_visit_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  value_t* v = &(nv->value);
  str_t* temp = (str_t*)ctx;

  if (temp->str[temp->size - 1] != '?') {
    str_append(temp, "&");
  }

  str_append(temp, nv->name);
  str_append(temp, "=");

  if (v->type == VALUE_TYPE_STRING) {
    str_append(temp, value_str(v));
  } else {
    tk_command_arguments_str_extend(temp, v);
    value_str_ex(v, temp->str + temp->size, temp->capacity - temp->size);
    temp->size = strlen(temp->str);
  }

  return RET_OK;
}

ret_t tk_command_arguments_from_object(tk_object_t* args, str_t* temp) {
  return_value_if_fail(args != NULL && temp != NULL, RET_BAD_PARAMS);
  str_extend(temp, MAX_PATH);
  return tk_object_foreach_prop(args, tk_command_arguments_visit_prop, temp);
}

static ret_t tk_command_arguments_visit_fscript_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  tk_object_t* fscript = (tk_object_t*)ctx;
  value_t v;
  memset(&v, 0x00, sizeof(value_t));

  if (fscript_eval(fscript, value_str(&(nv->value)), &v) == RET_OK) {
    named_value_set_value(nv, &v);
  }
  value_reset(&v);

  return RET_OK;
}

ret_t tk_command_arguments_fscript(tk_object_t* args, tk_object_t* ctx) {
  return_value_if_fail(args != NULL && ctx != NULL, RET_BAD_PARAMS);
  return tk_object_foreach_prop(args, tk_command_arguments_visit_fscript_prop, ctx);
}

ret_t str_random(str_t* str, const char* format, uint32_t max) {
  char buff[256];
  return_value_if_fail(str != NULL && format != NULL && max > 0, RET_BAD_PARAMS);

  tk_snprintf(buff, sizeof(buff) - 1, format, random() % max);

  return str_set(str, buff);
}

#define STR_INVALID_PROP_CHARS "<>+-*/=()$!|&"

bool_t tk_is_valid_prop_name(const char* name) {
  const char* p = name;
  return_value_if_fail(name != NULL, FALSE);

  if (tk_str_start_with(name, "item.")) {
    return TRUE;
  }

  while (*p) {
    if (strchr(STR_INVALID_PROP_CHARS, *p) != NULL) {
      return FALSE;
    } else {
      p++;
    }
  }

  return TRUE;
}
