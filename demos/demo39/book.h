
#ifndef TK_BOOK_H
#define TK_BOOK_H

#include "tkc/str.h"

BEGIN_C_DECLS

/**
 * @class book_t
 * @annotation ["model"]
 * 图书信息。
 *
 */
typedef struct _book_t {
  /**
   * @property {str_t} name
   * @annotation ["readable", "writable"]
   * 书名。
   */
  str_t name;
  /**
   * @property {uint32_t} stock
   * @annotation ["readable", "writable"]
   * 库存。
   */
  uint32_t stock;
} book_t;

/**
 * @method book_create
 * 创建book对象。
 *
 * @annotation ["constructor"]
 * @return {book_t*} 返回book对象。
 */
book_t* book_create(void);

/**
 * @method book_destroy
 * 销毁book对象。
 *
 * @annotation ["destructor"]
 * @param {book_t*} book book对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t book_destroy(book_t* book);

/**
 * @method book_sale
 * 卖掉一本。
 *
 * @annotation ["command"]
 * @param {book_t*} book book对象。
 *
 * @return {ret_t}
 * 返回RET_OK表示成功；返回RET_OBJECT_CHANGED表示模型有变化，View需要刷新；返回其它表示失败。
 */
ret_t book_sale(book_t* book);

/**
 * @method book_can_sale
 * 检查sale命令是否可以执行。
 *
 * @param {book_t*} book book对象。
 *
 * @return {bool_t} 返回FALSE表示不能执行，否则表示可以执行。
 */
bool_t book_can_sale(book_t* book);

int book_cmp(book_t* a, book_t* b);

END_C_DECLS

#endif /*TK_BOOK_H*/
