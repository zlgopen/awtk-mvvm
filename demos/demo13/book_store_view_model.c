
/**
 * File:   book_store_view_model.c
 * Author: AWTK Develop Team
 * Brief:  book_store_view_model
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

#include "tkc/utils.h"
#include "tkc/object_array.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "book_view_model.h"
#include "book_store_view_model.h"

static ret_t book_store_view_model_add_book(book_store_view_model_t* vm) {
  value_t v;
  tk_object_t* book = book_create();
  return_value_if_fail(book != NULL, RET_FAIL);

  value_set_object(&v, book);
  emitter_on(EMITTER(book), EVT_PROP_CHANGED, emitter_forward, vm);
  if (object_array_push(vm->abook_store, &v) == RET_OK) {
    view_model_notify_items_changed(VIEW_MODEL(vm), vm->abook_store);
  }
  TK_OBJECT_UNREF(book);
  return RET_OK;
}

static ret_t book_store_view_model_insert_category(book_store_view_model_t* vm) {
  value_t v;
  char name[100] = {0};
  int32_t len = tk_object_get_prop_int32(vm->categories, "length", -1);
  tk_object_t* category = object_default_create();
  return_value_if_fail(category != NULL, RET_FAIL);

  tk_snprintf(name, sizeof(name) - 1, "Category%d", random() % 1000);
  tk_object_set_prop_str(category, "name", name);
  tk_object_set_prop_int(category, "booksCount", random() % 1000);

  value_set_object(&v, category);
  emitter_on(EMITTER(category), EVT_PROP_CHANGED, emitter_forward, vm);

  if (vm->selected_category < 0 || vm->selected_category >= len) {
    object_array_push(vm->categories, &v);
    vm->last_inserted_category = len;
  } else {
    object_array_insert(vm->categories, vm->selected_category, &v);
    vm->last_inserted_category = vm->selected_category;
    vm->selected_category = vm->selected_category + 1;
  }

  TK_OBJECT_UNREF(category);
  return RET_OK;
}

static ret_t book_store_view_model_on_create(view_model_t* view_model, navigator_request_t* req) {
  uint32_t i;
  book_store_view_model_t* vm = (book_store_view_model_t*)(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  vm->abook_store = object_array_create();

  vm->selected_category = -1;
  vm->categories = object_array_create();
  emitter_on(EMITTER(vm->categories), EVT_ITEMS_CHANGED, emitter_forward, vm);

  for (i = 0; i < 10; i++) {
    book_store_view_model_insert_category(vm);
  }

  for (i = 0; i < 50; i++) {
    book_store_view_model_add_book(vm);
  }

  return RET_OK;
}

static ret_t book_store_view_model_on_destroy(tk_object_t* obj) {
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(vm->abook_store);
  TK_OBJECT_UNREF(vm->categories);

  return view_model_deinit(VIEW_MODEL(obj));
}

static ret_t book_store_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t book_store_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  book_store_view_model_t* vm = ((book_store_view_model_t*)(obj));

  tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
  if (sub != NULL) {
    return tk_object_get_prop(sub, name, v);
  }

  if (tk_str_ieq("items", name)) {
    value_set_object(v, vm->abook_store);
    return RET_OK;
  } else if (tk_str_ieq("categories", name)) {
    value_set_object(v, vm->categories);
    return RET_OK;
  } else if (tk_str_ieq("selectedCategory", name)) {
    value_set_int(v, vm->selected_category);
    return RET_OK;
  } else if (tk_str_ieq("lastInsertedCategory", name)) {
    value_set_int(v, vm->last_inserted_category);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t book_store_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
  uint32_t index = 0;
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, FALSE);

  if (TK_STR_IS_NOT_EMPTY(args)) {
    tk_object_t* a = object_default_create();
    tk_command_arguments_to_object(args, a);
    index = tk_object_get_prop_int32(a, "index", -1);
    TK_OBJECT_UNREF(a);
  }

  if (tk_str_ieq("addBook", name)) {
    return TRUE;
  } else if (tk_str_ieq("clearBooks", name)) {
    int32_t len = tk_object_get_prop_int32(vm->abook_store, "length", -1);
    return len > 0;
  } else if (tk_str_ieq("removeCurrentBook", name)) {
    return TRUE;
  } else if (tk_str_ieq("saleBook", name)) {
    value_t v;
    object_array_get(vm->abook_store, index, &v);
    view_model_t* book_view_model = book_view_model_create(value_object(&v));
    return_value_if_fail(book_view_model != NULL, FALSE);

    bool_t ret = view_model_can_exec(book_view_model, "sale", args);
    TK_OBJECT_UNREF(book_view_model);
    return ret;
  } else if (tk_str_ieq("insertCategory", name)) {
    return TRUE;
  } else if (tk_str_ieq("clearCategories", name)) {
    int32_t len = tk_object_get_prop_int32(vm->categories, "length", -1);
    return len > 0;
  } else if (tk_str_ieq("removeSelectedCategory", name)) {
    int32_t len = tk_object_get_prop_int32(vm->categories, "length", -1);
    int32_t index = vm->selected_category;
    return index >= 0 && index < len;
  } else if (tk_str_ieq("incBooksCount", name)) {
    return TRUE;
  } else if (tk_str_ieq("decBooksCount", name)) {
    value_t v;
    if (object_array_get(vm->categories, index, &v) == RET_OK) {
      tk_object_t* category = value_object(&v);
      return tk_object_get_prop_int(category, "booksCount", 0) > 0;
    }
    return FALSE;
  } else if (tk_str_ieq("setSelectedCategory", name)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t book_store_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
  uint32_t index = 0;
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);

  if (TK_STR_IS_NOT_EMPTY(args)) {
    tk_object_t* a = object_default_create();
    tk_command_arguments_to_object(args, a);
    index = tk_object_get_prop_int32(a, "index", -1);
    TK_OBJECT_UNREF(a);
  }

  if (tk_str_ieq("addBook", name)) {
    return book_store_view_model_add_book(vm);
  } else if (tk_str_ieq("clearBooks", name)) {
    if (object_array_clear_props(vm->abook_store) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->abook_store);
    }
    return RET_OK;
  } else if (tk_str_ieq("removeCurrentBook", name)) {
    if (object_array_remove(vm->abook_store, index) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->abook_store);
    }
    return RET_OK;
  } else if (tk_str_ieq("saleBook", name)) {
    value_t v;
    object_array_get(vm->abook_store, index, &v);
    view_model_t* book_view_model = book_view_model_create(value_object(&v));
    return_value_if_fail(book_view_model != NULL, RET_FAIL);

    ret_t ret = view_model_exec(book_view_model, "sale", args);
    TK_OBJECT_UNREF(book_view_model);
    return ret;
  } else if (tk_str_ieq("insertCategory", name)) {
    return book_store_view_model_insert_category(vm);
  } else if (tk_str_ieq("clearCategories", name)) {
    object_array_clear_props(vm->categories);
    return RET_OK;
  } else if (tk_str_ieq("setSelectedCategory", name)) {
    vm->selected_category = index;
    return RET_OBJECT_CHANGED;
  } else if (tk_str_ieq("removeSelectedCategory", name)) {
    object_array_remove(vm->categories, vm->selected_category);
    vm->selected_category = -1;
    return RET_OBJECT_CHANGED;
  } else if (tk_str_ieq("incBooksCount", name)) {
    value_t v;
    if (object_array_get(vm->categories, index, &v) == RET_OK) {
      tk_object_t* category = value_object(&v);
      int32_t i = tk_object_get_prop_int(category, "booksCount", 0);
      tk_object_set_prop_int(category, "booksCount", i + 1);
    }
    return RET_OK;
  } else if (tk_str_ieq("decBooksCount", name)) {
    value_t v;
    if (object_array_get(vm->categories, index, &v) == RET_OK) {
      tk_object_t* category = value_object(&v);
      int32_t i = tk_object_get_prop_int(category, "booksCount", 0);
      if (i > 0) {
        tk_object_set_prop_int(category, "booksCount", i - 1);
      }
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_book_store_view_model_vtable = {
    .type = "book_store_view_model_t",
    .desc = "book_store_view_model_t",
    .size = sizeof(book_store_view_model_t),
    .on_destroy = book_store_view_model_on_destroy,
    .get_prop = book_store_view_model_get_prop,
    .set_prop = book_store_view_model_set_prop,
    .can_exec = book_store_view_model_can_exec,
    .exec = book_store_view_model_exec};

view_model_t* book_store_view_model_create(navigator_request_t* req) {
  tk_object_t* obj = tk_object_create(&s_book_store_view_model_vtable);
  view_model_t* view_model = view_model_init(VIEW_MODEL(obj));
  return_value_if_fail(view_model != NULL, NULL);

  book_store_view_model_on_create(view_model, req);

  return view_model;
}
