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

static ret_t books_generate_data(books_t* books, uint32_t nr);

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
  book_info_t* info = NULL;

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(VIEW_MODEL(obj), value_int(v));

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  info = books_get(VIEW_MODEL(obj), index);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

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

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, books_size(VIEW_MODEL(obj)));

    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  info = books_get(VIEW_MODEL(obj), index);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

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
    ret = index < books_size(VIEW_MODEL(obj));
  } else if (tk_str_ieq(name, BOOK_CMD_SALE)) {
    const book_info_t* info = books_get(VIEW_MODEL(obj), index);
    ret = info != NULL && info->stock > 0;
  } else if (tk_str_ieq(name, BOOK_CMD_ADD)) {
    ret = TRUE;
  } else if (tk_str_ieq(name, BOOK_CMD_CLEAR)) {
    ret = books_size(VIEW_MODEL(obj)) > 0;
  } else {
    ret = FALSE;
    log_debug("not supported\n");
  }

  return ret;
}

static ret_t books_exec(object_t* obj, const char* name, const char* args) {
  ret_t ret = TRUE;
  uint32_t index = tk_atoi(args);

  if (tk_str_ieq(name, BOOK_CMD_REMOVE)) {
    ENSURE(books_remove(VIEW_MODEL(obj), index) == RET_OK);
    ret = RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, BOOK_CMD_SALE)) {
    ENSURE(books_sale(VIEW_MODEL(obj), index) == RET_OK);
    ret = RET_OBJECT_CHANGED;
  } else if (tk_str_ieq(name, BOOK_CMD_ADD)) {
    ENSURE(books_generate_data(BOOKS(obj), 1) == RET_OK);
    ret = RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, BOOK_CMD_CLEAR)) {
    ENSURE(books_clear(VIEW_MODEL(obj)) == RET_OK);
    ret = RET_ITEMS_CHANGED;
  } else {
    ret = RET_FAIL;
    log_debug("not supported\n");
  }

  return ret;
}

static ret_t books_on_destroy(object_t* obj) {
  books_clear(VIEW_MODEL(obj));
  darray_deinit(&(BOOKS(obj)->books));

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_books_vtable = {.type = "books",
                                               .desc = "books",
                                               .size = sizeof(books_t),
                                               .exec = books_exec,
                                               .is_collection = TRUE,
                                               .can_exec = books_can_exec,
                                               .get_prop = books_get_prop,
                                               .set_prop = books_set_prop,
                                               .on_destroy = books_on_destroy};

static ret_t books_generate_data(books_t* books, uint32_t nr) {
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    char name[TK_NAME_LEN + 1];
    tk_snprintf(name, TK_NAME_LEN, "book %d", random() % 10000);

    books_add(VIEW_MODEL(books), name, random() % 100);
  }

  return RET_OK;
}

view_model_t* books_create(navigator_request_t* req) {
  view_model_t* view_model = view_model_array_init(VIEW_MODEL(object_create(&s_books_vtable)));
  books_t* books = BOOKS(view_model);
  return_value_if_fail(books != NULL, NULL);

  darray_init(&(books->books), 100, (tk_destroy_t)book_info_destroy, (tk_compare_t)book_info_cmp);

  books_generate_data(books, 100);

  return view_model;
}

ret_t books_remove(view_model_t* view_model, uint32_t index) {
  books_t* books = BOOKS(view_model);
  return_value_if_fail(books != NULL, RET_BAD_PARAMS);

  return darray_remove_index(&(books->books), index);
}

ret_t books_add(view_model_t* view_model, const char* name, uint32_t stock) {
  book_info_t* info = NULL;
  books_t* books = BOOKS(view_model);
  return_value_if_fail(books != NULL && name != NULL, RET_BAD_PARAMS);

  info = book_info_create(name, stock);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  return darray_push(&(books->books), info);
}

uint32_t books_size(view_model_t* view_model) {
  books_t* books = BOOKS(view_model);
  return_value_if_fail(books != NULL, 0);

  return books->books.size;
}

ret_t books_clear(view_model_t* view_model) {
  books_t* books = BOOKS(view_model);
  return_value_if_fail(books != NULL, RET_BAD_PARAMS);

  return darray_clear(&(books->books));
}

book_info_t* books_get(view_model_t* view_model, uint32_t index) {
  books_t* books = BOOKS(view_model);

  return_value_if_fail(books != NULL && index < books_size(view_model), NULL);

  return (book_info_t*)(books->books.elms[index]);
}

ret_t books_sale(view_model_t* view_model, uint32_t index) {
  books_t* books = BOOKS(view_model);
  book_info_t* info = books_get(view_model, index);
  return_value_if_fail(books != NULL && info != NULL, RET_BAD_PARAMS);

  return_value_if_fail(info->stock > 0, RET_FAIL);
  info->stock--;

  return RET_OK;
}
