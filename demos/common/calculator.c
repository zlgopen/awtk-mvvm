#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "calculator.h"
#include "tkc/expr_eval.h"

/***************calculator***************/;
calculator_t* calculator_create(void) {
  calculator_t* calculator = TKMEM_ZALLOC(calculator_t);
  return_value_if_fail(calculator != NULL, NULL);

  str_init(&(calculator->expr), 10);

  return calculator;
}

static ret_t calculator_destroy(calculator_t* calculator) {
  return_value_if_fail(calculator != NULL, RET_BAD_PARAMS);

  str_reset(&(calculator->expr));

  TKMEM_FREE(calculator);

  return RET_OK;
}

static ret_t calculator_add_char(calculator_t* calculator, const char* args) {
  str_append(&(calculator->expr), args);
  return RET_OBJECT_CHANGED;
}

static bool_t calculator_can_exec_remove_char(calculator_t* calculator, const char* args) {
  return calculator->expr.size > 0;
}

static ret_t calculator_remove_char(calculator_t* calculator, const char* args) {
  str_pop(&(calculator->expr));
  return RET_OBJECT_CHANGED;
}

static bool_t calculator_can_exec_eval(calculator_t* calculator, const char* args) {
  return calculator->expr.size > 0;
}

static ret_t calculator_eval(calculator_t* calculator, const char* args) {
  str_from_float(&(calculator->expr), tk_expr_eval(calculator->expr.str));
  return RET_OBJECT_CHANGED;
}

/***************calculator_view_model***************/

static ret_t calculator_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  calculator_view_model_t* vm = (calculator_view_model_t*)(obj);
  calculator_t* calculator = vm->calculator;

  if (tk_str_eq("expr", name)) {
    str_from_value(&(calculator->expr), v);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static ret_t calculator_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  calculator_view_model_t* vm = (calculator_view_model_t*)(obj);
  calculator_t* calculator = vm->calculator;

  if (tk_str_eq("expr", name)) {
    value_set_str(v, calculator->expr.str);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static bool_t calculator_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  calculator_view_model_t* vm = (calculator_view_model_t*)(obj);
  calculator_t* calculator = vm->calculator;

  if (tk_str_eq("add_char", name)) {
    return TRUE;
  } else if (tk_str_eq("remove_char", name)) {
    return calculator_can_exec_remove_char(calculator, args);
  } else if (tk_str_eq("eval", name)) {
    return calculator_can_exec_eval(calculator, args);
  } else {
    return FALSE;
  }
}

static ret_t calculator_view_model_exec(object_t* obj, const char* name, const char* args) {
  calculator_view_model_t* vm = (calculator_view_model_t*)(obj);
  calculator_t* calculator = vm->calculator;

  if (tk_str_eq("add_char", name)) {
    return calculator_add_char(calculator, args);
  } else if (tk_str_eq("remove_char", name)) {
    return calculator_remove_char(calculator, args);
  } else if (tk_str_eq("eval", name)) {
    return calculator_eval(calculator, args);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

static ret_t calculator_view_model_on_destroy(object_t* obj) {
  calculator_view_model_t* vm = (calculator_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  calculator_destroy(vm->calculator);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_calculator_view_model_vtable = {
    .type = "calculator",
    .desc = "a simple calculator",
    .size = sizeof(calculator_view_model_t),
    .exec = calculator_view_model_exec,
    .can_exec = calculator_view_model_can_exec,
    .get_prop = calculator_view_model_get_prop,
    .set_prop = calculator_view_model_set_prop,
    .on_destroy = calculator_view_model_on_destroy};

view_model_t* calculator_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_calculator_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  calculator_view_model_t* calculator_view_model = (calculator_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  calculator_view_model->calculator = calculator_create();
  ENSURE(calculator_view_model->calculator != NULL);

  return vm;
}
