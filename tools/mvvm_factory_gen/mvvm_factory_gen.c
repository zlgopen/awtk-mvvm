#include "tkc/mem.h"
#include "tkc/str.h"
#include "tkc/asset_info.h"
#include "tkc/fs.h"
#include "base/enums.h"
#include "common/utils.h"

#ifdef WITH_JERRYSCRIPT

#include "jerryscript.h"

#if defined(JERRY_EXTERNAL_CONTEXT) && (JERRY_EXTERNAL_CONTEXT == 1)
#include "jerryscript-port-default.h"

static void* jerry_malloc_context(size_t size, void* cb_data_p) {
  (void)cb_data_p;
  return TKMEM_ALLOC(size);
}

static void jerry_init_external_context(void) {
  jerry_context_t* ctx;
  ctx = jerry_create_context(JERRY_GLOBAL_HEAP_SIZE * 1024, jerry_malloc_context, NULL);
  jerry_port_default_set_current_context(ctx);
}

static void jerry_deinit_external_context(void) {
  jerry_context_t* ctx = jerry_port_get_current_context();
  TKMEM_FREE(ctx);
}
#endif /*JERRY_EXTERNAL_CONTEXT*/

ret_t jerry_script_init(void) {
  static bool_t s_jerryscript_inited = FALSE;

  if (!s_jerryscript_inited) {
#if defined(JERRY_EXTERNAL_CONTEXT) && (JERRY_EXTERNAL_CONTEXT == 1)
    jerry_init_external_context();
#endif /*JERRY_EXTERNAL_CONTEXT*/

    jerry_init(JERRY_INIT_EMPTY);

    s_jerryscript_inited = TRUE;
  }

  return RET_OK;
}

ret_t jerry_script_deinit(void) {
  jerry_cleanup();

#if defined(JERRY_EXTERNAL_CONTEXT) && (JERRY_EXTERNAL_CONTEXT == 1)
  jerry_deinit_external_context();
#endif /*JERRY_EXTERNAL_CONTEXT*/

  return RET_OK;
}

ret_t output_snapshot_c_source(const char* filename, const char* prefix, const char* name,
                               uint8_t* buff, uint32_t size) {
  uint32_t i = 0;
  fs_file_t* ft = NULL;
  char str[TK_NAME_LEN + 1];
  char var_name[2 * TK_NAME_LEN + 1];
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  log_debug("filename=%s prefix=%s name=%s size=%u\n", filename, prefix, name, size);

  ft = fs_open_file(os_fs(), filename, "wb+");
  if (ft != NULL) {
    tk_snprintf(var_name, 2 * TK_NAME_LEN, "%s_%s", prefix ? prefix : "", name);
    fs_file_printf(ft, "TK_CONST_DATA_ALIGN(static unsigned char %s[]) = {", var_name);
    for (i = 0; i < size; i++) {
      if ((i % 20) == 0) {
        fs_file_printf(ft, "\n");
      }
      fs_file_printf(ft, "0x%02x,", (int)(buff[i]));
    }

    fs_file_printf(ft, "};/*%u*/\n", size);
    fs_file_close(ft);

    return RET_OK;
  }

  return RET_FAIL;
}

#endif /*WITH_JERRYSCRIPT*/

int main(int argc, char** argv) {
  const char* in_filename = NULL;
  const char* out_dirname = NULL;
  const char* script = NULL;
  const key_type_value_t* kv = asset_type_find_by_value(ASSET_TYPE_SCRIPT);
  uint32_t script_size = 0;
  uint32_t data_size = 0;

  if (argc < 3) {
    printf("Usage: %s in_filename out_dirname\n", argv[0]);
  }

  in_filename = argc < 2 ? "./tools/mvvm_factory_gen/mvvm_factory.js" : argv[1];
  out_dirname = argc < 3 ? "./src/mvvm/jerryscript" : argv[2];

  script = (jerry_char_t*)read_file(in_filename, &script_size);
  if (script != NULL) {
    str_t path;

    str_init(&path, 0);

#if defined(WITH_JERRYSCRIPT) && defined(WITH_JERRYSCRIPT_SNAPSHOT)
    uint32_t* data = TKMEM_ZALLOCN(uint32_t, 1024 * 1024);

    if (data != NULL) {
      jerry_value_t parse_result;

      jerry_script_init();
      parse_result = jerry_parse(script, script_size, NULL);

      if (!jerry_value_is_error(parse_result)) {
        jerry_value_t generate_result = jerry_generate_snapshot(parse_result, 0, data, 1024 * 1024);

        if (!jerry_value_is_error(generate_result)) {
          data_size = (uint32_t)jerry_get_number_value(generate_result);

          if (data_size > 0) {
            str_set(&path, out_dirname);
            str_append(&path, "/mvvm_factory.snapshot");

            output_snapshot_c_source(path.str, kv->name, "mvvm_factory", (uint8_t*)data,
                                     data_size * sizeof(uint32_t));
            printf("filename=%s size=%u\n", path.str, data_size);
          }

          generate_result =
              jerry_exec_snapshot(data, data_size, 0, JERRY_SNAPSHOT_EXEC_ALLOW_STATIC, NULL);
        } else {
          log_debug("snapshot generate error\n");
        }
        jerry_release_value(generate_result);
      }

      jerry_release_value(parse_result);
      jerry_script_deinit();

      TKMEM_FREE(data);
    }
#else
    log_debug(
        "not supported to generete snapshot, please rebuild with WITH_JS and WITH_JS_SNAPSHOT.\n");
#endif

    str_set(&path, out_dirname);
    str_append(&path, "/mvvm_factory.res");

    data_size = strlen(script);
    output_c_source(path.str, "default", kv->name, "mvvm_factory", (uint8_t*)script, data_size);
    printf("filename=%s size=%u\n", path.str, data_size);

    TKMEM_FREE(script);
  }

  return 0;
}
