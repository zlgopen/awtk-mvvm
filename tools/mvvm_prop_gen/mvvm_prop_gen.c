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
#include "tkc/darray.h"
#include "tkc/fscript.h"
#include "tkc/object_default.h"
#include "tkc/value.h"
#include "common/utils.h"
#include "mvvm/awtk/ui_loader_mvvm.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/binding_rule.h"
#include "mvvm/base/data_binding.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/navigator_request.h"
#include "mvvm/base/binding_rule_parser.h"
#include "mvvm_prop_gen.h"

#define TYPE_NEED_STORE(TYPE) ((TYPE >= MVVM_VIEW_MODEL) && (TYPE <= MVVM_ITEM_FOREACH))

typedef ret_t (*mvvm_prop_gen_exec)(mvvm_prop_gen_t*, const char*, const char*);

struct _mvvm_prop_exec_t;
typedef struct _mvvm_prop_exec_t mvvm_prop_exec_t;

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

struct _mvvm_prop_exec_t {
  uint8_t priority;
  const char* keyword;
  const char* prop;
  mvvm_prop_gen_exec exec;
};

struct _mvvm_prop_record_t {
  str_t vm;
  char* tag;
  uint8_t type;
  mvvm_prop_record_t* next;
  mvvm_prop_record_t* last_vm;
};

struct _mvvm_prop_gen_t {
  darray_t* exec_stack;
  uint32_t result_size;
  uint32_t result_valid;
  mvvm_prop_record_t* records;
  mvvm_prop_result_t** results;
  /* expr */
  darray_t* expr_props;
  tk_object_t* fscript_ctx;
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

static path_item_t* path_item_clone(path_item_t* path_item) {
  path_item_t* tmp_path_item = NULL;
  return_value_if_fail(
      path_item != NULL && path_item->path != NULL && path_item->path_len < PROP_MAX_LEN, NULL);

  tmp_path_item = TKMEM_ZALLOC(path_item_t);
  return_value_if_fail(tmp_path_item != NULL, NULL);

  tmp_path_item->next = NULL;
  tmp_path_item->is_array = path_item->is_array;
  tmp_path_item->path_len = path_item->path_len;
  tmp_path_item->path = tk_strndup(path_item->path, path_item->path_len);

  return tmp_path_item;
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

static int32_t mvvm_prop_exec_cmp(const void* a, const void* b) {
  mvvm_prop_exec_t* exec_a = (mvvm_prop_exec_t*)a;
  mvvm_prop_exec_t* exec_b = (mvvm_prop_exec_t*)b;
  return_value_if_fail(exec_a != NULL && exec_b != NULL, RET_BAD_PARAMS);
  return (exec_a->priority) - (exec_b->priority);
}

static ret_t mvvm_prop_run_exec(void* ctx, const void* data) {
  mvvm_prop_gen_t* mvvm_prop_gen = (mvvm_prop_gen_t*)ctx;
  mvvm_prop_exec_t* mvvm_prop_exec = (mvvm_prop_exec_t*)data;
  return_value_if_fail(mvvm_prop_gen != NULL && mvvm_prop_exec != NULL, RET_BAD_PARAMS);
  mvvm_prop_exec->exec(mvvm_prop_gen, mvvm_prop_exec->keyword, mvvm_prop_exec->prop);
  return RET_OK;
}

static mvvm_prop_exec_t* mvvm_prop_exec_create(mvvm_prop_gen_exec exec, uint8_t priority,
                                               const char* keyword, const char* prop) {
  mvvm_prop_exec_t* mvvm_prop_exec = TKMEM_ZALLOC(mvvm_prop_exec_t);
  return_value_if_fail(mvvm_prop_exec != NULL && exec != NULL && prop != NULL, NULL);

  mvvm_prop_exec->keyword = tk_strdup(keyword);
  mvvm_prop_exec->prop = tk_strdup(prop);
  mvvm_prop_exec->exec = exec;
  mvvm_prop_exec->priority = priority;

  return mvvm_prop_exec;
}

static ret_t mvvm_prop_exec_destory(mvvm_prop_exec_t* mvvm_prop_exec) {
  return_value_if_fail(mvvm_prop_exec != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(mvvm_prop_exec->keyword);
  TKMEM_FREE(mvvm_prop_exec->prop);
  TKMEM_FREE(mvvm_prop_exec);
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
  mvvm_prop_gen->exec_stack =
      darray_create(10, (tk_destroy_t)mvvm_prop_exec_destory, (tk_compare_t)mvvm_prop_exec_cmp);
  mvvm_prop_gen->expr_props = darray_create(10, default_destroy, (tk_compare_t)tk_strcmp);
  mvvm_prop_gen->fscript_ctx = object_default_create();

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

  if (mvvm_prop_gen->exec_stack != NULL) {
    darray_destroy(mvvm_prop_gen->exec_stack);
  }

  if (mvvm_prop_gen->expr_props != NULL) {
    darray_destroy(mvvm_prop_gen->expr_props);
  }

  while (mvvm_prop_gen->records != NULL) {
    log_debug("do not have enough endl tag!\n");

    mvvm_prop_record_t* iter = mvvm_prop_gen->records;
    mvvm_prop_gen->records = mvvm_prop_gen->records->next;
    mvvm_prop_record_destory(iter);
  }
  TK_OBJECT_UNREF(mvvm_prop_gen->fscript_ctx);

  TKMEM_FREE(mvvm_prop_gen);
  return RET_OK;
}

static ret_t copy_last_view_model_name(mvvm_prop_gen_t* mvvm_prop_gen) {
  ret_t ret = RET_OK;
  mvvm_prop_record_t* record = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL, RET_FAIL);

  /* support empty view_model */
  if (record->last_vm != NULL) {
    ret = str_append(&record->vm, record->last_vm->vm.str);
  }

  return ret;
}

static path_item_t* result_find_path_item(mvvm_prop_result_t* mvvm_prop_result, const char* path,
                                          uint32_t path_len, bool_t is_array) {
  path_item_t* item = NULL;
  return_value_if_fail(mvvm_prop_result != NULL && path != NULL, NULL);

  item = mvvm_prop_result->group_head;
  for (; item != NULL; item = item->next) {
    if (item->is_array == is_array && item->path_len == path_len &&
        tk_str_eq_with_len(item->path, path, path_len)) {
      return item;
    }
  }

  return NULL;
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

static ret_t result_push_in_results_array(mvvm_prop_gen_t* mvvm_prop_gen,
                                          mvvm_prop_result_t* result) {
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

static ret_t result_marge_path_item(mvvm_prop_result_t* mvvm_prop_result,
                                    mvvm_prop_result_t* data) {
  path_item_t *item = NULL, *tmp_item = NULL;
  return_value_if_fail(mvvm_prop_result != NULL && data != NULL, RET_BAD_PARAMS);

  item = data->group_head;
  for (; item != NULL; item = item->next) {
    if (result_find_path_item(mvvm_prop_result, item->path, item->path_len, item->is_array)) {
      continue;
    }
    tmp_item = path_item_clone(item);
    return_value_if_fail(tmp_item != NULL, RET_OOM);
    result_add_path_item(mvvm_prop_result, tmp_item);
  }

  return RET_OK;
}

static mvvm_prop_result_t* result_find_in_results_array(mvvm_prop_gen_t* mvvm_prop_gen,
                                                        const char* prop, uint32_t prop_len) {
  uint32_t i = 0;
  mvvm_prop_result_t* result = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && prop != NULL, NULL);

  for (i = 0; i < mvvm_prop_gen->result_size; i++) {
    result = mvvm_prop_gen->results[i];
    if (prop_len == result->prop_len && tk_str_eq_with_len(result->prop, prop, prop_len)) {
      return result;
    }
  }

  return NULL;
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
  }

  mvvm_prop_gen->records = record->next;
  mvvm_prop_record_destory(record);

  return RET_OK;
}

/**
 * �ж�fscript�����Ƿ��������ı��������Ǿֲ�������ϵͳ�����������ı�����Ҫͨ��vm����ȡ��
 * @see fscript.md # 3.4 ��������
 */
static bool_t is_fscript_ctx_variable(fscript_func_call_t* func_call, value_t* v, int32_t arg_index,
                                      fscript_func_t set_local) {
  if (v->type != VALUE_TYPE_ID) {
    return FALSE;
  }
  if (v->value.id.index >= 0) {
    return FALSE;
  }
  if (func_call->func == set_local && arg_index == 0) {
    return FALSE;
  }
  if (tk_str_start_with(v->value.id.id, FSCRIPT_STR_GLOBAL_PREFIX)) {
    return FALSE;
  }
  return TRUE;
}

static ret_t mvvm_prop_gen_parse_one_fscript_func_call(darray_t* variables,
                                                       fscript_func_call_t* func_call,
                                                       fscript_func_t set_local) {
  int32_t i = 0;
  value_t* v = NULL;
  fscript_args_t* args = &(func_call->args);

  if (args == NULL) {
    return RET_OK;
  }

  for (i = 0; i < args->size; i++) {
    v = &(args->args[i]);
    if (v->type == VALUE_TYPE_ID) {
      if (is_fscript_ctx_variable(func_call, v, i, set_local)) {
        darray_push(variables, tk_strdup(v->value.id.id));
      }
    } else if (v->type == VALUE_TYPE_FUNC) {
      mvvm_prop_gen_parse_one_fscript_func_call(variables, value_func(v), set_local);
    }
  }

  return RET_OK;
}

/**
 * ����fscript, �����������ɱ���ʽ�д��ڵı�����
 */
static ret_t parse_args_from_expr(mvvm_prop_gen_t* mvvm_prop_gen, const char* expr) {
  fscript_t* fscript = NULL;
  fscript_func_t set_local = NULL;
  fscript_func_call_t* func_call = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && mvvm_prop_gen->expr_props != NULL && expr != NULL,
                       RET_BAD_PARAMS);

  darray_clear(mvvm_prop_gen->expr_props);

  fscript = fscript_create(mvvm_prop_gen->fscript_ctx, expr);
  return_value_if_fail(fscript != NULL, RET_FAIL);

  func_call = fscript->first;
  set_local = fscript_find_func(fscript, "set_local", strlen("set_local"));

  while (func_call != NULL) {
    mvvm_prop_gen_parse_one_fscript_func_call(mvvm_prop_gen->expr_props, func_call, set_local);
    func_call = func_call->next;
  }

  fscript_destroy(fscript);

  return RET_OK;
}

static ret_t view_model_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* keyword,
                                 const char* prop) {
  MVVM_TYPE type = MVVM_VIEW_MODEL;
  mvvm_prop_record_t* record = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && prop != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL && record->type == MVVM_NOTYPE, RET_FAIL);

  while (prop != NULL && *prop != '\0') {
    if (tk_str_start_with(prop, "sub_view_model:")) {
      return_value_if_fail(type != MVVM_SUB_VIEW_MODEL_ARRAY, RET_FAIL);
      return_value_if_fail(
          copy_last_view_model_name(mvvm_prop_gen) == RET_OK && record->vm.size > 0, RET_FAIL);

      prop += 14;
      str_append_char(&record->vm, '.');
      type = type > MVVM_SUB_VIEW_MODEL ? type : MVVM_SUB_VIEW_MODEL;
    } else if (tk_str_start_with(prop, "sub_view_model_array:")) {
      return_value_if_fail(type != MVVM_VIEW_MODEL_COMB, RET_FAIL);
      return_value_if_fail(
          copy_last_view_model_name(mvvm_prop_gen) == RET_OK && record->vm.size > 0, RET_FAIL);

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

static ret_t bind_data_exec_foreach(mvvm_prop_result_t* result, const char* vm,
                                    const char* bind_data) {
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

static ret_t bind_data_evt_exec_each(mvvm_prop_gen_t* mvvm_prop_gen, const char* prop) {
  bool_t is_array;
  mvvm_prop_record_t* record = NULL;
  mvvm_prop_result_t *result = NULL, *curr_result = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && prop != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL, RET_FAIL);
  record = record->type >= MVVM_VIEW_MODEL ? record : record->last_vm;

  prop = prop_format(prop, strlen(prop));
  return_value_if_fail(prop != NULL, RET_FAIL);

  result = mvvm_prop_result_create(prop, strlen(prop));
  return_value_if_fail(result != NULL, RET_FAIL);

  is_array = check_prop_is_array(prop);
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
    curr_result = result_find_in_results_array(mvvm_prop_gen, result->prop, result->prop_len);
    if (curr_result != NULL) {
      result_marge_path_item(curr_result, result);
      mvvm_prop_result_destory(result);
    } else {
      result_push_in_results_array(mvvm_prop_gen, result);
    }
  }

  return RET_OK;
}

static ret_t binding_data_evt_exec_handle_expr(mvvm_prop_gen_t* mvvm_prop_gen, const char* expr) {
  ret_t ret = RET_OK;
  uint32_t i = 0;
  const char* prop = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && expr != NULL, RET_BAD_PARAMS);

  ret = parse_args_from_expr(mvvm_prop_gen, expr);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  for (i = 0; i < mvvm_prop_gen->expr_props->size; i++) {
    prop = (const char*)darray_get(mvvm_prop_gen->expr_props, i);
    ret = bind_data_evt_exec_each(mvvm_prop_gen, prop);
    return_value_if_fail(ret == RET_OK, RET_FAIL);
  }

  return RET_OK;
}

static ret_t visit_fscript_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  mvvm_prop_gen_t* mvvm_prop_gen = (mvvm_prop_gen_t*)ctx;

  if (!tk_str_eq(NAVIGATOR_ARG_REQ, nv->name)) {
    binding_data_evt_exec_handle_expr(mvvm_prop_gen, value_str(&(nv->value)));
  }

  return RET_OK;
}

static ret_t bind_data_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* keyword,
                                const char* expr) {
  binding_rule_t* rule = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL && expr != NULL, RET_BAD_PARAMS);

  rule = binding_rule_parse(keyword, expr, FALSE);
  return_value_if_fail(rule != NULL, RET_FAIL);

  if (binding_rule_is_data_binding(rule)) {
    data_binding_t* data_binding = DATA_BINDING(rule);
    binding_data_evt_exec_handle_expr(mvvm_prop_gen, data_binding->path);
  } else if (binding_rule_is_command_binding(rule)) {
    command_binding_t* command_binding = COMMAND_BINDING(rule);
    if (tk_str_ieq(COMMAND_BINDING_CMD_FSCRIPT, command_binding->command)) {
      binding_data_evt_exec_handle_expr(mvvm_prop_gen, command_binding->args);
    } else if (command_binding->args != NULL &&
               tk_str_start_with(command_binding->args, COMMAND_ARGS_FSCRIPT_PREFIX)) {
      tk_object_t* objargs = object_default_create();
      if (objargs != NULL) {
        if (tk_command_arguments_to_object(command_binding->args, objargs) == RET_OK) {
          tk_object_foreach_prop(objargs, visit_fscript_prop, mvvm_prop_gen);
        }
        tk_object_unref(objargs);
      }
    }
  }

  tk_object_unref(TK_OBJECT(rule));

  return RET_OK;
}

static ret_t item_foreach_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* keyword,
                                   const char* prop) {
  uint32_t iter_len = 0;
  const char* iter = NULL;
  mvvm_prop_record_t* record = NULL;
  enum VF_STATE { VF_START, VF_VM, VF_INDEX, VF_ITEM = 4 } state = VF_START;
  return_value_if_fail(mvvm_prop_gen != NULL && prop != NULL, RET_BAD_PARAMS);

  record = mvvm_prop_gen->records;
  return_value_if_fail(record != NULL && record->type == MVVM_NOTYPE, RET_FAIL);

  return_value_if_fail(copy_last_view_model_name(mvvm_prop_gen) == RET_OK, RET_FAIL);

  iter = find_substr_until_segm(prop, ',', &iter_len);
  while (iter != NULL || iter_len > 0) {
    if (state == VF_START) {
      if (record->vm.size > 0) str_append_char(&record->vm, '.');
      str_append_with_len(&record->vm, prop, iter_len);
      /* may be is prop, so gen one result path */
      binding_data_evt_exec_handle_expr(mvvm_prop_gen, record->vm.str + record->vm.size - iter_len);
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

static ret_t item_for_items_evt_exec(mvvm_prop_gen_t* mvvm_prop_gen, const char* keyword,
                                     const char* prop) {
  return_value_if_fail(mvvm_prop_gen != NULL && prop != NULL, RET_BAD_PARAMS);

  if (tk_stricmp(prop, "true") == 0) {
    return item_foreach_evt_exec(mvvm_prop_gen, keyword, "");
  }

  return RET_OK;
}

ret_t mvvm_prop_gen_exec_start(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag) {
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL, RET_BAD_PARAMS);
  darray_clear(mvvm_prop_gen->exec_stack);
  return scope_start_evt_exec(mvvm_prop_gen, tag);
}

ret_t mvvm_prop_gen_exec_end(mvvm_prop_gen_t* mvvm_prop_gen, const char* tag) {
  return_value_if_fail(mvvm_prop_gen != NULL && tag != NULL, RET_BAD_PARAMS);
  return scope_end_evt_exec(mvvm_prop_gen, tag);
}

static bool_t mvvm_prop_gen_ensure_prefix(const char* keyword, const char* prefix) {
  return_value_if_fail(keyword != NULL, FALSE);

  if (!tk_str_start_with(keyword, prefix)) {
    return FALSE;
  }
  keyword += strlen(prefix);

  if ((*keyword) != ':') {
    return FALSE;
  }
  keyword += 1;

  if (strlen(keyword) > 0) {
    return TRUE;
  }
  return FALSE;
}

ret_t mvvm_prop_gen_try_gen_exec_by_keyword(mvvm_prop_gen_t* mvvm_prop_gen, const char* keyword,
                                            const char* prop) {
  mvvm_prop_exec_t* mvvm_prop_exec = NULL;
  return_value_if_fail(mvvm_prop_gen != NULL, RET_BAD_PARAMS);

  if (keyword == NULL && prop == NULL) {
    darray_sort(mvvm_prop_gen->exec_stack, NULL);
    darray_foreach(mvvm_prop_gen->exec_stack, mvvm_prop_run_exec, (void*)mvvm_prop_gen);
    darray_clear(mvvm_prop_gen->exec_stack);
  } else if (tk_str_eq(keyword, WIDGET_PROP_V_MODEL)) {
    mvvm_prop_exec = mvvm_prop_exec_create(view_model_evt_exec, 0, keyword, prop);
    return_value_if_fail(mvvm_prop_exec != NULL, RET_FAIL);
  } else if (mvvm_prop_gen_ensure_prefix(keyword, BINDING_RULE_DATA_PREFIX) ||
             mvvm_prop_gen_ensure_prefix(keyword, BINDING_RULE_COMMAND_PREFIX) ||
             mvvm_prop_gen_ensure_prefix(keyword, BINDING_RULE_CONDITION_IF) ||
             mvvm_prop_gen_ensure_prefix(keyword, BINDING_RULE_CONDITION_ELIF)) {
    mvvm_prop_exec = mvvm_prop_exec_create(bind_data_evt_exec, 1, keyword, prop);
    return_value_if_fail(mvvm_prop_exec != NULL, RET_FAIL);
  } else if (tk_str_eq(keyword, BINDING_RULE_ITEMS)) {
    mvvm_prop_exec = mvvm_prop_exec_create(item_foreach_evt_exec, 0, keyword, prop);
    return_value_if_fail(mvvm_prop_exec != NULL, RET_FAIL);
  } else if (tk_str_eq(keyword, WIDGET_PROP_V_FOR_ITEMS)) {
    mvvm_prop_exec = mvvm_prop_exec_create(item_for_items_evt_exec, 0, keyword, prop);
    return_value_if_fail(mvvm_prop_exec != NULL, RET_FAIL);
  }

  if (mvvm_prop_exec) {
    darray_push(mvvm_prop_gen->exec_stack, mvvm_prop_exec);
  }

  return RET_OK;
}

mvvm_prop_result_t** mvvm_prop_gen_result_create(mvvm_prop_gen_t* mvvm_prop_gen,
                                                 uint32_t* result_size) {
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
  return_value_if_fail(results != NULL && json != NULL, RET_BAD_PARAMS);

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
    if (path_item != NULL) {
      do {
        if (j++ > 0) {
          str_append(json, ",");
        }
        str_append(json, "{");
        str_append_json_str_pair(json, JSON_PROTO_PATH, path_item->path);
        str_append(json, ",");
        str_append_json_bool_pair(json, JSON_PROTO_IS_ARRAY, path_item->is_array);
        str_append(json, "}");
        path_item = path_item->next;
      } while (path_item != NULL);
    } else {
      str_append(json, "{");
      str_append_json_str_pair(json, JSON_PROTO_PATH, "");
      str_append(json, ",");
      str_append_json_bool_pair(json, JSON_PROTO_IS_ARRAY, FALSE);
      str_append(json, "}");
    }
    str_append(json, "]");
  }
  str_append(json, "}");

  return RET_OK;
}
