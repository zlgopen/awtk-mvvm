
/**
 * File:  shape.hpp
 * Author: AWTK Develop Team
 * Brief:  shape view model
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

#ifndef TK_SHAPE_HPP
#define TK_SHAPE_HPP

#include "tkc/str.h"
#include "mvvm/cpp/adapter.hpp"

class Shape : public vm::ViewModel {
  public:
    Shape(navigator_request_t* request);
    virtual ~Shape();
  
  public:
    virtual ret_t Exec(const char* name, const char* args);
    virtual bool_t CanExec(const char* name, const char* args) const;
    virtual ret_t GetProp(const char* name, value_t* v) const;
    virtual ret_t SetProp(const char* name, const value_t* v);

  private:    
    int32_t type;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
    int32_t opacity;
    int32_t text_align;
    str_t name;
    mutable str_t overview;
};

view_model_t* shape_view_model_create(navigator_request_t* req);

#endif/*TK_SHAPE_HPP*/

