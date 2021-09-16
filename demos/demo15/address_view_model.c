﻿
/*This file is generated by code generator*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "address_view_model.h"

static ret_t address_view_model_get_provinces(object_t* obj, value_t* v) {
  value_set_str(v, "Beijing;Shanghai;Guangdong");
  return RET_OK;
}

static ret_t address_view_model_get_cities(object_t* obj, value_t* v) {
  address_view_model_t* vm = (address_view_model_t*)(obj);

  if (tk_str_eq(vm->province, "Beijing")) {
    value_set_str(v, "Beijing");
    return RET_OK;
  } else if (tk_str_eq(vm->province, "Shanghai")) {
    value_set_str(v, "Shanghai");
    return RET_OK;
  } else if (tk_str_eq(vm->province, "Guangdong")) {
    value_set_str(v, "Guangzhou;Shenzhen");
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t address_view_model_get_contories(object_t* obj, value_t* v) {
  address_view_model_t* vm = (address_view_model_t*)(obj);

  if (tk_str_eq(vm->city, "Beijing")) {
    value_set_str(v, "Dongcheng;Xicheng;Chaoyang;Fengtai;Shijingshan;Haidian");
    return RET_OK;
  } else if (tk_str_eq(vm->city, "Shanghai")) {
    value_set_str(v, "Xuhui;Changning;Jingan;Putuo;Hongkou;Yangpu");
    return RET_OK;
  } else if (tk_str_eq(vm->city, "Guangzhou")) {
    value_set_str(v, "Tianhe;Huangpu;Liwan;Yuexiu;Haizhu");
    return RET_OK;
  } else if (tk_str_eq(vm->city, "Shenzhen")) {
    value_set_str(v, "Luohu;Futian;Nanshan;Baoan;Longgang");
    return RET_OK;
  }
  return RET_NOT_FOUND;
}

static ret_t address_view_model_get_address(object_t* obj, value_t* v) {
  address_view_model_t* vm = (address_view_model_t*)(obj);
  str_t* temp = &(vm->temp);

  str_set(temp, vm->province);
  str_append(temp, " ");
  str_append(temp, vm->city);
  str_append(temp, " ");
  str_append(temp, vm->country);
  str_append(temp, " ");
  str_append(temp, vm->detail);

  value_set_str(v, temp->str);

  return RET_NOT_FOUND;
}

static ret_t address_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  address_view_model_t* vm = (address_view_model_t*)(obj);

  if (tk_str_ieq("province", name)) {
    vm->province = tk_str_copy(vm->province, value_str(v));

    if (tk_str_eq(vm->province, "Beijing")) {
      vm->city = tk_str_copy(vm->city, "Beijing");
    } else if (tk_str_eq(vm->province, "Shanghai")) {
      vm->city = tk_str_copy(vm->city, "Shanghai");
    } else if (tk_str_eq(vm->province, "Guangdong")) {
      vm->city = tk_str_copy(vm->city, "Guangzhou");
    }
    return RET_OK;
  } else if (tk_str_ieq("city", name)) {
    vm->city = tk_str_copy(vm->city, value_str(v));

    if (tk_str_eq(vm->city, "Beijing")) {
      vm->country = tk_str_copy(vm->country, "Dongcheng");
    } else if (tk_str_eq(vm->city, "Shanghai")) {
      vm->country = tk_str_copy(vm->country, "Xuhui");
    } else if (tk_str_eq(vm->city, "Guangzhou")) {
      vm->country = tk_str_copy(vm->country, "Tianhe");
    } else if (tk_str_eq(vm->city, "Shenzhen")) {
      vm->country = tk_str_copy(vm->country, "Luohu");
    }
    return RET_OK;
  } else if (tk_str_ieq("country", name)) {
    vm->country = tk_str_copy(vm->country, value_str(v));
    return RET_OK;
  } else if (tk_str_ieq("detail", name)) {
    vm->detail = tk_str_copy(vm->detail, value_str(v));
    return RET_OK;
  } else if (tk_str_ieq("province_list", name)) {
    vm->province_list = tk_str_copy(vm->province_list, value_str(v));
    return RET_OK;
  } else if (tk_str_ieq("city_list", name)) {
    vm->city_list = tk_str_copy(vm->city_list, value_str(v));
    return RET_OK;
  } else if (tk_str_ieq("country_list", name)) {
    vm->country_list = tk_str_copy(vm->country_list, value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t address_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  address_view_model_t* vm = (address_view_model_t*)(obj);

  if (tk_str_ieq("province", name)) {
    value_set_str(v, vm->province);
    return RET_OK;
  } else if (tk_str_ieq("city", name)) {
    value_set_str(v, vm->city);
    return RET_OK;
  } else if (tk_str_ieq("country", name)) {
    value_set_str(v, vm->country);
    return RET_OK;
  } else if (tk_str_ieq("detail", name)) {
    value_set_str(v, vm->detail);
    return RET_OK;
  } else if (tk_str_ieq("province_list", name)) {
    address_view_model_get_provinces(obj, v);
    return RET_OK;
  } else if (tk_str_ieq("city_list", name)) {
    address_view_model_get_cities(obj, v);
    return RET_OK;
  } else if (tk_str_ieq("country_list", name)) {
    address_view_model_get_contories(obj, v);
    return RET_OK;
  } else if (tk_str_ieq("address", name)) {
    address_view_model_get_address(obj, v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static bool_t address_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  return FALSE;
}

static ret_t address_view_model_exec(object_t* obj, const char* name, const char* args) {
  return RET_NOT_FOUND;
}

static ret_t address_view_model_on_destroy(object_t* obj) {
  address_view_model_t* vm = (address_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(vm->province_list);
  TKMEM_FREE(vm->city_list);
  TKMEM_FREE(vm->country_list);
  TKMEM_FREE(vm->province);
  TKMEM_FREE(vm->city);
  TKMEM_FREE(vm->country);
  TKMEM_FREE(vm->detail);
  str_reset(&(vm->temp));

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_address_view_model_vtable = {"address_view_model_t",
                                                            "address_view_model_t",
                                                            sizeof(address_view_model_t),
                                                            FALSE,
                                                            address_view_model_on_destroy,
                                                            NULL,
                                                            address_view_model_get_prop,
                                                            address_view_model_set_prop,
                                                            NULL,
                                                            NULL,
                                                            address_view_model_can_exec,
                                                            address_view_model_exec};

view_model_t* address_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_address_view_model_vtable);
  view_model_t* view_model = view_model_init(VIEW_MODEL(obj));
  address_view_model_t* vm = (address_view_model_t*)(view_model);
  return_value_if_fail(vm != NULL, NULL);

  str_init(&(vm->temp), 0);
  vm->province = tk_str_copy(vm->province, "Beijing");
  vm->city = tk_str_copy(vm->city, "Beijing");
  vm->country = tk_str_copy(vm->country, "Dongcheng");
  vm->detail = tk_str_copy(vm->detail, "");

  return view_model;
}