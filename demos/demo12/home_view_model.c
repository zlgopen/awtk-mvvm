﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "home_view_model.h"

static ret_t home_view_model_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  home_t* ahome = ((home_view_model_t*)(obj))->ahome;

  if (tk_str_ieq("bed_room_info", name)) {
    return RET_OK;
  } else if (tk_str_ieq("living_room_info", name)) {
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t home_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  home_t* ahome = ((home_view_model_t*)(obj))->ahome;

  if (tk_str_ieq("bed_room_info", name)) {
    value_set_str(v, home_get_bed_room_info(ahome));
    return RET_OK;
  } else if (tk_str_ieq("living_room_info", name)) {
    value_set_str(v, home_get_living_room_info(ahome));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t home_view_model_can_exec(tk_object_t* obj, const char* name, const char* args) {
  home_view_model_t* vm = (home_view_model_t*)(obj);
  home_t* ahome = vm->ahome;
  if (tk_str_ieq("adjustBedRoom", name)) {
    return TRUE;

  } else if (tk_str_ieq("adjustLivingRoom", name)) {
    return TRUE;
  }
  return FALSE;
}

static ret_t home_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
  home_view_model_t* vm = (home_view_model_t*)(obj);
  home_t* ahome = vm->ahome;
  if (tk_str_ieq("adjustBedRoom", name)) {
    return home_adjust_bed_room(ahome);

  } else if (tk_str_ieq("adjustLivingRoom", name)) {
    return home_adjust_living_room(ahome);
  }
  return RET_NOT_FOUND;
}

static ret_t home_view_model_on_destroy(tk_object_t* obj) {
  home_view_model_t* vm = (home_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  emitter_off_by_ctx(EMITTER(vm->ahome), vm);
  home_destroy(vm->ahome);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_home_view_model_vtable = {"home_view_model_t",
                                                         "home_view_model_t",
                                                         sizeof(home_view_model_t),
                                                         FALSE,
                                                         home_view_model_on_destroy,
                                                         NULL,
                                                         home_view_model_get_prop,
                                                         home_view_model_set_prop,
                                                         NULL,
                                                         NULL,
                                                         home_view_model_can_exec,
                                                         home_view_model_exec};

view_model_t* home_view_model_create_with(home_t* ahome) {
  tk_object_t* obj = tk_object_create(&s_home_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  home_view_model_t* home_view_model = (home_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  home_view_model->ahome = ahome;

  emitter_on(EMITTER(ahome), EVT_PROPS_CHANGED, emitter_forward, vm);

  return vm;
}

ret_t home_view_model_attach(view_model_t* vm, home_t* ahome) {
  home_view_model_t* home_view_model = (home_view_model_t*)(vm);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  home_view_model->ahome = ahome;

  return RET_OK;
}

view_model_t* home_view_model_create(navigator_request_t* req) {
  home_t* ahome = home_create();
  return_value_if_fail(ahome != NULL, NULL);

  return home_view_model_create_with(ahome);
}
