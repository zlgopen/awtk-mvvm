
#ifndef TK_BOOK_H
#define TK_BOOK_H

#include "tkc/darray.h"
#include "mvvm/base/view_model_array.h"

BEGIN_C_DECLS

/**
 * @class book_t
 *
 * book store
 *
 */
typedef struct _book_t {
  char* name;
  uint32_t stock;
} book_t;

/**
 * @class books_view_model_t
 *
 * view model of book
 *
 */
typedef struct _books_view_model_t {
  view_model_array_t view_model_array;

  /*model object*/
  darray_t books;
} books_view_model_t;

/**
 * @method books_view_model_create
 * 创建book view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* books_view_model_create(navigator_request_t* req);

/*public for test*/

book_t* book_create(void);

ret_t books_view_model_clear(view_model_t* view_model);
uint32_t books_view_model_size(view_model_t* view_model);
ret_t books_view_model_remove(view_model_t* view_model, uint32_t index);
ret_t books_view_model_add(view_model_t* view_model, book_t* book);
book_t* books_view_model_get(view_model_t* view_model, uint32_t index);

END_C_DECLS

#endif /*TK_BOOK_H*/
