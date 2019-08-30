
/**
 * File:  room_settings.hpp
 * Author: AWTK Develop Team
 * Brief:  room_settings view model
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

#ifndef TK_ROOM_SETTINGS_HPP
#define TK_ROOM_SETTINGS_HPP

#include "mvvm/cpp/adapter.hpp"

class RoomSettings : public vm::ViewModel {
  public:
    RoomSettings(navigator_request_t* request);
    virtual ~RoomSettings();
  
  public:
    virtual ret_t Exec(const char* name, const char* args);
    virtual bool_t CanExec(const char* name, const char* args) const;
    virtual ret_t GetProp(const char* name, value_t* v) const;
    virtual ret_t SetProp(const char* name, const value_t* v);

  private:    
    /*房间名称*/
    char* room;
    /*温度*/
    double temp;
    /*湿度*/
    double humidity;
};

view_model_t* room_settings_view_model_create(navigator_request_t* req);

#endif/*TK_ROOM_SETTINGS_HPP*/

