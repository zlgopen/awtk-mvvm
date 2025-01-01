/**
 * File:   mvvm_iotjs.h
 * Author: AWTK Develop Team
 * Brief:  mvvm iotjs global functions.
 *
 * Copyright (c) 2019 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-02-14 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "tkc/types_def.h"

#ifdef WITH_IOTJS
#include "tkc/utils.h"
#include "base/main_loop.h"
#include "mvvm/jerryscript/jerry_script_helper.h"

#include "iotjs_js.h"
#include "iotjs_env.h"
#include "iotjs_module.h"
#include "iotjs_binding.h"
#include "iotjs_debuglog.h"
#include "event_source_iotjs.h"

extern bool iotjs_initialize(iotjs_environment_t* env);

ret_t mvvm_iotjs_get_iotjs_require(char* name, jerry_value_t* require) {
  ret_t ret = RET_FAIL;
  jerry_value_t process = iotjs_module_get("process");
  *require = jerry_create_undefined();

  jerry_value_t modules = iotjs_jval_get_property(process, "module");
  if (iotjs_jval_as_object(modules)) {
    jerry_value_t load_fun = iotjs_jval_get_property(modules, "load");
    if(iotjs_jval_as_function(load_fun)) {
      jerry_value_t jname = jerry_create_string((const jerry_char_t*)(name));
      jerry_value_t args[] = { jname };
      *require = jerry_call_function(load_fun, modules, args, sizeof(args) / sizeof(jerry_value_t));
      jerry_release_value(jname);
      ret = RET_OK;
    }
    jerry_release_value(load_fun);
  }
  jerry_release_value(modules);
  return ret;
}

static bool_t mvvm_iotjs_object_init(iotjs_environment_t* env) {

  jerry_value_t jmain = jerry_exec_snapshot((const uint32_t*)iotjs_js_modules_s,
                                            iotjs_js_modules_l, module_iotjs_idx,
                                            JERRY_SNAPSHOT_EXEC_ALLOW_STATIC, NULL);
  
  if (jerry_value_is_error(jmain)) {
    jerry_value_t errval = jerry_get_value_from_error(jmain, false);
    iotjs_uncaught_exception(errval);
    jerry_release_value(errval);
    assert(!"Load iotjs.js fail~");
  }
  jerry_release_value(jmain);
  iotjs_environment_set_state(env, kRunningMain);
  return TRUE;
}

static ret_t mvvm_iotjs_event_source_manager_init() {
  event_source_t* iotjs_source = event_source_iotjs_create();
  return_value_if_fail(iotjs_source != NULL, RET_OOM);

  return main_loop_add_event_source(main_loop(), iotjs_source);
}

ret_t mvvm_iotjs_init(void) {

  iotjs_environment_t* env = NULL;
  char* argv[] = {"iotjs", "util"};
#if defined(JERRY_EXTERNAL_CONTEXT) && (JERRY_EXTERNAL_CONTEXT == 1)
  jerry_init_external_context();
#endif /*JERRY_EXTERNAL_CONTEXT*/

  // Initialize debug log and environments
  iotjs_debuglog_init();
  srand((unsigned)jerry_port_get_current_time());

  env = iotjs_environment_get();
  goto_error_if_fail(iotjs_environment_parse_command_line_arguments(env, ARRAY_SIZE(argv), argv));

  // Initialize IoT.js
  goto_error_if_fail(iotjs_initialize(env));

  goto_error_if_fail(mvvm_iotjs_object_init(env));

  return mvvm_iotjs_event_source_manager_init();
error :
  iotjs_terminate(env);
  return RET_FAIL;
}

ret_t mvvm_iotjs_deinit(void) {
  iotjs_environment_t* env = iotjs_environment_get();

  if (!iotjs_environment_is_exiting(env)) {
    iotjs_environment_set_state(env, kExiting);
  }

  // Ends IoT.js
  iotjs_end(env);
  iotjs_terminate(env);

  iotjs_environment_release();
  iotjs_debuglog_release();

#if defined(JERRY_EXTERNAL_CONTEXT) && (JERRY_EXTERNAL_CONTEXT == 1)
  jerry_deinit_external_context();
#endif /*JERRY_EXTERNAL_CONTEXT*/

  return RET_OK;
}

ret_t mvvm_iotjs_run(void) {
  int start_time = time_now_ms();
  iotjs_environment_t* env = iotjs_environment_get();
  // Run event loop.
  if (env->state != kRunningLoop) {
    if (env->state == kExiting) {
      log_error("iotjs error, iotjs stop operation! \r\n");
      return RET_FAIL;
    } else {
      iotjs_environment_set_state(env, kRunningLoop);
    }
  }

  bool more;
  do {
    more = uv_run(iotjs_environment_loop(env), UV_RUN_ONCE);
    more |= iotjs_process_next_tick();

    jerry_value_t ret_val = jerry_run_all_enqueued_jobs();
    if (jerry_value_is_error(ret_val)) {
      ret_val = jerry_get_value_from_error(ret_val, true);
      iotjs_uncaught_exception(ret_val);
      jerry_release_value(ret_val);
    }

    if (more == false) {
      more = uv_loop_alive(iotjs_environment_loop(env));
    }
  } while (more && (time_now_ms() - start_time < 8));

  return RET_OK;
}
#endif /*WITH_IOTJS*/
