/**
 * File:   book.c
 * Author: AWTK Develop Team
 * Brief:  book
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-05-28 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include "awtk.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"

#include "book.h"

static ret_t book_sale(tk_object_t* obj) {
  book_t* book = BOOK(obj);
  book->stock--;
  return RET_OBJECT_CHANGED;
}

static bool_t book_can_sale(tk_object_t* obj) {
  book_t* book = BOOK(obj);
  return book->stock > 0;
}

static ret_t book_get_name(tk_object_t* obj, value_t* v) {
  book_t* book = BOOK(obj);
  value_set_str(v, book->name.str);
  return RET_OK;
}

static ret_t book_get_stock(tk_object_t* obj, value_t* v) {
  book_t* book = BOOK(obj);
  value_set_uint32(v, book->stock);
  return RET_OK;
}

static ret_t book_set_name(tk_object_t* obj, const value_t* v) {
  book_t* book = BOOK(obj);
  str_set(&(book->name), value_str(v));
  return RET_OK;
}

static ret_t book_set_stock(tk_object_t* obj, const value_t* v) {
  book_t* book = BOOK(obj);
  book->stock = value_uint32(v);
  return RET_OK;
}

static ret_t book_destroy(tk_object_t* obj) {
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  book_t* book = BOOK(obj);
  str_reset(&(book->name));

  return RET_OK;
}

static int32_t book_compare(tk_object_t* obj, tk_object_t* other) {
  return_value_if_fail(obj != NULL && other != NULL, -1);
  book_t* a = BOOK(obj);
  book_t* b = BOOK(other);
  return strcmp(a->name.str, b->name.str);
}

static ret_t book_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  book_t* book = BOOK(obj);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "name")) {
    return book_get_name(obj, v);
  } else if (tk_str_eq(name, "stock")) {
    return book_get_stock(obj, v);
  }

  return RET_NOT_FOUND;
}

static ret_t book_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  book_t* book = BOOK(obj);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "name")) {
    return book_set_name(obj, v);
  } else if (tk_str_eq(name, "stock")) {
    return book_set_stock(obj, v);
  }

  return RET_NOT_FOUND;
}

static bool_t book_can_exec(tk_object_t* obj, const char* name, const char* args) {
  book_t* book = BOOK(obj);
  return_value_if_fail(book != NULL, FALSE);

  if (tk_str_eq(name, "sale")) {
    return book_can_sale(obj);
  }

  return FALSE;
}

static ret_t book_exec(tk_object_t* obj, const char* name, const char* args) {
  book_t* book = BOOK(obj);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "sale")) {
    return book_sale(obj);
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_book_vtable = {.type = "book",
                                              .desc = "book",
                                              .size = sizeof(book_t),
                                              .is_collection = FALSE,
                                              .on_destroy = book_destroy,
                                              .compare = book_compare,
                                              .get_prop = book_get_prop,
                                              .set_prop = book_set_prop,
                                              .remove_prop = NULL,
                                              .foreach_prop = NULL,
                                              .can_exec = book_can_exec,
                                              .exec = book_exec,
                                              .clone = NULL};

tk_object_t* book_create(void) {
  tk_object_t* obj = tk_object_create(&s_book_vtable);
  return_value_if_fail(obj != NULL, NULL);

  book_t* book = BOOK(obj);
  str_init(&(book->name), 32);
  str_random(&(book->name), "book %d", 10000);
  book->stock = random() % 100;

  return obj;
}

book_t* book_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_book_vtable, NULL);
  return (book_t*)(obj);
}
