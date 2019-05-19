
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
typedef struct _book_t{
  char* isbn;
  char* name;
  uint32_t stock;
} book_t;

/**
 * @class book_view_model_t
 *
 * view model of book
 *
 */
typedef struct _book_view_model_t {
  view_model_t view_model;

  /*model object*/
  darray_t books;
} book_view_model_t;

/**
 * @method book_view_model_create
 * 创建book view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* book_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_BOOK_H*/
