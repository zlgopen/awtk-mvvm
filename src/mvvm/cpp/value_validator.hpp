/**
 * File:  iview_model.hpp
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

#ifndef TK_DATA_VALIDATOR_HPP
#define TK_DATA_VALIDATOR_HPP

#include "mvvm/base/value_validator.h"

class ValueValidator {
  public:
    ValueValidator() {}
    virtual ~ValueValidator() {}
  
  public:
    virtual bool_t IsValid(const value_t* value, str_t* msg) = 0;
    virtual ret_t Fix(value_t* value) = 0;
};

#endif/*TK_DATA_VALIDATOR_HPP*/

