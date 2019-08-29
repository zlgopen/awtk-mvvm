
/**
 * File:  calculator.hpp
 * Author: AWTK Develop Team
 * Brief: calculator view model 
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
 * 2019-08-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CALCULATOR_HPP
#define TK_CALCULATOR_HPP

#include "tkc/str.h"
#include "mvvm/cpp/adapter.hpp"

class Calculator : public ViewModel {
  public:
    Calculator(navigator_request_t* request);
    virtual ~Calculator();
  
  public:
    virtual ret_t Exec(const char* name, const char* args);
    virtual bool_t CanExec(const char* name, const char* args) const;
    virtual ret_t GetProp(const char* name, value_t* v) const;
    virtual ret_t SetProp(const char* name, const value_t* v);

  private:    
    str_t expr;
};

view_model_t* calculator_view_model_create(navigator_request_t* req);

#endif/*TK_CALCULATOR_HPP*/

