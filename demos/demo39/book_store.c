#include "tkc/mem.h"
#include "tkc/utils.h"
#include "book_store.h"
#include "mvvm/base/utils.h"

ret_t book_store_remove(book_store_t* book_store, uint32_t index) {
  return_value_if_fail(book_store != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < book_store_get_items(book_store), RET_BAD_PARAMS);

  darray_remove_index(&(book_store->books), index);

  return RET_ITEMS_CHANGED;
}

bool_t book_store_can_remove(book_store_t* book_store, uint32_t index) {
  return_value_if_fail(book_store != NULL, FALSE);

  return index < book_store_get_items(book_store);
}

ret_t book_store_add(book_store_t* book_store) {
  book_t* book = NULL;
  return_value_if_fail(book_store != NULL, RET_BAD_PARAMS);
  book = book_create();
  return_value_if_fail(book != NULL, RET_OOM);

  ENSURE(darray_push(&(book_store->books), book) == RET_OK);

  return RET_ITEMS_CHANGED;
}

uint32_t book_store_get_items(book_store_t* book_store) {
  return_value_if_fail(book_store != NULL, 0);

  return book_store->books.size;
}

ret_t book_store_clear(book_store_t* book_store) {
  return_value_if_fail(book_store != NULL, RET_BAD_PARAMS);

  darray_clear(&(book_store->books));

  return RET_ITEMS_CHANGED;
}

book_t* book_store_get(book_store_t* book_store, uint32_t index) {
  return_value_if_fail(book_store != NULL, NULL);
  return_value_if_fail(index < book_store_get_items(book_store), NULL);

  return (book_t*)(book_store->books.elms[index]);
}

ret_t book_store_destroy(book_store_t* book_store) {
  return_value_if_fail(book_store != NULL, RET_BAD_PARAMS);

  darray_deinit(&(book_store->books));
  TKMEM_FREE(book_store);

  return RET_OK;
}

book_store_t* book_store_create(void) {
  book_store_t* book_store = TKMEM_ZALLOC(book_store_t);
  return_value_if_fail(book_store != NULL, NULL);

  darray_init(&(book_store->books), 100, (tk_destroy_t)book_destroy, (tk_compare_t)book_cmp);

  return book_store;
}
