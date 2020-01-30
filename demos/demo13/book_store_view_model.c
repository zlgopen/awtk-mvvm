﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "book_store_view_model.h"

view_model_t* book_store_view_model_attach(object_t* obj, uint32_t index) {
  view_model_t* vm = VIEW_MODEL(obj);
  book_store_view_model_t* book_store_view_model = (book_store_view_model_t*)(vm);
  book_t* book = book_store_get(book_store_view_model->abook_store, index);
  view_model_t* book_view_model = book_store_view_model->book_view_model;

  book_view_model_attach(book_view_model, book);

  return VIEW_MODEL(book_view_model);
}

static ret_t book_store_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  view_model_t* view_model = VIEW_MODEL(obj);
  book_store_t* abook_store = ((book_store_view_model_t*)(obj))->abook_store;
  
  if(view_model_array_default_set_prop(view_model, name, v) == RET_OK) {
    return RET_OK;
  }

  if (tk_str_eq("items", name)) {
     
     return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  view_model = book_store_view_model_attach(obj, index);

  return view_model_set_prop(view_model, name, v);
}

static ret_t book_store_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  view_model_t* view_model = VIEW_MODEL(obj);
  book_store_t* abook_store = ((book_store_view_model_t*)(obj))->abook_store;
  
  if(view_model_array_default_get_prop(view_model, name, v) == RET_OK) {
    return RET_OK;
  }

  if (tk_str_eq("items", name)) {
     value_set_uint32(v, book_store_get_items(abook_store));
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
  book_store_t* abook_store = vm->abook_store;
  if (tk_str_eq("clear", name)) {
    return TRUE;

  } else if (tk_str_eq("remove", name)) {
    return TRUE;

  } else if (tk_str_eq("add", name)) {
    return TRUE;
  }
  
  view_model = book_store_view_model_attach(obj, index);

  return view_model_can_exec(view_model, name, NULL);
}

static ret_t book_store_view_model_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* view_model = VIEW_MODEL(obj);

 
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  book_store_t* abook_store = vm->abook_store;
  if (tk_str_eq("clear", name)) {
    return book_store_clear(abook_store);

  } else if (tk_str_eq("remove", name)) {
    return book_store_remove(abook_store, tk_atoi(args));

  } else if (tk_str_eq("add", name)) {
    return book_store_add(abook_store);
  }

  view_model = book_store_view_model_attach(obj, index);

  return view_model_exec(view_model, name, NULL);
}

static ret_t book_store_view_model_on_destroy(object_t* obj) {
  book_store_view_model_t* vm = (book_store_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  
  book_view_model_attach(vm->book_view_model, NULL);
  OBJECT_UNREF(vm->book_view_model);
  book_store_destroy(vm->abook_store);

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_book_store_view_model_vtable = {
  .type = "book_store_view_model_t",
  .desc = "book_store_view_model_t",
  .is_collection = TRUE,
  .size = sizeof(book_store_view_model_t),
  .exec = book_store_view_model_exec,
  .can_exec = book_store_view_model_can_exec,
  .get_prop = book_store_view_model_get_prop,
  .set_prop = book_store_view_model_set_prop,
  .on_destroy = book_store_view_model_on_destroy
};

view_model_t* book_store_view_model_create_with(book_store_t* abook_store) {
  object_t* obj = object_create(&s_book_store_view_model_vtable);
  view_model_t* vm = view_model_array_init(VIEW_MODEL(obj));
  book_store_view_model_t* book_store_view_model = (book_store_view_model_t*)(vm);
  
  book_store_view_model->book_view_model = book_view_model_create_with(NULL);
  return_value_if_fail(vm != NULL, NULL);

  book_store_view_model->abook_store = abook_store;
  ENSURE(book_store_view_model->abook_store != NULL);
  

  return vm;
}

view_model_t* book_store_view_model_create(navigator_request_t* req) {
  book_store_t* abook_store = book_store_create();
  return_value_if_fail(abook_store != NULL, NULL);

  return book_store_view_model_create_with(abook_store);
}
