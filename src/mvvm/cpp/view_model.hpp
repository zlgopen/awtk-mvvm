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

#include "tkc/emitter.h"
#include "mvvm/base/navigator_request.h"

class ViewModel {
  public:
    ViewModel(navigator_request_t* request);
    virtual ~ViewModel();
  
  public:
    virtual ret_t Exec(const char* name, const char* args);
    virtual bool_t CanExec(const char* name, const char* args);
    virtual ret_t GetProp(const char* name, value_t* v);
    virtual ret_t SetProp(const char* name, const value_t* v);

  public:    
    ret_t Off(uint32_t id);
    ret_t OffByCtx(void* ctx);
    ret_t DispatchEvent(event_t* event);
    uint32_t On(uint32_t event, event_func_t on_event, void* ctx);

  public:
    navigator_request_t* request;

  private:
    emitter_t emitter;
};

#endif/*TK_VIEW_MODEL_HPP*/

