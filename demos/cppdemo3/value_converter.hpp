/**
 * File:   value_converter.hpp
 * Author: AWTK Develop Team
 * Brief:  value converter
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-06-01 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_TEMPERATURE_CONVERTER_HPP
#define TK_TEMPERATURE_CONVERTER_HPP

#include "mvvm/cpp/adapter.hpp"

/**
 * @class TemperatureValidator
 *
 * @annotation ["model", "cpp"]
 * 温度转换器。
 *
 */
class TemperatureConverter : public vm::ValueConverter {
 public:
  /**
   * @method Create
   * 转换器的 Create 函数。
   *
   * @annotation ["constructor"]
   *
   * @return {void*} 返回value_converter对象。
   */
  static void* Create(void);

 public:
  virtual ret_t ToModel(const value_t* from, value_t* to);
  virtual ret_t ToView(const value_t* from, value_t* to);
};

#endif /*TK_TEMPERATURE_CONVERTER_HPP*/
