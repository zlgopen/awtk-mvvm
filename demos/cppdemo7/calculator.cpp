
/**
 * File:  calculator.cpp
 * Author: AWTK Develop Team
 * Brief:  calculator
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/expr_eval.h"
#include "tkc/utils.h"
#include "calculator.h"

Calculator::Calculator() {
}

Calculator::~Calculator() {
}

ret_t Calculator::AddChar(char c) {
  char str[2] = {c, 0};

  this->expr += str;

  return RET_OBJECT_CHANGED;
}

ret_t Calculator::RemoveChar() {
  this->expr.erase(this->expr.size() - 1);
  return RET_OBJECT_CHANGED;
}

bool_t Calculator::CanRemoveChar() const {
  return this->expr.size() > 0;
}

ret_t Calculator::Eval() {
  char str[32];
  double value = tk_expr_eval(this->expr.c_str());
  tk_snprintf(str, sizeof(str), "%g", value);

  this->expr = str;

  return RET_OBJECT_CHANGED;
}

bool_t Calculator::CanEval() const {
  return this->expr.size() > 0;
}
