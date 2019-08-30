/**
 * File:  value_validator.hpp
 * Author: AWTK Develop Team
 * Brief:  value validator interface for cpp
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

#ifndef TK_DATA_VALIDATOR_HPP
#define TK_DATA_VALIDATOR_HPP

#include "mvvm/base/value_validator.h"

/**
 * @class ValueValidator
 *
 * 值校验器。
 *
 * 用户在界面上输入的类型可能是无效的，ValueValidator负责将检查用户输入的有效性。
 *
 */
class ValueValidator {
  public:
    ValueValidator() {}
    virtual ~ValueValidator() {}
  
  public:
    /**
     * @method IsValid
     * 检查值是否有效。
     *
     * @param {value_t*} value 待验证的值。
     * @param {str_t*} str 返回错误信息。
     *
     * @return {bool_t} 返回TRUE表示有效，否则表示无效。
     */
    virtual bool_t IsValid(const value_t* value, str_t* msg) = 0;

    /**
     * @method Fix
     * 将无效值修改为正确的值。
     *
     * @param {value_t*} value 修正后的值。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t Fix(value_t* value) = 0;
};

#endif/*TK_DATA_VALIDATOR_HPP*/

