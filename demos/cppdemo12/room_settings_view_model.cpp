﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "room_settings_view_model.h"

static ret_t room_settings_view_model_set_prop(tk_object_t* obj, const char* name,
                                               const value_t* v) {
  RoomSettings* aRoomSettings = ((room_settings_view_model_t*)(obj))->aRoomSettings;

  if (tk_str_ieq("temp", name)) {
    aRoomSettings->temp = value_double(v);

    return RET_OK;
  } else if (tk_str_ieq("humidity", name)) {
    aRoomSettings->humidity = value_double(v);

    return RET_OK;
  } else if (tk_str_ieq("room", name)) {
    aRoomSettings->room = value_str(v);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t room_settings_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  RoomSettings* aRoomSettings = ((room_settings_view_model_t*)(obj))->aRoomSettings;

  if (tk_str_ieq("temp", name)) {
    value_set_double(v, aRoomSettings->temp);
    return RET_OK;
  } else if (tk_str_ieq("humidity", name)) {
    value_set_double(v, aRoomSettings->humidity);
    return RET_OK;
  } else if (tk_str_ieq("room", name)) {
    value_set_str(v, aRoomSettings->room.c_str());
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t room_settings_view_model_can_exec(tk_object_t* obj, const char* name,
                                                const char* args) {
  room_settings_view_model_t* vm = (room_settings_view_model_t*)(obj);
  if (tk_str_ieq("Return", name)) {
    return TRUE;
  }
  return FALSE;
}

static ret_t room_settings_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
  room_settings_view_model_t* vm = (room_settings_view_model_t*)(obj);
  RoomSettings* aRoomSettings = vm->aRoomSettings;
  if (tk_str_ieq("Return", name)) {
    return aRoomSettings->Return();
  }
  return RET_NOT_FOUND;
}

static ret_t room_settings_view_model_on_destroy(tk_object_t* obj) {
  room_settings_view_model_t* vm = (room_settings_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  delete (vm->aRoomSettings);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const struct room_settings_view_model_vtable_t {
  object_vtable_t base;
  room_settings_view_model_vtable_t() {
    base.type = "room_settings_view_model_t";
    base.desc = "room_settings_view_model_t";
    base.size = sizeof(room_settings_view_model_t);
    base.is_collection = FALSE;
    base.on_destroy = room_settings_view_model_on_destroy;
    base.get_prop = room_settings_view_model_get_prop;
    base.set_prop = room_settings_view_model_set_prop;
    base.can_exec = room_settings_view_model_can_exec;
    base.exec = room_settings_view_model_exec;
  }
} s_room_settings_view_model_vtable;

view_model_t* room_settings_view_model_create_with(RoomSettings* aRoomSettings) {
  tk_object_t* obj = tk_object_create(&s_room_settings_view_model_vtable.base);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  room_settings_view_model_t* room_settings_view_model = (room_settings_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  room_settings_view_model->aRoomSettings = aRoomSettings;

  return vm;
}

ret_t room_settings_view_model_attach(view_model_t* vm, RoomSettings* aRoomSettings) {
  room_settings_view_model_t* room_settings_view_model = (room_settings_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  room_settings_view_model->aRoomSettings = aRoomSettings;

  return RET_OK;
}

view_model_t* room_settings_view_model_create(navigator_request_t* req) {
  RoomSettings* aRoomSettings = new RoomSettings(req);
  return_value_if_fail(aRoomSettings != NULL, NULL);

  return room_settings_view_model_create_with(aRoomSettings);
}
