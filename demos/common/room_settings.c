/**
 * File:   room_settings.c
 * Author: AWTK Develop Team
 * Brief:  room_settings
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
 * 2019-02-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"

#include "room_settings.h"

#define CMD_RETURN "return"

static ret_t room_settings_set_prop(object_t* obj, const char* name, const value_t* v) {
  room_settings_t* room_settings = ROOM_SETTINGS(obj);

  if (tk_str_eq(PROP_TEMP, name)) {
    room_settings->temp = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(PROP_HUMIDITY, name)) {
    room_settings->humidity = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(PROP_ROOM, name)) {
    room_settings->room = tk_str_copy(room_settings->room, value_str(v));
    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t room_settings_get_prop(object_t* obj, const char* name, value_t* v) {
  room_settings_t* room_settings = ROOM_SETTINGS(obj);

  if (tk_str_eq(PROP_TEMP, name)) {
    value_set_double(v, room_settings->temp);

    return RET_OK;
  } else if (tk_str_eq(PROP_HUMIDITY, name)) {
    value_set_double(v, room_settings->humidity);

    return RET_OK;
  } else if (tk_str_eq(PROP_ROOM, name)) {
    value_set_str(v, room_settings->room);

    return RET_OK;
  } else {
    return RET_NOT_FOUND;
  }
}

static bool_t room_settings_can_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, CMD_RETURN)) {
    return TRUE;
  }

  return FALSE;
}

static ret_t room_settings_return_result(room_settings_t* room_settings) {
  value_t v;

  value_set_object(&v, OBJECT(room_settings->req));
  object_copy_prop(OBJECT(room_settings->req), OBJECT(room_settings), PROP_TEMP);
  object_copy_prop(OBJECT(room_settings->req), OBJECT(room_settings), PROP_HUMIDITY);

  return navigator_request_on_result(room_settings->req, &v);
}

static ret_t room_settings_exec(object_t* obj, const char* name, const char* args) {
  room_settings_t* room_settings = ROOM_SETTINGS(obj);

  if (tk_str_eq(name, CMD_RETURN)) {
    return room_settings_return_result(room_settings);
  }

  return RET_NOT_FOUND;
}

static ret_t room_settings_on_destroy(object_t* obj) {
  room_settings_t* room_settings = ROOM_SETTINGS(obj);

  TKMEM_FREE(room_settings->room);
  view_model_deinit(VIEW_MODEL(obj));

  return RET_OK;
}

static const object_vtable_t s_room_settings_vtable = {.type = "room_settings",
                                                       .desc = "room_settings",
                                                       .size = sizeof(room_settings_t),
                                                       .exec = room_settings_exec,
                                                       .can_exec = room_settings_can_exec,
                                                       .get_prop = room_settings_get_prop,
                                                       .set_prop = room_settings_set_prop,
                                                       .on_destroy = room_settings_on_destroy};

view_model_t* room_settings_create(navigator_request_t* req) {
  view_model_t* view_model = view_model_init(VIEW_MODEL(object_create(&s_room_settings_vtable)));
  room_settings_t* room_settings = ROOM_SETTINGS(view_model);
  return_value_if_fail(room_settings != NULL, NULL);

  room_settings->req = req;
  object_copy_prop(OBJECT(view_model), OBJECT(req), PROP_ROOM);
  object_copy_prop(OBJECT(view_model), OBJECT(req), PROP_TEMP);
  object_copy_prop(OBJECT(view_model), OBJECT(req), PROP_HUMIDITY);

  return view_model;
}
