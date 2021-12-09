
/**
 * File:   book_store_view_model.c
 * Author: AWTK Develop Team
 * Brief:  book_store_view_model
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t book_store_view_model_on_create(view_model_t* view_model, navigator_request_t* req) {
  book_store_view_model_t* vm = (book_store_view_model_t*)(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  vm->abook_store = object_array_create();

  return RET_OK;
}

static ret_t book_store_view_model_on_destroy(tk_object_t* obj) {
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(vm->abook_store);

  return view_model_deinit(VIEW_MODEL(obj));
}

static ret_t book_store_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  book_store_view_model_t* vm = ((book_store_view_model_t*)(obj));

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
  }

  return RET_NOT_FOUND;
}

static bool_t book_store_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
  uint32_t index = 0;
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, FALSE);

  if (args != NULL) {
    tk_object_t* a = object_default_create();
    tk_command_arguments_to_object(args, a);
    index = tk_object_get_prop_int32(a, "index", -1);
    TK_OBJECT_UNREF(a);
  }
  if (tk_str_ieq("add", name)) {
    return TRUE;
  } else if (tk_str_ieq("clear", name)) {
    int32_t len = tk_object_get_prop_int32(vm->abook_store, "length", -1);
    return len > 0;
  } else if (tk_str_ieq("remove", name)) {
    return TRUE;
  } else if (tk_str_ieq("sale", name)) {
    value_t v;
    object_array_get(vm->abook_store, index, &v);
    view_model_t* book_view_model = book_view_model_create(value_object(&v));
    return_value_if_fail(book_view_model != NULL, FALSE);

    bool_t ret = view_model_can_exec(book_view_model, name, args);
    TK_OBJECT_UNREF(book_view_model);
    return ret;
  }

  return FALSE;
}

static ret_t book_store_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
  uint32_t index = 0;
  view_model_t* view_model = VIEW_MODEL(obj);
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);

  if (args != NULL) {
    tk_object_t* a = object_default_create();
    tk_command_arguments_to_object(args, a);
    index = tk_object_get_prop_int32(a, "index", -1);
    TK_OBJECT_UNREF(a);
  }

  if (tk_str_ieq("add", name)) {
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
  } else if (tk_str_ieq("clear", name)) {
    if (object_array_clear_props(vm->abook_store) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->abook_store);
    }
    return RET_OK;
  } else if (tk_str_ieq("remove", name)) {
    if (object_array_remove(vm->abook_store, index) == RET_OK) {
      view_model_notify_items_changed(VIEW_MODEL(vm), vm->abook_store);
    }
    return RET_OK;
  } else if (tk_str_ieq("sale", name)) {
    value_t v;
    object_array_get(vm->abook_store, index, &v);
    view_model_t* book_view_model = book_view_model_create(value_object(&v));
    return_value_if_fail(book_view_model != NULL, RET_FAIL);

    ret_t ret = view_model_exec(book_view_model, name, args);
    TK_OBJECT_UNREF(book_view_model);
    return ret;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_book_store_view_model_vtable = {"book_store_view_model_t",
                                                               "book_store_view_model_t",
                                                               sizeof(book_store_view_model_t),
                                                               FALSE,
                                                               book_store_view_model_on_destroy,
                                                               NULL,
                                                               book_store_view_model_get_prop,
                                                               book_store_view_model_set_prop,
                                                               NULL,
                                                               NULL,
                                                               book_store_view_model_can_exec,
                                                               book_store_view_model_exec};

view_model_t* book_store_view_model_create(navigator_request_t* req) {
  tk_object_t* obj = tk_object_create(&s_book_store_view_model_vtable);
  view_model_t* view_model = view_model_init(VIEW_MODEL(obj));
  return_value_if_fail(view_model != NULL, NULL);

  book_store_view_model_on_create(view_model, req);

  return view_model;
}
