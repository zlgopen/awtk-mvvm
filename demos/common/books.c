/**
 * File:   books.c
 * Author: AWTK Develop Team
 * Brief:  books
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
 * 2019-03-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "books.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"

book_info_t* book_info_init(book_info_t* info, const char* name, float_t stock) {
  return_value_if_fail(info != NULL && name != NULL, NULL);

  info->stock = stock;
  tk_strncpy(info->name, name, TK_NAME_LEN);

  return info;
}

book_info_t* book_info_create(const char* name, float_t stock) {
  book_info_t* info = NULL;
  return_value_if_fail(name != NULL, NULL);

  info = TKMEM_ZALLOC(book_info_t);
  return_value_if_fail(info != NULL, NULL);

  return book_info_init(info, name, stock);
}

int book_info_cmp(book_info_t* a, book_info_t* b) {
  return_value_if_fail(a != NULL && b != NULL, -1);

  return strcmp(a->name, b->name);
}

ret_t book_info_destroy(book_info_t* info) {
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(info);

  return RET_OK;
}

static ret_t books_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  ret_t ret = RET_OK;
  books_t* books = BOOKS(obj);
  book_info_t* info = NULL;

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < books_count(MODEL(obj)), RET_BAD_PARAMS);
  info = (book_info_t*)(books->books.elms[index]);

  if (tk_str_eq(BOOK_PROP_NAME, name)) {
    tk_strncpy(info->name, value_str(v), TK_NAME_LEN);
  } else if (tk_str_eq(BOOK_PROP_STOCK, name)) {
    info->stock = value_float(v);
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

static ret_t books_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  ret_t ret = RET_OK;
  book_info_t* info = NULL;
  books_t* books = BOOKS(obj);

  if (tk_str_eq(MODEL_PROP_ITEMS, name)) {
    value_set_int(v, books_count(MODEL(obj)));

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < books_count(MODEL(obj)), RET_BAD_PARAMS);
  info = (book_info_t*)(books->books.elms[index]);

  if (tk_str_eq(BOOK_PROP_NAME, name)) {
    value_set_str(v, info->name);
  } else if (tk_str_eq(BOOK_PROP_STOCK, name)) {
    value_set_float(v, info->stock);
  } else if (tk_str_eq(BOOK_PROP_STYLE, name)) {
    value_set_str(v, index % 2 ? "odd" : "even");
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

static bool_t books_can_exec(object_t* obj, const char* name, const char* args) {
  bool_t ret = TRUE;
  uint32_t index = tk_atoi(args);
  if (tk_str_ieq(name, BOOK_CMD_REMOVE)) {
    ret = index < books_count(MODEL(obj));
  } else if (tk_str_ieq(name, BOOK_CMD_SALE)) {
    ret = index < books_count(MODEL(obj));
  } else if (tk_str_ieq(name, BOOK_CMD_ADD)) {
    ret = TRUE;
  } else if (tk_str_ieq(name, BOOK_CMD_CLEAR)) {
    ret = books_count(MODEL(obj)) > 0;
  } else {
    ret = FALSE;
    log_debug("not supported\n");
  }

  return ret;
}

static ret_t books_exec(object_t* obj, const char* name, const char* args) {
  bool_t ret = TRUE;
  uint32_t index = tk_atoi(args);

  if (tk_str_ieq(name, BOOK_CMD_REMOVE)) {
    ret = books_remove(MODEL(obj), index) == RET_OK;
  } else if (tk_str_ieq(name, BOOK_CMD_SALE)) {
    ret = books_set_stock_delta(MODEL(obj), index, -1) == RET_OK;
    object_notify_changed(obj);
  } else if (tk_str_ieq(name, BOOK_CMD_ADD)) {
    ret = books_add(MODEL(obj), "name", 123) == RET_OK;
  } else if (tk_str_ieq(name, BOOK_CMD_CLEAR)) {
    ret = books_clear(MODEL(obj)) == RET_OK;
  } else {
    ret = FALSE;
    log_debug("not supported\n");
  }

  return RET_OK;
}

static const object_vtable_t s_books_vtable = {.type = "books",
                                               .desc = "books",
                                               .size = sizeof(books_t),
                                               .exec = books_exec,
                                               .is_collection = TRUE,
                                               .can_exec = books_can_exec,
                                               .get_prop = books_get_prop,
                                               .set_prop = books_set_prop};

model_t* books_create(navigator_request_t* req) {
  model_t* model = MODEL(object_create(&s_books_vtable));
  books_t* books = BOOKS(model);
  return_value_if_fail(books != NULL, NULL);

  darray_init(&(books->books), 100, (tk_destroy_t)book_info_destroy, (tk_compare_t)book_info_cmp);

  books_add(model, "green book", 100);
  books_add(model, "red book", 100);

  return model;
}

ret_t books_remove(model_t* model, uint32_t index) {
  ret_t ret = RET_FAIL;
  books_t* books = BOOKS(model);
  return_value_if_fail(books != NULL, ret);

  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  ret = darray_remove_index(&(books->books), index);
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return ret;
}

ret_t books_add(model_t* model, const char* name, uint32_t stock) {
  ret_t ret = RET_FAIL;
  book_info_t* info = NULL;
  books_t* books = BOOKS(model);
  return_value_if_fail(books != NULL && name != NULL, ret);

  info = book_info_create(name, stock);
  return_value_if_fail(info != NULL, ret);

  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  ret = darray_push(&(books->books), info);
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return ret;
}

uint32_t books_count(model_t* model) {
  books_t* books = BOOKS(model);
  return_value_if_fail(books != NULL, 0);

  return books->books.size;
}

ret_t books_clear(model_t* model) {
  ret_t ret = RET_OK;
  books_t* books = BOOKS(model);

  return_value_if_fail(books != NULL, RET_BAD_PARAMS);

  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  ret = darray_clear(&(books->books));
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return ret;
}

book_info_t* books_get(model_t* model, uint32_t index) {
  books_t* books = BOOKS(model);

  return_value_if_fail(books != NULL && index < books_count(model), NULL);

  return (book_info_t*)(books->books.elms[index]);
}

ret_t books_set_stock(model_t* model, uint32_t index, uint32_t stock) {
  char name[64];
  tk_snprintf(name, sizeof(name), "[%d].%s", index, BOOK_PROP_STOCK);

  return object_set_prop_int(OBJECT(model), name, stock);
}

ret_t books_set_stock_delta(model_t* model, uint32_t index, int32_t delta) {
  int32_t stock = 0;
  books_t* books = BOOKS(model);
  book_info_t* info = NULL;
  return_value_if_fail(books != NULL && index < books_count(model), RET_BAD_PARAMS);
  info = (book_info_t*)(books->books.elms[index]);

  stock = info->stock + delta;
  return_value_if_fail(stock >= 0, RET_FAIL);

  return books_set_stock(model, index, stock);
}
