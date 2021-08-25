/**
 * File:   value_validator.hpp
 * Author: AWTK Develop Team
 * Brief:  value validator
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

#ifndef TK_TEMPERATURE_VALIDATOR_H
#define TK_TEMPERATURE_VALIDATOR_H

#include "mvvm/cpp/adapter.hpp"

/**
 * @class TemperatureValidator
 *
 * @annotation ["model", "cpp"]
 * 温度验证器。
 *
 */
class TemperatureValidator : public vm::ValueValidator {
 public:
  /**
   * @method Create
   * 验证器的 Create 函数。
   *
   * @annotation ["constructor"]
   *
   * @return {void*} 返回value_validator对象。
   */
  static void* Create(void);

 public:
  virtual bool_t IsValid(const value_t* value, str_t* msg);
  virtual ret_t Fix(value_t* value);
};

#endif /*TK_TEMPERATURE_VALIDATOR_H*/
