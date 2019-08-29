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

#ifndef TK_DATA_CONVERTER_HPP
#define TK_DATA_CONVERTER_HPP

#include "mvvm/base/value_converter.h"

class ValueConverter {
  public:
    ValueConverter() {}
    virtual ~ValueConverter() {}
  
  public:
    virtual ret_t ToModel(const value_t* from, value_t* to) = 0;
    virtual ret_t ToView(const value_t* from, value_t* to) = 0;
};

#endif/*TK_DATA_CONVERTER_HPP*/

