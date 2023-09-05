/**
 * File:   binding_rule.h
 * Author: AWTK Develop Team
 * Brief:  binding rule parser
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
 * 2019-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/tokenizer.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/items_binding.h"
#include "mvvm/base/condition_binding.h"
#include "mvvm/base/binding_rule_parser.h"

/*
 * v-on:click=
 * v-on:pointer_down=
 * v-on:key_down:ctrl_a
 * v-data:visible=
 */

static ret_t data_binding_init(data_binding_t* rule, tokenizer_t* t) {
  if (tokenizer_has_more(t)) {
    const char* prop = t->str + t->cursor;

    return tk_object_set_prop_str(TK_OBJECT(rule), DATA_BINDING_PROP, prop);
  }

  return RET_FAIL;
}

static ret_t command_binding_init(command_binding_t* rule, tokenizer_t* t) {
  if (tokenizer_has_more(t)) {
    const char* event = tokenizer_next(t);
    return_value_if_fail(
        tk_object_set_prop_str(TK_OBJECT(rule), COMMAND_BINDING_EVENT, event) == RET_OK, RET_FAIL);

    if (tokenizer_has_more(t)) {
      const char* event_filter = tokenizer_next(t);
      return tk_object_set_prop_str(TK_OBJECT(rule), COMMAND_BINDING_KEY_FILTER, event_filter);
    } else {
      return RET_OK;
    }
  }

  return RET_FAIL;
}

static binding_rule_t* binding_rule_create(const char* name, bool_t inputable) {
  tokenizer_t t;
  binding_rule_t* rule = NULL;
  return_value_if_fail(name != NULL, NULL);
  return_value_if_fail(tokenizer_init(&t, name, strlen(name), ":") != NULL, NULL);

  if (tokenizer_has_more(&t)) {
    const char* type = tokenizer_next(&t);

    if (tk_str_ieq(type, BINDING_RULE_DATA_PREFIX)) {
      rule = BINDING_RULE(data_binding_create());
      if (rule != NULL) {
        if (data_binding_init((data_binding_t*)rule, &t) != RET_OK) {
          tk_object_unref((tk_object_t*)rule);
          rule = NULL;
        }
        tk_object_set_prop_str(TK_OBJECT(rule), DATA_BINDING_MODE,
                               inputable ? BINDING_STR_TWO_WAY : BINDING_STR_ONE_WAY);
      }
    } else if (tk_str_ieq(type, BINDING_RULE_COMMAND_PREFIX)) {
      rule = BINDING_RULE(command_binding_create());
      if (rule != NULL) {
        if (command_binding_init((command_binding_t*)rule, &t) != RET_OK) {
          tk_object_unref((tk_object_t*)rule);
          rule = NULL;
        }
      }
    } else if (tk_str_ieq(type, BINDING_RULE_ITEMS)) {
      rule = BINDING_RULE(items_binding_create());
    } else if (tk_str_ieq(type, BINDING_RULE_CONDITION_IF)) {
      rule = BINDING_RULE(condition_binding_create());
    }
  }

  tokenizer_deinit(&t);

  return rule;
}

binding_rule_t* binding_rule_parse(const char* name, const char* value, bool_t inputable) {
  tokenizer_t t;
  const char* k = NULL;
  const char* v = NULL;
  char key[TK_NAME_LEN + 1];
  binding_rule_t* rule = NULL;
  return_value_if_fail(name != NULL && value != NULL, NULL);

  rule = binding_rule_create(name, inputable);
  return_value_if_fail(rule != NULL, NULL);

  if (tokenizer_init_ex(&t, value, strlen(value), " \r\n{}", "=,") == NULL) {
    tk_object_unref(TK_OBJECT(rule));
    return NULL;
  }

  k = tokenizer_next_expr_until(&t, ",}");
  if (k != NULL) {
    ENSURE(tk_object_set_prop_str(TK_OBJECT(rule), k, NULL) == RET_OK);
    while (tokenizer_has_more(&t)) {
      k = tokenizer_next(&t);
      while (k && *k == ',' && tokenizer_has_more(&t)) {
        k = tokenizer_next(&t);
      }
      tk_strncpy(key, k, TK_NAME_LEN);
      key[TK_NAME_LEN] = '\0';

      v = tokenizer_next_expr_until(&t, ",}");
      if (v != NULL && *v == '=') {
        v += 1;
      }

      ENSURE(tk_object_set_prop_str(TK_OBJECT(rule), key, v) == RET_OK);
    }
  }

  tokenizer_deinit(&t);

  return rule;
}
