/**
 * File:   view_model_file.c
 * Author: AWTK Develop Team
 * Brief:  view_model_file
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-12-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "tkc/func_call_parser.h"
#include "mvvm/base/navigator.h"
#include "mvvm/view_models/view_model_file.h"

static ret_t view_model_file_on_destroy(tk_object_t* obj) {
  view_model_file_t* file = VIEW_MODEL_FILE(obj);

  TKMEM_FREE(file->content);
  str_reset(&(file->filename));

  view_model_deinit(VIEW_MODEL(obj));

  return RET_OK;
}

static ret_t view_model_file_set_filename(view_model_file_t* file, const char* filename) {
  char path[MAX_PATH + 1] = {0};
  return_value_if_fail(filename != NULL && file != NULL, RET_BAD_PARAMS);

  path_expand_vars(filename, path, sizeof(path) - 1);

  str_set(&(file->filename), path);

  return RET_OK;
}

static ret_t view_model_file_load(view_model_file_t* file) {
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(file->content);
  file->content = file_read(file->filename.str, &(file->size));
  file->is_dirty = FALSE;

  return RET_OK;
}

static ret_t view_model_file_save(view_model_file_t* file) {
  ret_t ret = RET_OK;
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  ret = file_write(file->filename.str, file->content, file->size);
  file->is_dirty = FALSE;

  return ret;
}

static int32_t view_model_file_compare(tk_object_t* obj, tk_object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_file_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  view_model_file_t* file = VIEW_MODEL_FILE(obj);
  return_value_if_fail(file != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_FILENAME)) {
    view_model_file_set_filename(file, value_str(v));
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_CONTENT)) {
    file->content = tk_str_copy(file->content, value_str(v));
    file->size = tk_strlen(file->content);
    file->is_dirty = TRUE;
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_AUTO_LOAD)) {
    file->auto_load = value_bool(v);
  } else if(tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_IS_DIRTY)) {
    file->is_dirty = value_bool(v);
  } else {
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static ret_t view_model_file_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  view_model_file_t* file = VIEW_MODEL_FILE(obj);
  return_value_if_fail(file != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_FILENAME)) {
    value_set_str(v, file->filename.str);
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_CONTENT)) {
    value_set_str(v, file->content);
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_SIZE)) {
    value_set_uint32(v, file->size);
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_AUTO_LOAD)) {
    value_set_bool(v, file->auto_load);
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_PROP_IS_DIRTY)) {
    value_set_bool(v, file->is_dirty);
  } else {
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static bool_t view_model_file_can_exec(tk_object_t* obj, const char* name, const char* args) {
  view_model_file_t* file = VIEW_MODEL_FILE(obj);
  return_value_if_fail(file != NULL && name != NULL, FALSE);

  if (tk_str_eq(name, TK_OBJECT_CMD_SAVE)) {
    return file->is_dirty;
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_CMD_BROWSE)) {
    return tk_str_start_with(args, "open") || tk_str_start_with(args, "save");
  } else if (tk_str_eq(name, TK_OBJECT_CMD_RELOAD)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t view_model_file_browse(view_model_file_t* file, const char* args) {
  ret_t ret = RET_OK;
  const char* filter = NULL;
  const char* title = NULL;
  bool_t for_save = FALSE;
  tk_object_t* obj = func_call_parse(args, tk_strlen(args));
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  filter = tk_object_get_prop_str(obj, "filter");
  title = tk_object_get_prop_str(obj, "title");

  if (tk_str_eq(obj->name, "open")) {
    for_save = FALSE;
  } else if (tk_str_eq(obj->name, "save")) {
    for_save = TRUE;
  } else if (tk_str_eq(obj->name, "saveas")) {
    for_save = TRUE;
  }

  ret = navigator_pick_file(title, filter, for_save, &(file->filename));
  if (ret == RET_OK) {
    if (tk_str_eq(obj->name, "open")) {
      if (file->auto_load) {
        view_model_file_load(file);
      }
    } else if (tk_str_eq(obj->name, "saveas")) {
      view_model_file_save(file);
    }

    ret = RET_OBJECT_CHANGED;
  }

  TK_OBJECT_UNREF(obj);

  return ret;
}

static ret_t view_model_file_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_OK;
  view_model_file_t* file = VIEW_MODEL_FILE(obj);
  return_value_if_fail(file != NULL && name != NULL, FALSE);

  if (tk_str_eq(name, TK_OBJECT_CMD_SAVE)) {
    file->is_dirty = FALSE;
    ret = file_write(file->filename.str, file->content, file->size);
    return ret == RET_OK ? RET_OBJECT_CHANGED : RET_FAIL;
  } else if (tk_str_eq(name, STR_VIEW_MODEL_FILE_CMD_BROWSE)) {
    return view_model_file_browse(file, args);
  } else if (tk_str_eq(name, TK_OBJECT_CMD_RELOAD)) {
    view_model_file_load(file);
    return file->content != NULL ? RET_OBJECT_CHANGED : RET_FAIL;
  }

  return RET_OK;
}

static const object_vtable_t s_model_file_vtable = {.type = "view_model_file",
                                                    .desc = "view_model_file",
                                                    .size = sizeof(view_model_file_t),
                                                    .is_collection = FALSE,
                                                    .on_destroy = view_model_file_on_destroy,
                                                    .compare = view_model_file_compare,
                                                    .get_prop = view_model_file_get_prop,
                                                    .set_prop = view_model_file_set_prop,
                                                    .can_exec = view_model_file_can_exec,
                                                    .exec = view_model_file_exec};

#include "tkc/path.h"
#include "tkc/fs.h"

static ret_t view_model_file_init(view_model_file_t* file, tk_object_t* args) {
  const char* path = NULL;
  bool_t auto_load = FALSE;
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  path = tk_object_get_prop_str(args, "path");

  if (path != NULL) {
    view_model_file_set_filename(file, path);
  }

  auto_load = tk_object_get_prop_bool(args, "auto_load", FALSE);
  if (auto_load) {
    view_model_file_load(file);
  } else {
    file->size = file_get_size(file->filename.str);
  }
  file->auto_load = auto_load;
  
  return RET_OK;
}

view_model_t* view_model_file_create(navigator_request_t* req) {
  const char* type_and_args = NULL;
  tk_object_t* obj = tk_object_create(&s_model_file_vtable);
  view_model_file_t* file = VIEW_MODEL_FILE(obj);
  return_value_if_fail(file != NULL, NULL);
  type_and_args = tk_object_get_prop_str(req->args, NAVIGATOR_ARG_VIEW_MODEL_TYPE);

  file->size = 0;
  file->content = NULL;
  file->is_dirty = FALSE;
  str_init(&(file->filename), MAX_PATH);

  if (type_and_args != NULL && strchr(type_and_args, '(') != NULL) {
    /*add*/
    tk_object_t* args = func_call_parse(type_and_args, tk_strlen(type_and_args));
    if (args != NULL) {
      view_model_file_init(file, args);
      TK_OBJECT_UNREF(args);
    }
  }

  return view_model_init(VIEW_MODEL(obj));
}
