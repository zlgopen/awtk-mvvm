/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "conf_io/app_conf.h"
#include "tkc/dl.h"
#include "mvvm_app.inc"

#ifdef WITH_JERRYSCRIPT
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#endif

#define MAX_ARGV 10

static const char* s_start_page = "home_page";
static const char* s_system_bar = NULL;
static const char* s_system_bar_bottom = NULL;
static const char* s_res_root = NULL;
static const char* s_custom_widgets_dll_path = NULL;
static const char* s_render_mode = "OpenGL";
static const char* s_run_default = NULL;
static const char* s_is_revert = NULL;

#define ON_CMD_LINE(argc, argv)                                                                \
  {                                                                                            \
    const char* usage =                                                                        \
        "Usage: %s [start_page=xxx] [system_bar=xxx] [bottom_system_bar=xxx] "                 \
        "[res_root=xxx] [lcd_w=800] [lcd_h=480] "                                              \
        "[custom_widgets_dll_path=xxx] [render_mode=xxx] [run_default=xxx] [is_revert=xxx]\n"; \
    if (argc >= 2) {                                                                           \
      char key[TK_NAME_LEN + 1];                                                               \
      int i = 1;                                                                               \
                                                                                               \
      for (i = 1; i < argc; i++) {                                                             \
        const char* p = argv[i];                                                               \
        const char* val = strstr(p, "=");                                                      \
        uint32_t len = (uint32_t)(val - p);                                                    \
                                                                                               \
        if (*(argv[i]) == '\0') {                                                              \
          continue;                                                                            \
        }                                                                                      \
                                                                                               \
        if (val == NULL || val == p || *(val + 1) == '\0' || len > TK_NAME_LEN) {              \
          log_debug(usage, argv[0]);                                                           \
          exit(0);                                                                             \
        }                                                                                      \
                                                                                               \
        tk_strncpy(key, p, len);                                                               \
        key[len] = '\0';                                                                       \
                                                                                               \
        if (tk_str_icmp(key, "res_root") == 0) {                                               \
          s_res_root = val + 1;                                                                \
        } else if (tk_str_icmp(key, "start_page") == 0) {                                      \
          s_start_page = val + 1;                                                              \
        } else if (tk_str_icmp(key, "system_bar") == 0) {                                      \
          s_system_bar = val + 1;                                                              \
        } else if (tk_str_icmp(key, "bottom_system_bar") == 0) {                               \
          s_system_bar_bottom = val + 1;                                                       \
        } else if (tk_str_icmp(key, "lcd_w") == 0) {                                           \
          lcd_w = tk_atoi(val + 1);                                                            \
        } else if (tk_str_icmp(key, "lcd_h") == 0) {                                           \
          lcd_h = tk_atoi(val + 1);                                                            \
        } else if (tk_str_icmp(key, "custom_widgets_dll_path") == 0) {                         \
          s_custom_widgets_dll_path = val + 1;                                                 \
        } else if (tk_str_icmp(key, "render_mode") == 0) {                                     \
          s_render_mode = val + 1;                                                             \
        } else if (tk_str_icmp(key, "run_default") == 0) {                                     \
          s_run_default = val + 1;                                                             \
        } else if (tk_str_icmp(key, "is_revert") == 0) {                                       \
          s_is_revert = val + 1;                                                               \
        } else {                                                                               \
          log_debug(usage, argv[0]);                                                           \
        }                                                                                      \
      }                                                                                        \
    }                                                                                          \
  }

static ret_t __get_default_res_root(char path[MAX_PATH + 1]) {
  char app_root[MAX_PATH + 1];
  if (path_app_root(app_root) == RET_OK) {
    return path_build(path, MAX_PATH, app_root, "res_default", NULL);
  }
  return RET_FAIL;
}

static bool_t __str_end_with(const char* str, const char* text) {
  size_t len1 = strlen(str);
  size_t len2 = strlen(text);
  return (len1 >= len2 && strncmp(str + len1 - len2, text, len2) == 0);
}

typedef ret_t (*custom_widget_register_t)(void);
typedef const char* (*custom_widget_supported_render_mode_t)(void);
static ret_t custom_widget_dlls_register(const char* dirname, const char* render_mode) {
  str_t funcname;
  fs_item_t item;
  char path[MAX_PATH] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), dirname);
  return_value_if_fail(dir != NULL, RET_FAIL);

  str_init(&funcname, 0);

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_dir) {
      if (!tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
        path_build(path, MAX_PATH, dirname, item.name, NULL);
        custom_widget_dlls_register(path, render_mode);
      }
    } else {
      tk_dl_t* dl = NULL;
      custom_widget_register_t register_func;
      custom_widget_supported_render_mode_t render_mode_func;

#ifdef WIN32
      if (item.is_reg_file && __str_end_with(item.name, ".dll")) {
        str_set(&funcname, item.name);
        str_replace(&funcname, ".dll", "");
      } else {
        continue;
      }
#elif defined LINUX
      if (item.is_reg_file && __str_end_with(item.name, ".so") &&
          tk_str_start_with(item.name, "lib")) {
        str_set(&funcname, item.name);
        str_replace(&funcname, "lib", "");
        str_replace(&funcname, ".so", "");
      } else {
        continue;
      }
#elif defined MACOS
      if (item.is_reg_file && __str_end_with(item.name, ".dylib") &&
          tk_str_start_with(item.name, "lib")) {
        str_set(&funcname, item.name);
        str_replace(&funcname, "lib", "");
        str_replace(&funcname, ".dylib", "");
      } else {
        continue;
      }
#else
      log_debug("not supported platform.\n");
      return RET_FAIL;
#endif

      path_build(path, MAX_PATH, dirname, item.name, NULL);
      dl = tk_dl_open(path);
      if (dl == NULL) {
#ifdef WIN32
        log_debug("%s opened fail, error code: %d!\n", path, GetLastError());
#else
        log_debug("%s opened fail!\n", path);
#endif
        continue;
      }

      str_append(&funcname, "_supported_render_mode");
      render_mode_func = (custom_widget_supported_render_mode_t)(tk_dl_sym(dl, funcname.str));
      if (render_mode_func != NULL) {
        const char* modes = render_mode_func();

        if (strstr(modes, render_mode) == NULL) {
          log_debug("%s not supported %s render mode \n", path, render_mode);
          continue;
        }

        str_replace(&funcname, "_supported_render_mode", "_register");
        register_func = (custom_widget_register_t)tk_dl_sym(dl, funcname.str);
        if (register_func != NULL) {
          register_func();
        } else {
          log_debug("%s not found %s!\n", path, funcname.str);
        }
      } else {
        log_debug("%s not found %s!\n", path, funcname.str);
      }
    }
  }

  str_reset(&funcname);
  return RET_OK;
}

static ret_t custom_widgets_register(void) {
  const char* path = s_custom_widgets_dll_path;
  if (path != NULL) {
    return_value_if_fail(dir_exist(path), RET_BAD_PARAMS);
    return custom_widget_dlls_register(path, s_render_mode);
  }
  return RET_OK;
}

static ret_t application_on_launch(void) {
  // 当程序初始化完成时调用，全局只触发一次。

#ifdef WITH_JERRYSCRIPT
  js_application_on_launch();
#endif

  return RET_OK;
}

static ret_t application_on_exit(void) {
  // 当程序退出时调用，全局只触发一次。

#ifdef WITH_JERRYSCRIPT
  js_application_on_exit();
#endif

  return RET_OK;
}

static ret_t application_init(void) {
  custom_widgets_register();
  application_on_launch();

  if (s_system_bar != NULL && *s_system_bar != '\0') {
    navigator_to(s_system_bar);
  }

  if (s_system_bar_bottom != NULL && *s_system_bar_bottom != '\0') {
    navigator_to(s_system_bar_bottom);
  }

  navigator_to(s_start_page);

  return RET_OK;
}

static ret_t application_exit() {
  log_debug("application_exit\n");
  application_on_exit();

  return RET_OK;
}

static ret_t assets_init(void) {
  assets_manager_t* am = assets_manager();
  const char* res_root = NULL;
  bool_t run_default = FALSE;

  if (TK_STR_IS_NOT_EMPTY(s_is_revert)) {
    bool_t is_revert = tk_atob(s_is_revert);
    if (is_revert) {
      res_root = assets_manager_get_res_root(am);
      if (dir_exist(res_root)) {
        fs_remove_dir_r(os_fs(), res_root);
      }
    }
  }

  if (TK_STR_IS_NOT_EMPTY(s_run_default)) {
    run_default = tk_atob(s_run_default);
  }

  if (!run_default) {
    if (s_res_root != NULL && *s_res_root != '\0') {
      assets_manager_set_res_root(am, s_res_root);
    }

    res_root = assets_manager_get_res_root(am);
    if (!dir_exist(res_root)) {
      run_default = TRUE;
    }
  }

  if (run_default) {
    char path[MAX_PATH + 1];
    if (__get_default_res_root(path) == RET_OK) {
      assets_manager_set_res_root(am, path);
    }
  }

#ifdef WITH_FS_RES
  assets_manager_load(am, ASSET_TYPE_FONT, "default");
  assets_manager_load(am, ASSET_TYPE_STYLE, "default");
#else
#endif

  tk_init_assets();
  return RET_OK;
}

static ret_t global_init(void) {
  mvvm_app_init();
  mvvm_app_run_default_scripts();

  return RET_OK;
}

#define GLOBAL_INIT() global_init()
#define GLOBAL_EXIT() mvvm_app_deinit()

#include "awtk_main.inc"
