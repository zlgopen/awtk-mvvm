/**
 * File:  view_model.hpp
 * Author: AWTK Develop Team
 * Brief:  view_model interface for cpp
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

#ifndef TK_VIEW_MODEL_HPP
#define TK_VIEW_MODEL_HPP

#include "mvvm/cpp/object.hpp"
#include "mvvm/base/navigator_request.h"

namespace vm {

/**
 * @class ViewModel
 *
 * C++ ViewModel的基类。
 *
 */
class ViewModel : public Object {
 public:
  ViewModel(navigator_request_t* request);
  virtual ~ViewModel();

 public:
  /**
   * @method OnWillMount
   * 打开视图即将加载模型时通知view_model。
   *
   * @param {navigator_request_t*} req request对象。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t OnWillMount(navigator_request_t* request);
  /**
   * @method OnMount
   * 视图与模型绑定完成后通知模型。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t OnMount();
  /**
   * @method OnWillUnmount
   * 视图即将关闭时通知模型。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t OnWillUnmount();
  /**
   * @method OnUnmount
   * 视图销毁时通知模型。
   *
   * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
   */
  virtual ret_t OnUnmount();

 public:
  navigator_request_t* request;
};

}  // namespace vm

#endif /*TK_VIEW_MODEL_HPP*/
