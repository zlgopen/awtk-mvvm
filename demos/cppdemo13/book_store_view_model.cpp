﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "book_store_view_model.h"

view_model_t* book_store_view_model_attach(object_t* obj, uint32_t index) {
  view_model_t* vm = VIEW_MODEL(obj);
  book_store_view_model_t* book_store_view_model = (book_store_view_model_t*)(vm);
  Book* Book = book_store_view_model->aBookStore->Get(index);
  view_model_t* book_view_model = book_store_view_model->book_view_model;

  book_view_model_attach(book_view_model, Book);

  return VIEW_MODEL(book_view_model);
}

static ret_t book_store_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  view_model_t* view_model = VIEW_MODEL(obj);
  BookStore* aBookStore = ((book_store_view_model_t*)(obj))->aBookStore;
  
  if(view_model_array_default_set_prop(view_model, name, v) == RET_OK) {
    return RET_OK;
  }

  if (tk_str_ieq("items", name)) {
     
     return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  view_model = book_store_view_model_attach(obj, index);

  return view_model_set_prop(view_model, name, v);
}

static ret_t book_store_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  view_model_t* view_model = VIEW_MODEL(obj);
  BookStore* aBookStore = ((book_store_view_model_t*)(obj))->aBookStore;
  
  if(view_model_array_default_get_prop(view_model, name, v) == RET_OK) {
    return RET_OK;
  }

  if (tk_str_ieq("items", name)) {
     value_set_uint32(v, aBookStore->GetItems());
     return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  view_model = book_store_view_model_attach(obj, index);

  return view_model_get_prop(view_model, name, v);
}


static bool_t book_store_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* view_model = VIEW_MODEL(obj);

 
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  BookStore* aBookStore = vm->aBookStore;
  if (tk_str_ieq("Remove", name)) {
    return aBookStore->CanRemove(tk_atoi(args));

  } else if (tk_str_ieq("Clear", name)) {
    return TRUE;

  } else if (tk_str_ieq("Add", name)) {
    return TRUE;
  }
  
  view_model = book_store_view_model_attach(obj, index);

  return view_model_can_exec(view_model, name, NULL);
}

static ret_t book_store_view_model_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* view_model = VIEW_MODEL(obj);

 
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  BookStore* aBookStore = vm->aBookStore;
  if (tk_str_ieq("Remove", name)) {
    return aBookStore->Remove(tk_atoi(args));

  } else if (tk_str_ieq("Clear", name)) {
    return aBookStore->Clear();

  } else if (tk_str_ieq("Add", name)) {
    return aBookStore->Add();
  }

  view_model = book_store_view_model_attach(obj, index);

  return view_model_exec(view_model, name, NULL);
}

static ret_t book_store_view_model_on_destroy(object_t* obj) {
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  book_view_model_attach(vm->book_view_model, NULL);
  OBJECT_UNREF(vm->book_view_model);
  delete (vm->aBookStore);

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_book_store_view_model_vtable = {
  "book_store_view_model_t",
  "book_store_view_model_t",
  sizeof(book_store_view_model_t),
  TRUE,
  book_store_view_model_on_destroy,
  NULL,
  book_store_view_model_get_prop,
  book_store_view_model_set_prop,
  NULL,
  NULL,
  book_store_view_model_can_exec,
  book_store_view_model_exec
};

view_model_t* book_store_view_model_create_with(BookStore* aBookStore) {
  object_t* obj = object_create(&s_book_store_view_model_vtable);
  view_model_t* vm = view_model_array_init(VIEW_MODEL(obj));
  book_store_view_model_t* book_store_view_model = (book_store_view_model_t*)(vm);
  
  book_store_view_model->book_view_model = book_view_model_create_with(NULL);
  return_value_if_fail(vm != NULL, NULL);

  book_store_view_model->aBookStore = aBookStore;
  ENSURE(book_store_view_model->aBookStore != NULL);
  

  return vm;
}

view_model_t* book_store_view_model_create(navigator_request_t* req) {
  BookStore* aBookStore = new BookStore();
  return_value_if_fail(aBookStore != NULL, NULL);

  return book_store_view_model_create_with(aBookStore);
}
