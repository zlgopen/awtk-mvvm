﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "book_view_model.h"

static ret_t book_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  book_t* abook = ((book_view_model_t*)(obj))->abook;

  if (tk_str_ieq("name", name)) {
     str_set(&(abook->name), value_str(v));

     return RET_OK;
  } else if (tk_str_ieq("stock", name)) {
     abook->stock = value_uint32(v);

     return RET_OK;
  }
  
  return RET_NOT_FOUND;
}


static ret_t book_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  book_t* abook = ((book_view_model_t*)(obj))->abook;

  if (tk_str_ieq("name", name)) {
     value_set_str(v, abook->name.str);
     return RET_OK;
  } else if (tk_str_ieq("stock", name)) {
     value_set_uint32(v, abook->stock);
     return RET_OK;
  }

  return RET_NOT_FOUND;
}


static bool_t book_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
 
  book_view_model_t* vm = (book_view_model_t*)(obj);
  book_t* abook = vm->abook;
  if (tk_str_ieq("sale", name)) {
    return book_can_sale(abook);
  }
  return FALSE;
}

static ret_t book_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
 
  book_view_model_t* vm = (book_view_model_t*)(obj);
  book_t* abook = vm->abook;
  if (tk_str_ieq("sale", name)) {
    return book_sale(abook);
  }
  return RET_NOT_FOUND;
}

static ret_t book_view_model_on_destroy(tk_object_t* obj) {
  book_view_model_t* vm = (book_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  book_destroy(vm->abook);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_book_view_model_vtable = {
  .type = "book_view_model_t",
  .desc = "book_view_model_t",
  .size = sizeof(book_view_model_t),
  .is_collection = FALSE,
  .on_destroy = book_view_model_on_destroy,
  .compare = NULL,
  .get_prop = book_view_model_get_prop,
  .set_prop = book_view_model_set_prop,
  .remove_prop = NULL,
  .foreach_prop = NULL,
  .clear_props = NULL,
  .find_prop = NULL,
  .find_props = NULL,
  .can_exec = book_view_model_can_exec,
  .exec = book_view_model_exec,
  .clone = NULL
};

view_model_t* book_view_model_create_with(book_t* abook) {
  tk_object_t* obj = tk_object_create(&s_book_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  book_view_model_t* book_view_model = (book_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  book_view_model->abook = abook;
  

  return vm;
}

ret_t book_view_model_attach(view_model_t* vm, book_t* abook) {
  book_view_model_t* book_view_model = (book_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  book_view_model->abook = abook;

  return RET_OK;
}

view_model_t* book_view_model_create(navigator_request_t* req) {
  book_t* abook = book_create();
  return_value_if_fail(abook != NULL, NULL);

  return book_view_model_create_with(abook);
}
