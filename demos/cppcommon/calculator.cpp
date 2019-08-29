
/**
 * File:  calculator.cpp
 * Author: AWTK Develop Team
 * Brief:  calculator view model 
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "calculator.hpp"

Calculator::Calculator(navigator_request_t* request) : ViewModel(request) {
  str_init(&(this->expr), 10);
}

Calculator::~Calculator() {
  str_reset(&(this->expr));
}
  
ret_t Calculator::Exec(const char* name, const char* args) {
  if (tk_str_eq("add_char", name)) {
    str_append(&(this->expr), args);
    return RET_OBJECT_CHANGED;
  } else if (tk_str_eq("remove_char", name)) {
    str_pop(&(this->expr));
    return RET_OBJECT_CHANGED;
  } else if (tk_str_eq("eval", name)) {
    str_from_float(&(this->expr), tk_expr_eval(this->expr.str));
    return RET_OBJECT_CHANGED;
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

bool_t Calculator::CanExec(const char* name, const char* args) const {
  if (tk_str_eq("add_char", name)) {
    return TRUE;
  } else if (tk_str_eq("remove_char", name)) {
    return this->expr.size > 0;
  } else if (tk_str_eq("eval", name)) {
    return this->expr.size > 0;
  } else {
    return FALSE;
  }
}

ret_t Calculator::GetProp(const char* name, value_t* v) const {
  if (tk_str_eq("expr", name)) {
    value_set_str(v, this->expr.str);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

ret_t Calculator::SetProp(const char* name, const value_t* v) {
  if (tk_str_eq("expr", name)) {
    str_from_value(&(this->expr), v);
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

view_model_t* calculator_view_model_create(navigator_request_t* req) {
  return view_model_cpp_create(new Calculator(req));
}

