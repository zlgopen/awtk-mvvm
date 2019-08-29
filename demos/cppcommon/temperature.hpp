
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

#ifndef TK_TEMPERATURE_HPP
#define TK_TEMPERATURE_HPP

#include "mvvm/base/view_model.h"
#include "mvvm/cpp/view_model.hpp"

class Temperature : public ViewModel {
  public:
    Temperature(navigator_request_t* request);
    virtual ~Temperature();
  
  public:
    virtual ret_t Exec(const char* name, const char* args);
    virtual bool_t CanExec(const char* name, const char* args) const;
    virtual ret_t GetProp(const char* name, value_t* v) const;
    virtual ret_t SetProp(const char* name, const value_t* v);

  private:    
    double value;
    double old_value;
};

view_model_t* temperature_view_model_create(navigator_request_t* req);

#endif/*TK_TEMPERATURE_HPP*/

