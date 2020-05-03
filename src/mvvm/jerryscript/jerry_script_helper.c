/**
 * File:   jerry_script_helper.c
 * Author: AWTK Develop Team
 * Brief:  jerry_script_helper
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
 * 2020-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <string.h>
#include "jerryscript.h"
#include "jerryscript-port.h"
#include "jerryscript-core.h"
#include "jerryscript-ext/handler.h"

#include "tkc/fs.h"
#include "tkc/str.h"
#include "tkc/mem.h"
#include "base/assets_manager.h"

#define STR_MODULES "modules"
#ifndef NDEBUG

#define JERRY_BUFFER_SIZE (40 * 1024)
#define SYNTAX_ERROR_CONTEXT_SIZE 2
#define JERRY_SNAPSHOT_BUFFER_SIZE (JERRY_BUFFER_SIZE / sizeof(uint32_t))

static uint8_t buffer[JERRY_BUFFER_SIZE];

void jerry_script_print_error(jerry_value_t error_value) /**< error value */
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
} /* jerry_script_print_error */
#else
void jerry_script_print_error(jerry_value_t error_value) /**< error value */
{
  log_debug("error\n");
}
#endif

static ret_t awtk_jerryscript_get_module(const char* name, jerry_value_t* module) {
  ret_t ret = RET_OK;
  jerry_value_t obj = jerry_get_global_object();
  jerry_value_t prop_modules = jerry_create_string((const jerry_char_t*)"modules");
  jerry_value_t modules = jerry_get_property(obj, prop_modules);
  jerry_value_t prop_name = jerry_create_string((const jerry_char_t*)name);
  jerry_value_t has_prop_js = jerry_has_property(modules, prop_name);

  if (jerry_get_boolean_value(has_prop_js)) {
    *module = jerry_get_property(modules, prop_name);
    ret = RET_OK;
  } else {
    ret = RET_FAIL;
  }

  jerry_release_value(has_prop_js);
  jerry_release_value(prop_name);
  jerry_release_value(modules);
  jerry_release_value(prop_modules);
  jerry_release_value(obj);

  return ret;
}

static ret_t awtk_jerryscript_wrap_mudule(str_t* str, const char* filename, const char* script,
                                          uint32_t size) {
  str_append_more(str, "this." STR_MODULES "[\"", filename, "\"] = {};\n", NULL);
  str_append(str, "(function module(exports, global) {");
  str_append_with_len(str, script, size);
  str_append_more(str, "})(this." STR_MODULES "[\"", filename, "\"], this)\n", NULL);

  return RET_OK;
}

ret_t jerry_value_check(jerry_value_t value) {
  if (jerry_value_is_error(value)) {
    jerry_value_t ret;
    ret = jerry_get_value_from_error(value, true);
    jerry_script_print_error(ret);

    return RET_FAIL;
  } else {
    return RET_OK;
  }
}
ret_t jerry_script_eval_buff(const char* script, uint32_t size, const char* filename,
                             bool_t global) {
  ret_t ret = RET_FAIL;
  jerry_value_t parsed_code;
  return_value_if_fail(script != NULL && size > 0, RET_NOT_FOUND);

  if (global) {
    parsed_code = jerry_parse(NULL, 0, (jerry_char_t*)script, size, JERRY_PARSE_NO_OPTS);
    log_debug("load %s into global\n", filename);
  } else {
    str_t str;
    str_init(&str, size + 100);
    awtk_jerryscript_wrap_mudule(&str, filename, script, size);
    parsed_code = jerry_parse(NULL, 0, (jerry_char_t*)str.str, str.size, JERRY_PARSE_NO_OPTS);
    str_reset(&str);
    log_debug("load %s as module\n", filename);
  }

  if (jerry_value_check(parsed_code) == RET_OK) {
    jerry_value_t ret_value = jerry_run(parsed_code);
    ret = jerry_value_check(ret_value);
    if (ret == RET_OK) {
      jerry_release_value(ret_value);
    }
  }

  jerry_release_value(parsed_code);

  return ret;
}

ret_t jerry_script_eval_file(const char* filename, bool_t global) {
  ret_t ret = RET_FAIL;
  const asset_info_t* info = assets_manager_ref(assets_manager(), ASSET_TYPE_SCRIPT, filename);

  return_value_if_fail(info != NULL, RET_BAD_PARAMS);
  ret = jerry_script_eval_buff((const char*)info->data, info->size, filename, global);
  assets_manager_unref(assets_manager(), info);

  log_debug("jerry_script_eval_file: %s ret=%d\n", filename, ret);

  return ret;
}

jerry_value_t wrap_require(const jerry_value_t func_obj_val, const jerry_value_t this_p,
                           const jerry_value_t argv[], const jerry_length_t argc) {
  jerry_value_t jret = 0;

  if (argc >= 1) {
    char filename[MAX_PATH + 1];
    jerry_size_t size = jerry_get_utf8_string_size(argv[0]);
    assert(size < MAX_PATH);
    jerry_string_to_utf8_char_buffer(argv[0], (jerry_char_t*)filename, size);
    filename[size] = '\0';

    if (awtk_jerryscript_get_module(filename, &jret) == RET_OK) {
      log_debug("load %s success(cache)\n", filename);
      return jret;
    }

    if (jerry_script_eval_file(filename, FALSE) == RET_OK) {
      log_debug("load %s success\n", filename);
      if (awtk_jerryscript_get_module(filename, &jret) == RET_OK) {
        return jret;
      }
    } else {
      log_debug("load %s fail\n", filename);
    }
  }

  return jerry_create_null();
}

#define STR_BOOT_JS "var exports = {};\nthis." STR_MODULES "={};\n"

ret_t jerry_script_register_builtins(void) {
  jerryx_handler_register_global((const jerry_char_t*)"gc", jerryx_handler_gc);
  jerryx_handler_register_global((const jerry_char_t*)"print", jerryx_handler_print);
  jerryx_handler_register_global((const jerry_char_t*)"require", wrap_require);
  jerry_script_eval_buff(STR_BOOT_JS, strlen(STR_BOOT_JS), "boot.js", TRUE);

  return RET_OK;
}

