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
#include "common/utils.h"
#include "xml_mvvm_prop.h"

static int gen_one(const char* in_filename, const char* out_filename) {
  ret_t ret = RET_OK;
  return_value_if_fail(in_filename != NULL && out_filename != NULL, RET_FAIL);

  ret = xml_mvvm_prop_file_to_packfile(in_filename, out_filename);
  if (ret != RET_OK) {
    GEN_ERROR(in_filename);
    return RET_FAIL;
  }
  return RET_OK;
}

static int gen_folder(const char* in_filename, const char* out_filename) {
  fs_item_t item;
  ret_t ret = RET_OK;
  const char* c_xml = ".xml";
  char in_name[MAX_PATH] = {0};
  char out_name[MAX_PATH] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), in_filename);

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_reg_file && tk_strrstr(item.name, c_xml) != NULL) {
      str_t str_name;
      char ext_array[MAX_PATH] = {0};
      path_extname(item.name, ext_array, MAX_PATH);

      str_init(&str_name, 0);
      str_set(&str_name, item.name);

      path_build(in_name, MAX_PATH, in_filename, str_name.str, NULL);

      str_replace(&str_name, ext_array, "");

      str_append(&str_name, ".json");
      path_build(out_name, MAX_PATH, out_filename, str_name.str, NULL);

      ret = gen_one(in_name, out_name);
      str_reset(&str_name);

      if (ret == RET_FAIL) {
        GEN_ERROR(in_name);
        break;
      }

    } else if (item.is_dir && !tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
      char sub_in_folder[MAX_PATH] = {0};
      char sub_out_folder[MAX_PATH] = {0};

      path_build(sub_in_folder, MAX_PATH, in_filename, item.name, NULL);
      path_build(sub_out_folder, MAX_PATH, out_filename, item.name, NULL);

      ret = gen_folder(sub_in_folder, sub_out_folder);

      if (ret == RET_FAIL) {
        break;
      }
    }
  }
  fs_dir_close(dir);
  return ret;
}

int wmain(int argc, wchar_t* argv[]) {
  const char* in_filename = NULL;
  const char* out_filename = NULL;

  platform_prepare();

  if (argc < 3) {
    printf("Usage: %S in_filename out_filename \n", argv[0]);
    return 0;
  }

  str_t in_file;
  str_t out_file;

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

  if (in_stat_info.is_dir == TRUE && out_stat_info.is_dir == TRUE) {
    gen_folder(in_filename, out_filename);
  } else if (in_stat_info.is_reg_file == TRUE) {
    gen_one(in_filename, out_filename);
  } else {
    GEN_ERROR(in_filename);
  }

  str_reset(&in_file);
  str_reset(&out_file);

  return 0;
}

#include "common/main.inc"
