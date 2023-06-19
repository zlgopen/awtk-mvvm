/**
 * File:   mvvm_prop_gen.c
 * Author: AWTK Develop Team
 * Brief:  proto mvvm prop string
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

#include "tkc/mem.h"
#include "common/utils.h"
#include "mvvm_prop_gen.h"
#include "mvvm/awtk/ui_loader_mvvm.h"

#define SCOPE_START "scope_start"
#define SCOPE_END "scope_end"

#define TYPE_NEED_STORE(TYPE) ((TYPE >= MVVM_VIEW_MODEL) && (TYPE <= MVVM_ITEM_FOREACH))

struct _mvvm_prop_record_t;
typedef struct _mvvm_prop_record_t mvvm_prop_record_t;

typedef enum _MVVM_TYPE {
  MVVM_NOTYPE = 0x00,
  MVVM_VIEW_MODEL,
  MVVM_SUB_VIEW_MODEL,
  MVVM_SUB_VIEW_MODEL_ARRAY,
  MVVM_VIEW_MODEL_COMB,
  MVVM_ITEM_FOREACH,
  MVVM_RESERVERS = 0xFF
} MVVM_TYPE;

struct _mvvm_prop_record_t {
  str_t vm;
  char* tag;
  uint8_t type;
  mvvm_prop_record_t* next;
  mvvm_prop_record_t* last_vm;
};

struct _mvvm_prop_gen_t {
  uint32_t result_size;
  uint32_t result_valid;
  mvvm_prop_record_t* records;
  mvvm_prop_result_t** results;
};

static path_item_t* path_item_create(const char* path, uint32_t path_len, bool_t is_array) {
  path_item_t* path_item = TKMEM_ZALLOC(path_item_t);
  return_value_if_fail(path_item != NULL && path != NULL && path_len < PROP_MAX_LEN, NULL);

  path_item->next = NULL;
  path_item->is_array = is_array;

  path_item->path_len = path_len;
  path_item->path = tk_strndup(path, path_len);

  return path_item;
}

static ret_t path_item_destory(path_item_t* path_item) {
  return_value_if_fail(path_item != NULL, RET_BAD_PARAMS);
  if (path_item->path != NULL) {
    TKMEM_FREE(path_item->path);
  }
  TKMEM_FREE(path_item);
  return RET_OK;
}

static mvvm_prop_result_t* mvvm_prop_result_create(const char* prop, uint32_t prop_len) {
  mvvm_prop_result_t* mvvm_prop_result = TKMEM_ZALLOC(mvvm_prop_result_t);
  return_value_if_fail(mvvm_prop_result != NULL && prop != NULL && prop_len < PROP_MAX_LEN, NULL);

  mvvm_prop_result->group_size = 0;
  mvvm_prop_result->group_head = NULL;
  mvvm_prop_result->group_tail = NULL;

  mvvm_prop_result->prop_len = prop_len;
  mvvm_prop_result->prop = tk_strndup(prop, prop_len);

  return mvvm_prop_result;
}

static ret_t mvvm_prop_result_destory(mvvm_prop_result_t* mvvm_prop_result) {
  return_value_if_fail(mvvm_prop_result != NULL, RET_BAD_PARAMS);

  if (mvvm_prop_result->prop != NULL) {
    TKMEM_FREE(mvvm_prop_result->prop);
  }

  if (mvvm_prop_result->group_size > 0) {
    while (mvvm_prop_result->group_head != NULL) {
      path_item_t* iter = mvvm_prop_result->group_head;
      mvvm_prop_result->group_head = mvvm_prop_result->group_head->next;
      path_item_destory(iter);
      iter = NULL;
    }
  }

  TKMEM_FREE(mvvm_prop_result);
  return RET_OK;
}

static mvvm_prop_record_t* mvvm_prop_record_create(const char* tag) {
  mvvm_prop_record_t* record = TKMEM_ZALLOC(mvvm_prop_record_t);
  return_value_if_fail(record != NULL && tag != NULL, NULL);

  str_init(&record->vm, 0);
  record->type = MVVM_NOTYPE;
  record->tag = tk_strndup(tag, strlen(tag));

  record->next = NULL;
  record->last_vm = NULL;

  return record;
}

static ret_t mvvm_prop_record_destory(mvvm_prop_record_t* record) {
  return_value_if_fail(record != NULL, RET_BAD_PARAMS);

  str_reset(&record->vm);

  if (record->tag != NULL) {
    TKMEM_FREE(record->tag);
  }

  record->next = NULL;
  record->last_vm = NULL;
  TKMEM_FREE(record);

  return RET_OK;
}

mvvm_prop_gen_t* mvvm_prop_gen_create(void) {
  mvvm_prop_gen_t* mvvm_prop_gen = TKMEM_ZALLOC(mvvm_prop_gen_t);
  goto_error_if_fail(mvvm_prop_gen != NULL);

  mvvm_prop_gen->result_size = 0;
  mvvm_prop_gen->result_valid = MVVM_PROP_RESULT_INIT_SIZE;
  mvvm_prop_gen->results = TKMEM_ZALLOCN(mvvm_prop_result_t*, mvvm_prop_gen->result_valid + 1);
  goto_error_if_fail(mvvm_prop_gen->results != NULL);

  mvvm_prop_gen->records = NULL;

  return mvvm_prop_gen;
error:
  if (mvvm_prop_gen != NULL) {
    mvvm_prop_gen_destory(mvvm_prop_gen);
    mvvm_prop_gen = NULL;
  }
  return NULL;
}

ret_t mvvm_prop_gen_destory(mvvm_prop_gen_t* mvvm_prop_gen) {
  return_value_if_fail(mvvm_prop_gen != NULL, RET_BAD_PARAMS);

  if (mvvm_prop_gen->results != NULL) {
    mvvm_prop_gen_result_destory(mvvm_prop_gen->results, mvvm_prop_gen->result_size);
  }

  while (mvvm_prop_gen->records != NULL) {
    log_debug("do not have enough endl tag!\n");

    mvvm_prop_record_t* iter = mvvm_prop_gen->records;
    mvvm_prop_gen->records = mvvm_prop_gen->records->next;
    mvvm_prop_record_destory(iter);
  }

  TKMEM_FREE(mvvm_prop_gen);
  return RET_OK;
}

static ret_t copy_last_view_model_name(mvvm_prop_gen_t* mvvm_prop_gen) {
  mvvm_prop_record_t* record = NULL;
  mvvm_prop_record_t* last_record = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL && record->last_vm != NULL, RET_FAIL);

  last_record = record->last_vm;
  return str_append(&record->vm, record->last_vm->vm.str);
}

static ret_t result_add_path_item(mvvm_prop_result_t* mvvm_prop_result, path_item_t* path_item) {
  return_value_if_fail(mvvm_prop_result != NULL && path_item != NULL, RET_BAD_PARAMS);
  if (mvvm_prop_result->group_tail == NULL) {
    mvvm_prop_result->group_head = path_item;
    mvvm_prop_result->group_tail = path_item;
  } else {
    mvvm_prop_result->group_tail->next = path_item;
    mvvm_prop_result->group_tail = path_item;
  }
  mvvm_prop_result->group_size += 1;
  return RET_OK;
}

static ret_t result_push_in_results_array(mvvm_prop_gen_t* mvvm_prop_gen, mvvm_prop_result_t* result) {
  return_value_if_fail(mvvm_prop_gen != NULL && result != NULL, RET_BAD_PARAMS);

  if (mvvm_prop_gen->result_size >= mvvm_prop_gen->result_valid) {
    void* tmp = NULL;
    uint32_t new_size = (mvvm_prop_gen->result_valid << 1);
    tmp = TKMEM_REALLOCT(mvvm_prop_result_t, mvvm_prop_gen->results, new_size + 1);
    return_value_if_fail(tmp != NULL, RET_OOM);
    mvvm_prop_gen->results = tmp;
    mvvm_prop_gen->result_valid = new_size;
  }

  mvvm_prop_gen->results[mvvm_prop_gen->result_size++] = result;
  mvvm_prop_gen->results[mvvm_prop_gen->result_size] = NULL;

  return RET_OK;
}

static ret_t scope_start_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag) {
  mvvm_prop_record_t* record = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_record_create(tag);
  return_value_if_fail(record != NULL, RET_OOM);

  if (mvvm_prop_gen->records != NULL && TYPE_NEED_STORE(mvvm_prop_gen->records->type)) {
    record->last_vm = mvvm_prop_gen->records;
  } else if (mvvm_prop_gen->records != NULL && mvvm_prop_gen->records->last_vm != NULL) {
    record->last_vm = mvvm_prop_gen->records->last_vm;
  } else {
    record->last_vm = NULL;
  }

  record->next = mvvm_prop_gen->records;
  mvvm_prop_gen->records = record;

  return RET_OK;
}

static ret_t scope_end_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag) {
  mvvm_prop_record_t* record = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  if (record == NULL || !tk_str_eq(record->tag, tag)) {
    log_debug("start_tag[%s] and end_tag[%s] is not pair!\n", mvvm_prop_gen->records->tag, tag);
    return RET_FAIL;
  }

  mvvm_prop_gen->records = record->next;
  mvvm_prop_record_destory(record);

  return RET_OK;
}

static ret_t view_model_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag, const char* prop) {
  MVVM_TYPE type = MVVM_VIEW_MODEL;
  mvvm_prop_record_t* record = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL && prop != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL && tk_str_eq(record->tag, tag) && record->type == MVVM_NOTYPE, RET_FAIL);

  while (prop != NULL && *prop != '\0') {
    if (tk_str_start_with(prop, "sub_view_model:")) {
      return_value_if_fail(type != MVVM_SUB_VIEW_MODEL_ARRAY, RET_FAIL);
      return_value_if_fail(copy_last_view_model_name(mvvm_prop_gen) == RET_OK, RET_FAIL);

      prop += 14;
      str_append_char(&record->vm, '.');
      type = type > MVVM_SUB_VIEW_MODEL ? type : MVVM_SUB_VIEW_MODEL;
    } else if (tk_str_start_with(prop, "sub_view_model_array:")) {
      return_value_if_fail(type != MVVM_VIEW_MODEL_COMB, RET_FAIL);
      return_value_if_fail(copy_last_view_model_name(mvvm_prop_gen) == RET_OK, RET_FAIL);

      prop += 20;
      str_append_char(&record->vm, '.');
      type = MVVM_SUB_VIEW_MODEL_ARRAY;
    } else if (*prop == '+') {
      return_value_if_fail(type != MVVM_SUB_VIEW_MODEL_ARRAY, RET_FAIL);

      type = MVVM_VIEW_MODEL_COMB;
      str_append_char(&record->vm, '|');
    } else if (!tk_isspace(*prop)) {
      str_append_char(&record->vm, *prop);
    }
    prop += 1;
  }

  record->type = type;
  return RET_OK;
}

static const char* find_substr_until_segm(const char* prop, const char segm, uint32_t* len) {
  uint32_t max_len = 0;
  return_value_if_fail(len != NULL, NULL);

  (*len) = 0;
  if (prop == NULL || *prop == '\0') {
    return NULL;
  }

  max_len = strlen(prop);
  while (prop != NULL && (*len) < max_len && *prop != segm) {
    prop += 1;
    (*len) += 1;
  }

  if ((*len) >= max_len) {
    return NULL;
  }

  return prop + 1;
}

static bool_t check_prop_is_array(const char* prop) {
  return_value_if_fail(prop != NULL, FALSE);
  if (strchr(prop, '[') != NULL && strchr(prop, ']') != NULL) {
    return TRUE;
  }
  return FALSE;
}

static const char* prop_format(const char* prop, uint32_t prop_len) {
  char* ptr = (char*)prop;
  const char* ptr_pos = NULL;

  ptr_pos = strchr(ptr, ',');
  if (ptr_pos != NULL) {
    ptr[ptr_pos - ptr] = '\0';
  }

  return (const char*)ptr;
}

static ret_t bind_data_exec_foreach(mvvm_prop_result_t* result, const char* vm, const char* bind_data) {
  uint32_t prop_len = 0;
  uint32_t iter_len = 0;
  const char* prop = NULL;
  const char* iter = NULL;
  path_item_t* path_item = NULL;
  enum VF_STATE { VF_VM, VF_INDEX, VF_ITEM } state = VF_VM;
  return_value_if_fail(result != NULL && vm != NULL && bind_data != NULL, RET_FAIL);

  prop = tk_strrstr(bind_data, ".");
  if (prop == NULL) {
    state = VF_INDEX;
    prop = bind_data;
    prop_len = strlen(bind_data);
  } else {
    prop += 1;
    prop_len = strlen(prop);
  }

  iter = find_substr_until_segm(vm, '|', &iter_len);
  while (iter != NULL || iter_len > 0) {
    if (state == VF_VM) {
      state = VF_ITEM;
      path_item = path_item_create(vm, iter_len, TRUE);
    } else if (state == VF_ITEM) {
      if (tk_str_eq_with_len(vm, bind_data, (prop - bind_data - 1))) {
        result->prop_len = prop_len;
        tk_strncpy(result->prop, prop, prop_len);
        result_add_path_item(result, path_item);
        return RET_STOP;
      }
    } else if (state == VF_INDEX) {
      if (prop_len == iter_len && tk_str_eq_with_len(prop, vm, prop_len)) {
        return RET_QUIT;
      }
    }
    vm = iter;
    iter = find_substr_until_segm(iter, '|', &iter_len);
  }

  if (path_item != NULL) {
    path_item_destory(path_item);
  }
  return RET_OK;
}

static ret_t bind_data_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag, const char* prop) {
  bool_t is_array;
  mvvm_prop_record_t* record = NULL;
  mvvm_prop_result_t* result = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL && prop != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL && tk_str_eq(record->tag, tag), RET_FAIL);
  record = record->type >= MVVM_VIEW_MODEL ? record : record->last_vm;

  is_array = check_prop_is_array(prop);
  prop = prop_format(prop, strlen(prop));
  return_value_if_fail(prop != NULL, RET_FAIL);

  result = mvvm_prop_result_create(prop, strlen(prop));
  return_value_if_fail(result != NULL, RET_FAIL);

  while (record != NULL) {
    path_item_t* item = NULL;

    if (record->type == MVVM_ITEM_FOREACH) {
      ret_t ret;
      ret = bind_data_exec_foreach(result, record->vm.str, prop);
      if (ret == RET_STOP) {
        break;
      } else if (ret == RET_QUIT) {
        mvvm_prop_result_destory(result);
        result = NULL;
        break;
      }
    } else if (record->type == MVVM_VIEW_MODEL_COMB) {
      uint32_t iter_len = 0;
      const char* iter = NULL;
      const char* va_iter = record->vm.str;

      iter = find_substr_until_segm(va_iter, '|', &iter_len);
      while (iter != NULL || iter_len > 0) {
        item = path_item_create(va_iter, iter_len, FALSE);
        result_add_path_item(result, item);

        va_iter = iter;
        iter = find_substr_until_segm(iter, '|', &iter_len);
      }
    } else if (record->type >= MVVM_VIEW_MODEL) {
      item = path_item_create(record->vm.str, record->vm.size, FALSE);
      result_add_path_item(result, item);
    }

    if (is_array) break;
    record = record->last_vm;
  }

  if (result != NULL) {
    result_push_in_results_array(mvvm_prop_gen, result);
  }

  return RET_OK;
}

static ret_t item_foreach_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag, const char* prop) {
  uint32_t iter_len = 0;
  const char* iter = NULL;
  mvvm_prop_record_t* record = NULL;
  enum VF_STATE { VF_START, VF_VM, VF_INDEX, VF_ITEM = 4 } state = VF_START;
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL && prop != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL && tk_str_eq(record->tag, tag) && record->type == MVVM_NOTYPE, RET_FAIL);

  return_value_if_fail(copy_last_view_model_name(mvvm_prop_gen) == RET_OK, RET_FAIL);

  iter = find_substr_until_segm(prop, ',', &iter_len);
  while (iter != NULL || iter_len > 0) {
    if (state == VF_START) {
      str_append_char(&record->vm, '.');
      str_append_with_len(&record->vm, prop, iter_len);
      /* may be is prop, so gen one result path */
      bind_data_evt_exec(mvvm_prop_gen, tag, tk_strrstr(record->vm.str, ".") + 1);
      state = VF_VM;
    } else if ((state & VF_VM) == VF_VM) {
      while (tk_isspace(*prop) && (prop += 1) && (iter_len -= 1)) continue;

      if (tk_str_start_with(prop, "Index=")) {
        prop += 6;
        iter_len -= 6;
        state |= VF_INDEX;
      } else if (tk_str_start_with(prop, "Item=")) {
        prop += 5;
        iter_len -= 5;
        state |= VF_ITEM;
      } else {
        log_debug("v-for do not find Index=value or Item=value!\n");
        return RET_FAIL;
      }

      str_append_char(&record->vm, '|');
      str_append_with_len(&record->vm, prop, iter_len);
    }

    prop = iter;
    iter = find_substr_until_segm(iter, ',', &iter_len);
  }

  if ((state & VF_INDEX) == 0) {
    str_append(&record->vm, "|index");
  }

  if ((state & VF_ITEM) == 0) {
    str_append(&record->vm, "|item");
  }

  record->type = MVVM_ITEM_FOREACH;
  return RET_OK;
}

static ret_t item_for_items_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag, const char* prop) {
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL && prop != NULL, RET_BAD_PARAMS);

  if (tk_stricmp(prop, "true") == 0) {
    return item_foreach_evt_exec(mvvm_prop_gen, tag, "");
  }

  return RET_OK;
}

mvvm_prop_gen_exec mvvm_prop_gen_translate_keyword(const char* keyword) {
  return_value_if_fail(keyword != NULL, NULL);

  if (tk_str_eq(keyword, SCOPE_START)) {
    return scope_start_evt_exec;
  } else if (tk_str_eq(keyword, SCOPE_END)) {
    return scope_end_evt_exec;
  } else if (tk_str_eq(keyword, WIDGET_PROP_V_MODEL)) {
    return view_model_evt_exec;
  } else if (tk_str_start_with(keyword, BINDING_RULE_DATA_PREFIX)) {
    return bind_data_evt_exec;
  } else if (tk_str_eq(keyword, BINDING_RULE_ITEMS)) {
    return item_foreach_evt_exec;
  } else if (tk_str_eq(keyword, WIDGET_PROP_V_FOR_ITEMS)) {
    return item_for_items_evt_exec;
  }

  return NULL;
}

mvvm_prop_result_t** mvvm_prop_gen_result_create(mvvm_prop_gen_t* mvvm_prop_gen, uint32_t* result_size) {
  mvvm_prop_result_t** results = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL, NULL);

  results = mvvm_prop_gen->results;
  mvvm_prop_gen->results = NULL;

  (*result_size) = mvvm_prop_gen->result_size;
  return results;
}

ret_t mvvm_prop_gen_result_destory(mvvm_prop_result_t** results, uint32_t result_size) {
  uint32_t i = 0;
  if (results != NULL) {
    for (i = 0; i < result_size; ++i) {
      mvvm_prop_result_destory(results[i]);
    }
    TKMEM_FREE(results);
  }
  return RET_OK;
}

mvvm_prop_result_t** mvvm_prop_gen_proto_result(str_t* json, uint32_t* result_size) {
  uint32_t i = 0;
  mvvm_prop_result_t* res = NULL;
  mvvm_prop_result_t** results = NULL;
  mvvm_prop_gen_t* mvvm_prop_gen = mvvm_prop_gen_create();
  goto_error_if_fail(mvvm_prop_gen != NULL && json != NULL && result_size != NULL);

  str_replace(json, " ", "");
  str_replace(json, "\r", "");
  str_replace(json, "\n", "");
  str_replace(json, "\t", "");

  for (i = 1; i < json->size - 1; ++i) {
    uint32_t prop_size = 0;

    if (json->str[i] == ',') i += 1;
    goto_error_if_fail(json->str[i++] == '\"');
    while (json->str[i] != '\"') {
      prop_size += 1;
      goto_error_if_fail((++i) < json->size);
    }
    res = mvvm_prop_result_create(json->str + i - prop_size, prop_size);
    goto_error_if_fail(res != NULL);

    i += 1;
    goto_error_if_fail(json->str[i++] == ':');
    goto_error_if_fail(json->str[i++] == '[');
    while (json->str[i] != ']') {
      uint8_t is_array = 0;
      uint32_t path_size = 0;
      path_item_t* item = NULL;
      const char* path_iter = NULL;

      goto_error_if_fail(json->str[i++] == '{');
      goto_error_if_fail(json->str[i++] == '\"');
      find_substr_until_segm(json->str + i, '\"', &prop_size);
      goto_error_if_fail(strncmp(json->str + i + 1, JSON_PROTO_PATH, prop_size));
      i += prop_size + 1;

      goto_error_if_fail(json->str[i++] == ':');
      goto_error_if_fail(json->str[i++] == '\"');
      find_substr_until_segm(json->str + i, '\"', &path_size);
      goto_error_if_fail(path_size > 0);
      path_iter = json->str + i;
      i += path_size + 1;

      goto_error_if_fail(json->str[i++] == ',');
      goto_error_if_fail(json->str[i++] == '\"');
      find_substr_until_segm(json->str + i, '\"', &prop_size);
      goto_error_if_fail(strncmp(json->str + i + 1, JSON_PROTO_IS_ARRAY, prop_size));
      i += prop_size + 1;

      goto_error_if_fail(json->str[i++] == ':');
      find_substr_until_segm(json->str + i, '}', &prop_size);
      is_array |= (strncmp(json->str + i, "false", prop_size) != 0);
      is_array |= (strncmp(json->str + i, "FALSE", prop_size) != 0);
      i += prop_size;

      item = path_item_create(path_iter, path_size, is_array);
      goto_error_if_fail(item != NULL);
      result_add_path_item(res, item);
      goto_error_if_fail(json->str[i++] == '}');
      if (json->str[i] == ',') i += 1;
    }
    result_push_in_results_array(mvvm_prop_gen, res);
  }

  results = mvvm_prop_gen_result_create(mvvm_prop_gen, result_size);
  mvvm_prop_gen_destory(mvvm_prop_gen);
  return results;
error:
  if (res != NULL) {
    mvvm_prop_result_destory(res);
  }

  if (mvvm_prop_gen != NULL) {
    mvvm_prop_gen_destory(mvvm_prop_gen);
  }
  return NULL;
}

ret_t mvvm_prop_gen_save_result(mvvm_prop_result_t** results, uint32_t result_size, str_t* json) {
  uint32_t i = 0;
  return_value_if_fail(results != NULL && result_size > 0 && json != NULL, RET_BAD_PARAMS);

  str_append(json, "{");
  for (i = 0; i < result_size; ++i) {
    uint32_t j = 0;
    path_item_t* path_item = NULL;
    mvvm_prop_result_t* iter = results[i];

    if (i != 0) {
      str_append(json, ",");
    }
    str_append_json_str(json, iter->prop);
    str_append(json, ": [");

    path_item = iter->group_head;
    while (path_item != NULL) {
      if (j++ > 0) {
        str_append(json, ",");
      }
      str_append(json, "{");
      str_append_json_str_pair(json, JSON_PROTO_PATH, path_item->path);
      str_append(json, ",");
      str_append_json_bool_pair(json, JSON_PROTO_IS_ARRAY, path_item->is_array);
      str_append(json, "}");
      path_item = path_item->next;
    }
    str_append(json, "]");
  }
  str_append(json, "}");

  return RET_OK;
}