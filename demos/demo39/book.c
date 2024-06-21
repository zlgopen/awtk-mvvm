#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"

#include "book.h"

book_t* book_create(void) {
  book_t* book = TKMEM_ZALLOC(book_t);
  return_value_if_fail(book != NULL, NULL);

  str_init(&(book->name), 32);
  str_random(&(book->name), "book %d", 10000);
  book->stock = random() % 100;

  return book;
}

ret_t book_destroy(book_t* book) {
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  str_reset(&(book->name));

  TKMEM_FREE(book);

  return RET_OK;
}

ret_t book_sale(book_t* book) {
  book->stock--;
  return RET_OBJECT_CHANGED;
}

bool_t book_can_sale(book_t* book) {
  return book->stock > 0;
}

int book_cmp(book_t* a, book_t* b) {
  return_value_if_fail(a != NULL && b != NULL, -1);
  return strcmp(a->name.str, b->name.str);
}
