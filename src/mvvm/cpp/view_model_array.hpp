/**
 * File:  view_model_array.hpp
 * Author: AWTK Develop Team
 * Brief:  view_model array interface for cpp
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VIEW_MODEL_ARRAY_HPP
#define TK_VIEW_MODEL_ARRAY_HPP

#include "mvvm/cpp/view_model.hpp"

namespace vm {

class ViewModelArray : public ViewModel {
 public:
  ViewModelArray(navigator_request_t* request);
  virtual ~ViewModelArray();

 public:
  /**
   * @method GetSize
   * 获取数据的条数。
   *
   * @return {uint32_t} 返回数据的条数。
   */
  virtual uint32_t GetSize(void) const;

  /**
   * @method Exec
   * 执行指定的命令。
   *
   * @param {int32_t} index 索引。
   * @param {const char*} name 命令名。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t Exec(int32_t index, const char* name);

  /**
   * @method CanExec
   * 检查指定的命令是否可以执行。
   *
   * @param {int32_t} index 索引。
   * @param {const char*} name 命令名。
   *
   * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
   */
  virtual bool_t CanExec(int32_t index, const char* name) const;

  /**
   * @method GetProp
   * 获取指定属性的值。
   *
   * @param {int32_t} index 索引。
   * @param {const char*} name 属性名。
   * @param {value_t*} value 属性值。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t GetProp(int32_t index, const char* name, value_t* v) const;

  /**
   * @method SetProp
   * 设置指定属性的值。
   *
   * @param {int32_t} index 索引。
   * @param {const char*} name 属性名。
   * @param {value_t*} value 属性值。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t SetProp(int32_t index, const char* name, const value_t* v);

  /**
   * @method Remove
   * 删除指定的条目。
   *
   * @param {int32_t} index 索引。
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t Remove(int32_t index);

  /**
   * @method Clear
   * 清除全部条目。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t Clear();
};

}  // namespace vm

#endif /*TK_VIEW_MODEL_ARRAY_HPP*/
