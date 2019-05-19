#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "book.h"


/***************book***************/;

static book_t* book_create(void) {
  book_t* book = TKMEM_ZALLOC(book_t);
  return_value_if_fail(book != NULL, NULL);
  book->name = str_gen_random("book %d");
  return book;
} 

int book_cmp(book_t* a, book_t* b) {
  return_value_if_fail(a != NULL && b != NULL, -1);
  return strcmp(a->name, b->name);
}

static ret_t book_destroy(book_t* book) {
  TKMEM_FREE(book->name);

  TKMEM_FREE(book);

  return RET_OK;
}

static bool_t book_can_exec_sale(book_t* book, const char* args) {
  return book->stock > 0;
}

static ret_t book_sale(book_t* book, const char* args) {
  book->stock--;
  return RET_OBJECT_CHANGED;
}


/***************book_view_model***************/

static ret_t book_view_model_remove(view_model_t* view_model, uint32_t index) {
  book_view_model_t* book_vm = (book_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, RET_BAD_PARAMS);

  return darray_remove_index(&(book_vm->books), index);
}

static ret_t book_view_model_add(view_model_t* view_model, book_t* book) {
  book_view_model_t* book_vm = (book_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL && book != NULL, RET_BAD_PARAMS);

  return darray_push(&(book_vm->books), book);
}

static uint32_t book_view_model_size(view_model_t* view_model) {
  book_view_model_t* book_vm = (book_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, 0); 

  return book_vm->books.size;
}

static ret_t book_view_model_clear(view_model_t* view_model) {
  book_view_model_t* book_vm = (book_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, 0); 

  return darray_clear(&(book_vm->books));
}

static book_t* book_view_model_get(view_model_t* view_model, uint32_t index) {
  book_view_model_t* book_vm = (book_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, 0); 

  return_value_if_fail(book_vm != NULL && index < book_view_model_size(view_model), NULL);

  return (book_t*)(book_vm->books.elms[index]);
}

static ret_t book_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  book_t* book = NULL;
  view_model_t* vm = VIEW_MODEL(obj);

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(vm, value_int(v));

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  book = book_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq("isbn", name)) {
    book->isbn = tk_str_copy(book->isbn, value_str(v));
  } else if (tk_str_eq("name", name)) {
    book->name = tk_str_copy(book->name, value_str(v));
  } else if (tk_str_eq("stock", name)) {
    book->stock = value_uint32(v);
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static ret_t book_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  book_t* book = NULL;
  view_model_t* vm = VIEW_MODEL(obj);

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, book_view_model_size(VIEW_MODEL(obj)));

    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  book = book_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq("isbn", name)) {
    char* isbn = book->isbn;
    value_set_str(v, isbn);
  } else if (tk_str_eq("name", name)) {
    char* name = book->name;
    value_set_str(v, name);
  } else if (tk_str_eq("stock", name)) {
    uint32_t stock = book->stock;
    value_set_uint32(v, stock);
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}


static bool_t book_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* vm = VIEW_MODEL(obj);
  book_t* book = book_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, "remove")) {
    return index < book_view_model_size(vm);
  } else if (tk_str_ieq(name, "add")) {
    return TRUE;
  } else if (tk_str_ieq(name, "clear")) {
    return book_view_model_size(vm) > 0;
  } else if (tk_str_eq("sale", name)) {
    return book_can_exec_sale(book, args);
  } else {
    return FALSE;
  }
}

static ret_t book_view_model_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* vm = VIEW_MODEL(obj);
  book_t* book = book_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, "remove")) {
    ENSURE(book_view_model_remove(vm, index) == RET_OK);
    return RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, "add")) {
    ENSURE(book_view_model_add(vm, book_create()) == RET_OK);
    return RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, "clear")) {
    ENSURE(book_view_model_clear(vm) == RET_OK);
    return RET_ITEMS_CHANGED;
  } else if (tk_str_eq("sale", name)) {
    return book_sale(book, args);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t book_view_model_on_destroy(object_t* obj) {
  book_view_model_t* vm = (book_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);
  
  book_view_model_clear(VIEW_MODEL(obj));
  darray_deinit(&(vm->books));

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_book_view_model_vtable = {
  .type = "book",
  .desc = "book store",
  .is_collection = TRUE,
  .size = sizeof(book_view_model_t),
  .exec = book_view_model_exec,
  .can_exec = book_view_model_can_exec,
  .get_prop = book_view_model_get_prop,
  .set_prop = book_view_model_set_prop,
  .on_destroy = book_view_model_on_destroy
};

view_model_t* book_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_book_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  book_view_model_t* book_vm = (book_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  darray_init(&(book_vm->books), 100, 
    (tk_destroy_t)book_destroy, (tk_compare_t)book_cmp);

  return vm;
}
