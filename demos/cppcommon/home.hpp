
/**
 * File:  home.hpp
 * Author: AWTK Develop Team
 * Brief:  home view model
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
 * 2019-09-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_HOME_HPP
#define TK_HOME_HPP

#include "mvvm/cpp/adapter.hpp"

typedef struct _RoomInfo {
  double temp;
  double humidity;
} RoomInfo;

class Home : public vm::ViewModel {
  public:
    Home(navigator_request_t* request);
    virtual ~Home();
  
  public:
    virtual ret_t Exec(const char* name, const char* args);
    virtual bool_t CanExec(const char* name, const char* args) const;
    virtual ret_t GetProp(const char* name, value_t* v) const;
    virtual ret_t SetProp(const char* name, const value_t* v);

  public:
    static ret_t OnResult(navigator_request_t* req, const value_t* result);
  private:
    ret_t AdjustRoomSettings(const char* room_name, RoomInfo* info);

  private:    
    RoomInfo bed_room;
    RoomInfo living_room;
};

view_model_t* home_view_model_create(navigator_request_t* req);

#endif/*TK_HOME_HPP*/

