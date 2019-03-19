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

#define PROP_NAME "name"
#define PROP_STOCK "stock"
#define PROP_PUBLISH_DATE "publish_date"

#define CMD_ADD "add"
#define CMD_REMOVE "remove"

book_info_t* book_info_init(book_info_t* info, const char* name, float_t stock, uint32_t date) {
  return_value_if_fail(info != NULL && name != NULL, NULL);

  info->stock = stock;
  info->publish_date = date;
  tk_strncpy(info->name, name, TK_NAME_LEN);

  return info;
}

book_info_t* book_info_create(const char* name, float_t stock, uint32_t publish_date) {
  book_info_t* info = NULL;
  return_value_if_fail(name != NULL, NULL);

  info = TKMEM_ZALLOC(book_info_t);
  return_value_if_fail(info != NULL, NULL);

  return book_info_init(info, name, stock, publish_date);
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
  return_value_if_fail(index < books->books.size, RET_BAD_PARAMS);
  info = (book_info_t*)(books->books.elms[index]);

  if (tk_str_eq(PROP_NAME, name)) {
    tk_strncpy(info->name, value_str(v), TK_NAME_LEN);
  } else if (tk_str_eq(PROP_STOCK, name)) {
    info->stock = value_float(v);
  } else if (tk_str_eq(PROP_PUBLISH_DATE, name)) {
    info->publish_date = value_int(v);
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
    value_set_int(v, books->books.size);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < books->books.size, RET_BAD_PARAMS);
  info = (book_info_t*)(books->books.elms[index]);

  if (tk_str_eq(PROP_NAME, name)) {
    value_set_str(v, info->name);
  } else if (tk_str_eq(PROP_STOCK, name)) {
    value_set_float(v, info->stock);
  } else if (tk_str_eq(PROP_PUBLISH_DATE, name)) {
    value_set_int(v, info->publish_date);
  } else if (tk_str_eq(MODEL_PROP_ITEMS, name)) {
    value_set_int(v, books->books.size);
  } else {
    ret = RET_NOT_FOUND;
  }

  return ret;
}

static bool_t books_can_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_ieq(name, CMD_REMOVE)) {
  } else {
    log_debug("not supported\n");
  }

  return FALSE;
}

static ret_t books_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_ieq(name, CMD_REMOVE)) {
    log_debug("save\n");
  } else {
    log_debug("not supported\n");
    return RET_NOT_FOUND;
  }

  object_notify_changed(obj);

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

  books_add(model, "green book", 100, 0);
  books_add(model, "red book", 100, 0);

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

ret_t books_add(model_t* model, const char* name, uint32_t stock, uint32_t publish_date) {
  ret_t ret = RET_FAIL;
  book_info_t* info = NULL;
  books_t* books = BOOKS(model);
  return_value_if_fail(books != NULL && name != NULL, ret);

  info = book_info_create(name, stock, publish_date);
  return_value_if_fail(info != NULL, ret);

  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_WILL_CHANGE);
  ret = darray_push(&(books->books), info);
  emitter_dispatch_simple_event(EMITTER(model), EVT_ITEMS_CHANGED);

  return ret;
}

book_info_t* books_get(model_t* model, uint32_t index) {
  books_t* books = BOOKS(model);

  return_value_if_fail(books != NULL && index < books->books.size, NULL);

  return (book_info_t*)(books->books.elms[index]);
}

ret_t books_set_stock(model_t* model, uint32_t index, uint32_t stock) {
  char name[64];
  tk_snprintf(name, sizeof(name), "[%d].%s", index, PROP_STOCK);

  return object_set_prop_int(OBJECT(model), name, stock);
}
