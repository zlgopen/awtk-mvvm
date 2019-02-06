/**
 * File:   jsobj.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript wrapper
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
 * 2019-02-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "jsobj.h"
#include "tkc/utils.h"
#include "jerryscript-port.h"
#include "jerryscript-ext/handler.h"

#if !defined(NDEBUG)
#define JERRY_BUFFER_SIZE (40 * 1024)
#define SYNTAX_ERROR_CONTEXT_SIZE 2
#define JERRY_SNAPSHOT_BUFFER_SIZE (JERRY_BUFFER_SIZE / sizeof(uint32_t))

static uint8_t buffer[JERRY_BUFFER_SIZE];

static void print_unhandled_exception(jerry_value_t error_value) /**< error value */
{
  assert(!jerry_value_is_error(error_value));

  jerry_char_t err_str_buf[256];

  if (jerry_value_is_object(error_value)) {
    jerry_value_t stack_str = jerry_create_string((const jerry_char_t*)"stack");
    jerry_value_t backtrace_val = jerry_get_property(error_value, stack_str);
    jerry_release_value(stack_str);

    if (!jerry_value_is_error(backtrace_val) && jerry_value_is_array(backtrace_val)) {
      printf("Exception backtrace:\n");

      uint32_t length = jerry_get_array_length(backtrace_val);

      /* This length should be enough. */
      if (length > 32) {
        length = 32;
      }

      for (uint32_t i = 0; i < length; i++) {
        jerry_value_t item_val = jerry_get_property_by_index(backtrace_val, i);

        if (!jerry_value_is_error(item_val) && jerry_value_is_string(item_val)) {
          jerry_size_t str_size = jerry_get_string_size(item_val);

          if (str_size >= 256) {
            printf("%3u: [Backtrace string too long]\n", i);
          } else {
            jerry_size_t string_end = jerry_string_to_char_buffer(item_val, err_str_buf, str_size);
            assert(string_end == str_size);
            err_str_buf[string_end] = 0;

            printf("%3u: %s\n", i, err_str_buf);
          }
        }

        jerry_release_value(item_val);
      }
    }
    jerry_release_value(backtrace_val);
  }

  jerry_value_t err_str_val = jerry_value_to_string(error_value);
  jerry_size_t err_str_size = jerry_get_string_size(err_str_val);

  if (err_str_size >= 256) {
    const char msg[] = "[Error message too long]";
    err_str_size = sizeof(msg) / sizeof(char) - 1;
    memcpy(err_str_buf, msg, err_str_size + 1);
  } else {
    jerry_size_t string_end = jerry_string_to_char_buffer(err_str_val, err_str_buf, err_str_size);
    assert(string_end == err_str_size);
    err_str_buf[string_end] = 0;

    if (jerry_is_feature_enabled(JERRY_FEATURE_ERROR_MESSAGES) &&
        jerry_get_error_type(error_value) == JERRY_ERROR_SYNTAX) {
      unsigned int err_line = 0;
      unsigned int err_col = 0;

      /* 1. parse column and line information */
      for (jerry_size_t i = 0; i < string_end; i++) {
        if (!strncmp((char*)(err_str_buf + i), "[line: ", 7)) {
          i += 7;

          char num_str[8];
          unsigned int j = 0;

          while (i < string_end && err_str_buf[i] != ',') {
            num_str[j] = (char)err_str_buf[i];
            j++;
            i++;
          }
          num_str[j] = '\0';

          err_line = (unsigned int)strtol(num_str, NULL, 10);

          if (strncmp((char*)(err_str_buf + i), ", column: ", 10)) {
            break; /* wrong position info format */
          }

          i += 10;
          j = 0;

          while (i < string_end && err_str_buf[i] != ']') {
            num_str[j] = (char)err_str_buf[i];
            j++;
            i++;
          }
          num_str[j] = '\0';

          err_col = (unsigned int)strtol(num_str, NULL, 10);
          break;
        }
      } /* for */

      if (err_line != 0 && err_col != 0) {
        unsigned int curr_line = 1;

        bool is_printing_context = false;
        unsigned int pos = 0;

        /* 2. seek and print */
        while (buffer[pos] != '\0') {
          if (buffer[pos] == '\n') {
            curr_line++;
          }

          if (err_line < SYNTAX_ERROR_CONTEXT_SIZE ||
              (err_line >= curr_line && (err_line - curr_line) <= SYNTAX_ERROR_CONTEXT_SIZE)) {
            /* context must be printed */
            is_printing_context = true;
          }

          if (curr_line > err_line) {
            break;
          }

          if (is_printing_context) {
            jerry_port_log(JERRY_LOG_LEVEL_ERROR, "%c", buffer[pos]);
          }

          pos++;
        }

        jerry_port_log(JERRY_LOG_LEVEL_ERROR, "\n");

        while (--err_col) {
          jerry_port_log(JERRY_LOG_LEVEL_ERROR, "~");
        }

        jerry_port_log(JERRY_LOG_LEVEL_ERROR, "^\n");
      }
    }
  }

  jerry_port_log(JERRY_LOG_LEVEL_ERROR, "Script Error: %s\n", err_str_buf);
  jerry_release_value(err_str_val);
} /* print_unhandled_exception */
#else
static void print_unhandled_exception(jerry_value_t error_value) /**< error value */
{
  log_debug("print_unhandled_exception\n");
}
#endif /*print_unhandled_exception*/

ret_t jerry_value_check(jerry_value_t value) {
  if (jerry_value_is_error(value)) {
    jerry_value_t ret;
    ret = jerry_get_value_from_error(value, true);
    print_unhandled_exception(ret);

    return RET_FAIL;
  } else {
    return RET_OK;
  }
}

bool_t jsobj_has_prop(jerry_value_t obj, const char* name) {
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t has_prop_js = jerry_has_property(obj, prop_name);
  bool_t has_prop = (bool_t)jerry_get_boolean_value(has_prop_js);
  jerry_release_value(has_prop_js);
  jerry_release_value(prop_name);

  return has_prop;
}

jerry_value_t jsobj_get_model(const char* name) {
  jerry_value_t global_obj = jerry_get_global_object();
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t prop_value = jerry_get_property(global_obj, prop_name);
  jerry_release_value(prop_name);

  if (jerry_value_is_null(prop_value) || jerry_value_is_error(prop_value)) {
    jerry_release_value(prop_value);
    return global_obj;
  } else {
    jerry_release_value(global_obj);
    return prop_value;
  }
}

jerry_value_t jsobj_get_prop_value(jerry_value_t obj, const char* name) {
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t prop_value = jerry_get_property(obj, prop_name);
  jerry_release_value(prop_name);
  jerry_value_check(prop_value);

  return prop_value;
}

ret_t jsobj_set_prop(jerry_value_t obj, const char* name, const value_t* v, str_t* temp) {
  jerry_value_t prop_value = jerry_value_from_value(v, temp);
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);

  jerry_set_property(obj, prop_name, prop_value);

  jerry_release_value(prop_name);
  jerry_release_value(prop_value);

  return RET_OK;
}

char* jerry_get_utf8_value(jerry_value_t value, str_t* temp) {
  jerry_size_t string_size = jerry_get_utf8_string_size(value);
  return_value_if_fail(str_extend(temp, string_size) == RET_OK, NULL);

  jerry_string_to_utf8_char_buffer(value, (jerry_char_t*)temp->str, string_size);
  temp->size = string_size;
  temp->str[string_size] = '\0';

  return temp->str;
}

ret_t jsobj_get_prop(jerry_value_t obj, const char* name, value_t* v, str_t* temp) {
  ret_t ret = RET_FAIL;
  jerry_value_t prop_value = jsobj_get_prop_value(obj, name);
  ret = jerry_value_to_value(prop_value, v, temp);
  jerry_release_value(prop_value);

  return ret;
}

ret_t jsobj_exec(jerry_value_t obj, const char* name, const char* args) {
  ret_t ret = RET_FAIL;

  if (jsobj_has_prop(obj, name)) {
    jerry_value_t func = jsobj_get_prop_value(obj, name);
    if (jerry_value_is_function(func)) {
      jerry_value_t jsargs = jerry_create_string((const jerry_char_t*)args);
      jerry_value_t jsret = jerry_call_function(func, obj, &jsargs, 1);
      ret = (ret_t)jerry_value_to_number(jsret);
      jerry_release_value(func);
      jerry_release_value(jsret);
      jerry_release_value(jsargs);
    } else {
      ret = RET_NOT_IMPL;
      log_debug("not function %s\n", name);
    }
  } else {
    ret = RET_NOT_IMPL;
  }

  return ret;
}

bool_t jsobj_can_exec(jerry_value_t obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  char jsname[TK_NAME_LEN + 1];
  tk_snprintf(jsname, TK_NAME_LEN, "can%s", name);
  if (islower(jsname[3])) {
    jsname[3] = toupper(jsname[3]);
  }

  if (jsobj_has_prop(obj, jsname)) {
    jerry_value_t func = jsobj_get_prop_value(obj, jsname);

    if (jerry_value_is_function(func)) {
      jerry_value_t jsargs = jerry_create_string((const jerry_char_t*)args);
      jerry_value_t jsret = jerry_call_function(func, obj, &jsargs, 1);
      ret = jerry_value_to_boolean(jsret);
      jerry_release_value(func);
      jerry_release_value(jsret);
      jerry_release_value(jsargs);
    } else {
      ret = FALSE;
      log_debug("not function %s\n", name);
    }
  } else {
    ret = jsobj_has_prop_func(obj, name);
  }

  return ret;
}

bool_t jsobj_has_prop_func(jerry_value_t obj, const char* name) {
  jerry_value_t value = jsobj_get_prop_value(obj, name);

  bool_t ret = jerry_value_is_function(value);

  jerry_release_value(value);

  return ret;
}

ret_t jerry_value_to_value(jerry_value_t value, value_t* v, str_t* temp) {
  ret_t ret = RET_NOT_IMPL;

  if (!jerry_value_is_error(value)) {
    if (jerry_value_is_string(value)) {
      char* str = jerry_get_utf8_value(value, temp);
      if (str != NULL) {
        value_set_str(v, str);
        ret = RET_OK;
      } else {
        ret = RET_OOM;
      }
    } else if (jerry_value_is_number(value)) {
      double raw_value = jerry_get_number_value(value);

      if (floor(raw_value) == (int32_t)raw_value) {
        value_set_int32(v, (int32_t)raw_value);
      } else {
        value_set_double(v, raw_value);
      }
      ret = RET_OK;
    } else if (jerry_value_is_boolean(value)) {
      bool_t raw_value = jerry_get_boolean_value(value);
      value_set_bool(v, raw_value);
      ret = RET_OK;
    } else {
      void* p = NULL;
      if (jerry_get_object_native_pointer(value, &p, NULL)) {
        value_set_pointer(v, p);
        ret = RET_OK;
      } else {
        log_debug("not supported yet.\n");
      }
    }
  }

  return ret;
}

jerry_value_t jerry_value_from_value(const value_t* v, str_t* temp) {
  jerry_value_t value;

  if (v->type == VALUE_TYPE_STRING) {
    value = jerry_create_string((const jerry_char_t*)value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    if (temp != NULL) {
      str_from_value(temp, v);
      value = jerry_create_string((const jerry_char_t*)temp->str);
    } else {
      value = jerry_create_string((const jerry_char_t*)"");
    }
  } else if (v->type == VALUE_TYPE_POINTER) {
    value = jerry_create_object();
    jerry_set_object_native_pointer(value, (void*)value_pointer(v), NULL);
  } else {
    value = jerry_create_number(value_float(v));
  }

  return value;
}

void* jerry_value_to_pointer(jerry_value_t value) {
  value_t v;
  value_set_pointer(&v, NULL);
  jerry_value_to_value(value, &v, NULL);

  return value_pointer(&v);
}

jerry_value_t jerry_value_from_pointer(void* ptr) {
  value_t v;
  value_set_pointer(&v, ptr);

  return jerry_value_from_value(&v, NULL);
}

ret_t jsobj_set_prop_pointer(jerry_value_t obj, const char* name, void* ptr) {
  value_t v;
  value_set_pointer(&v, ptr);

  return jsobj_set_prop(obj, name, &v, NULL);
}
