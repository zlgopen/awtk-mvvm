
#ifndef TK_BOOK_STORE_H
#define TK_BOOK_STORE_H

#include "book.h"
#include "tkc/darray.h"

BEGIN_C_DECLS

/**
 * @class book_store_t
 * book store。
 *
 * @annotation ["collection:book", "model"]
 *
 */
typedef struct _book_store_t {
  /**
   * @property {uint32_t} items
   * @annotation ["fake", "readable"]
   * 总数量。
   */

  /*private*/
  darray_t books;
} book_store_t; 

/**
 * @method book_store_create
 * 创建book_store对象。
 *
 * @annotation ["constructor"]
 *
 * @return {book_store_t*} 返回book_store对象。
 */
book_store_t* book_store_create(void);

/**
 * @method book_store_destroy
 * 销毁book_store对象。
 *
 * @annotation ["destructor"]
 * @param {book_store_t*} book_store book_store对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t book_store_destroy(book_store_t* book_store);

/**
 * @method book_store_clear
 * 清除全部数据。
 *
 * @annotation ["command"]
 * @param {book_store_t*} book_store book_store对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t book_store_clear(book_store_t* book_store);

/**
 * @method book_store_remove
 * 删除指定序数的book。
 *
 * @annotation ["command"]
 * @param {book_store_t*} book_store book_store对象。
 * @param {uint32_t} index 序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t book_store_remove(book_store_t* book_store, uint32_t index);

/**
 * @method book_store_can_remove
 * 检查remove命令是否可以执行。
 *
 * @param {book_store_t*} book_store book_store对象。
 * @param {uint32_t} index 序数。
 *
 * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
 */
bool_t book_store_can_remove(book_store_t* book_store, uint32_t index);

/**
 * @method book_store_get_items
 * 获取总数。
 *
 * @param {book_store_t*} book_store book_store对象。
 *
 * @return {uint32_t} 返回用户数。
 */
uint32_t book_store_get_items(book_store_t* book_store);

/**
 * @method book_store_get
 * 获取指定序数的图书。
 *
 * @param {book_store_t*} book_store book_store对象。
 * @param {uint32_t} index 序数。
 *
 * @return {book_t*} 返回book对象。
 */
book_t* book_store_get(book_store_t* book_store, uint32_t index);

/**
 * @method book_store_add
 * 增加一本图书。
 *
 * @annotation ["command"]
 * @param {book_store_t*} book_store book_store对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t book_store_add(book_store_t* book_store);

END_C_DECLS

#endif /*TK_BOOK_STORE_H*/
