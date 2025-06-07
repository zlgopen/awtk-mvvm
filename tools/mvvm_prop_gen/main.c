/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  xml mvvm prop extract
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-05-22 GaoY <gaoyuan@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_reader_file.h"
#include "common/utils.h"
#include "xml_mvvm_prop.h"

static ret_t gen_one(const char* in_filename, const char* out_filename) {
  ret_t ret = RET_OK;
  return_value_if_fail(in_filename != NULL, RET_FAIL);

  ret = xml_mvvm_prop_file_to_packfile(in_filename, out_filename);
  if (ret != RET_OK) {
    GEN_ERROR(in_filename);
    return RET_FAIL;
  }
  return RET_OK;
}

static ret_t gen_folder(const char* in_foldername, const char* out_foldername) {
  fs_item_t item;
  ret_t ret = RET_OK;
  const char* c_xml = ".xml";
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), in_foldername);

  if (!fs_dir_exist(os_fs(), out_foldername)) {
    fs_create_dir(os_fs(), out_foldername);
  }

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file && tk_strrstr(item.name, c_xml) != NULL) {
      str_t str_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);

      str_init(&str_name, 0);
      str_set(&str_name, item.name);

      path_build(in_name, MAX_PATH, in_foldername, str_name.str, NULL);

      str_replace(&str_name, ext_array, "");

      str_append(&str_name, ".json");
      path_build(out_name, MAX_PATH, out_foldername, str_name.str, NULL);

      ret = gen_one(in_name, out_name);
      str_reset(&str_name);

      if (ret == RET_FAIL) {
        GEN_ERROR(in_name);
        break;
      }

    } else if (item.is_dir && !tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
      char sub_in_folder[MAX_PATH] = {0};
      char sub_out_folder[MAX_PATH] = {0};

      path_build(sub_in_folder, MAX_PATH, in_foldername, item.name, NULL);
      path_build(sub_out_folder, MAX_PATH, out_foldername, item.name, NULL);

      ret = gen_folder(sub_in_folder, sub_out_folder);

      if (ret == RET_FAIL) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

darray_t* get_res_names_from_sources_file(const char* src_filename, darray_t* sources) {
  data_reader_factory_t* factory = data_reader_factory();
  if (factory == NULL) {
    data_reader_factory_set(data_reader_factory_create());
    data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  }

  tk_object_t* json = conf_json_load(src_filename, FALSE);
  if (json != NULL) {
    char propname[MAX_PATH] = {0};
    int32_t size = tk_object_get_prop_int(json, "sources.#size", -1);
    if (size > 0) {
      for (int32_t i = 0; i < size; i++) {
        memset(propname, 0x00, sizeof(propname));
        tk_snprintf(propname, sizeof(propname), "sources.[%d]", i);

        const char* item = tk_object_get_prop_str(json, propname);
        char* name = tk_str_copy(NULL, item);
        darray_push(sources, name);
      }
    }
  }

  return sources;
}

bool_t end_with(const char* p, const char* str) {
  return_value_if_fail(p != NULL && str != NULL && strlen(p) >= strlen(str), FALSE);

  return strncmp(p + strlen(p) - strlen(str), str, strlen(str)) == 0;
}

ret_t makesure_folder_exist(const char* folder) {
  char tmp[MAX_PATH + 1] = {0};
  if (end_with(folder, "\\") || end_with(folder, "/")) {
    tk_strncpy(tmp, folder, tk_strlen(folder) - 1);
    folder = tmp;
  }

  char p_folder[MAX_PATH + 1] = {0};
  path_dirname(folder, p_folder, MAX_PATH);
  if (!fs_dir_exist(os_fs(), p_folder)) {
    makesure_folder_exist(p_folder);
  }

  if (!fs_dir_exist(os_fs(), folder)) {
    fs_create_dir(os_fs(), folder);
  }

  return RET_OK;
}

static ret_t gen_sources(const char* src_filename, const char* in_foldername, const char* out_foldername) {
  ret_t ret = RET_OK;
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};

  if (!fs_file_exist(os_fs(), src_filename)) {
    log_debug("gen fail, sources file \"%s\" not exist!", src_filename);
    return RET_FAIL;
  }

  if (!fs_dir_exist(os_fs(), out_foldername)) {
    fs_create_dir(os_fs(), out_foldername);
  }

  darray_t sources;
  darray_init(&sources, 10, default_destroy, NULL);
  get_res_names_from_sources_file(src_filename, &sources);

  for (size_t i = 0; i < sources.size; i++) {
    str_t str_name;
    char ext_array[MAX_PATH] = {0};
    const char* name = (const char*)darray_get(&sources, i);

    path_build(in_name, MAX_PATH, in_foldername, name, NULL);
    if (!fs_file_exist(os_fs(), in_name)) {
      continue;
    }

    path_extname(name, ext_array, MAX_PATH);

    str_init(&str_name, 0);
    str_set(&str_name, name);
    str_replace(&str_name, ext_array, "");
    str_append(&str_name, ".json");
    path_build(out_name, MAX_PATH, out_foldername, str_name.str, NULL);

    char out_folder[MAX_PATH + 1] = {0};
    path_dirname(out_name, out_folder, MAX_PATH);
    makesure_folder_exist(out_folder);

    ret = gen_one(in_name, out_name);
    str_reset(&str_name);

    if (ret == RET_FAIL) {
      GEN_ERROR(in_name);
      break;
    }
  }

  darray_deinit(&sources);

  return ret;
}

int wmain(int argc, wchar_t* argv[]) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;
  const char* src_filename = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename [src_filename] \n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;
  str_t src_file;

  str_init(&in_file, 0);
  str_init(&out_file, 0);

  str_from_wstr(&in_file, argv[1]);
  str_from_wstr(&out_file, argv[2]);

  in_filename = in_file.str;
  out_filename = out_file.str;

  fs_stat_info_t in_stat_info;
  fs_stat_info_t out_stat_info;
  fs_stat(os_fs(), in_filename, &in_stat_info);
  fs_stat(os_fs(), out_filename, &out_stat_info);

  str_init(&src_file, 0);
  if (argc > 3) {
    str_from_wstr(&src_file, argv[3]);
    str_trim(&src_file, " ");
    if (!str_eq(&src_file, "")) {
      src_filename = src_file.str;
    }
  }

  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    if (src_filename != NULL) {
      gen_sources(src_filename, in_filename, out_filename);
    } else {
      gen_folder(in_filename, out_filename);
    }
  } else if (in_stat_info.is_reg_file == TRUE) {
    gen_one(in_filename, out_filename);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);
  str_reset(&src_file);

  return 0;
}

#include "common/main.inc"
