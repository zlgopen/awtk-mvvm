/**
 * File:   calculator.c
 * Author: AWTK Develop Team
 * Brief:  calculator
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "tkc/expr_eval.h"
#include "calculator.h"

calculator_t* calculator_create(void) {
  calculator_t* calculator = TKMEM_ZALLOC(calculator_t);
  return_value_if_fail(calculator != NULL, NULL);

  str_init(&(calculator->expr), 10);
  str_set(&(calculator->expr), "");

  return calculator;
}

ret_t calculator_destroy(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, RET_BAD_PARAMS);

  str_reset(&(calculator->expr));
  TKMEM_FREE(calculator);

  return RET_OK;
}

ret_t calculator_add_char(calculator_t* calculator, const char* args) {
  return_value_if_fail(calculator != NULL, RET_BAD_PARAMS);

  str_append(&(calculator->expr), args);

  return RET_OBJECT_CHANGED;
}

bool_t calculator_can_remove_char(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, FALSE);

  return calculator->expr.size > 0;
}

ret_t calculator_remove_char(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, RET_BAD_PARAMS);

  str_pop(&(calculator->expr));

  return RET_OBJECT_CHANGED;
}

bool_t calculator_can_eval(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, FALSE);

  return calculator->expr.size > 0;
}

ret_t calculator_eval(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, RET_BAD_PARAMS);

  str_from_float(&(calculator->expr), tk_expr_eval(calculator->expr.str));

  return RET_OBJECT_CHANGED;
}

const char* calculator_get_expr(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, NULL);

  return calculator->expr.str;
}

ret_t calculator_set_expr(calculator_t* calculator, const char* expr) {
  return_value_if_fail(calculator != NULL, RET_BAD_PARAMS);

  return str_set(&(calculator->expr), expr);
}
