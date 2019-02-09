/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm global functions.
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "widgets/window.h"
#include "jerryscript-port.h"
#include "jerryscript-ext/handler.h"
#include "mvvm/base/model_factory.h"
#include "mvvm/jerryscript/jsobj.h"
#include "mvvm/jerryscript/mvvm_jerryscript.h"

const char* s_boot_code =
    "var ValueConverters = {};\n \
                           var ValueValidators = {};\n \
                           var console = { \n \
                            log: function(args) { \n \
                              print(args); \n \
                            } \n \
                           }; \n \
                           console.log('hello awtk'); \n \
                           ";

static model_t* model_jerryscript_create_with_window(void* args) {
  char* p = NULL;
  model_t* model = NULL;
  const char* vmodel = NULL;
  char name[TK_NAME_LEN + 5];
  widget_t* win = WIDGET(args);
  const asset_info_t* asset = NULL;
  return_value_if_fail(win != NULL, NULL);

  vmodel = widget_get_prop_str(win, WIDGET_PROP_V_MODEL, NULL);
  return_value_if_fail(vmodel != NULL, NULL);

  tk_strncpy(name, vmodel, sizeof(name) - 1);
  p = strrchr(name, '.');
  if (p != NULL) {
    *p = '\0';
  }

  asset = widget_load_asset(win, ASSET_TYPE_SCRIPT, name);
  return_value_if_fail(asset != NULL, NULL);

  model = model_jerryscript_create(name, (const char*)(asset->data), asset->size);
  widget_unload_asset(win, asset);

  return model;
}

ret_t mvvm_jerryscript_init(void) {
  jerry_init(JERRY_INIT_EMPTY);
  jerryx_handler_register_global((const jerry_char_t*)"print", jerryx_handler_print);

  return_value_if_fail(value_validator_jerryscript_init() == RET_OK, RET_FAIL);
  return_value_if_fail(value_converter_jerryscript_init() == RET_OK, RET_FAIL);

  model_factory_register(".js", model_jerryscript_create_with_window);
  jerryscript_run("boot", s_boot_code, strlen(s_boot_code));

  return RET_OK;
}

ret_t jerryscript_run(const char* name, const char* code, uint32_t code_size) {
  ret_t ret = RET_FAIL;
  jerry_value_t jscode = 0;
  jerry_value_t jsret = 0;
  return_value_if_fail(code != NULL && code_size > 0, RET_BAD_PARAMS);

  jscode = jerry_parse((const jerry_char_t*)name, strlen(name), (const jerry_char_t*)code,
                       code_size, JERRY_PARSE_NO_OPTS);
  jsret = jerry_run(jscode);
  ret = jerry_value_check(jsret);

  jerry_release_value(jscode);
  jerry_release_value(jsret);

  return ret;
}

ret_t mvvm_jerryscript_deinit(void) {
  value_converter_jerryscript_deinit();
  value_validator_jerryscript_deinit();
  jerry_cleanup();

  return RET_OK;
}
