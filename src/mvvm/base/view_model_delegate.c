/**
 * File:   view_model_delegate.c
 * Author: AWTK Develop Team
 * Brief:  view_model_delegate
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
 * 2019-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/view_model_delegate.h"

typedef struct _command_desc_t {
  view_model_exec_t exec;
  view_model_can_exec_t can_exec;
} command_desc_t;

static command_desc_t* command_desc_init(command_desc_t* desc, view_model_exec_t exec,
                                         view_model_can_exec_t can_exec) {
  return_value_if_fail(desc != NULL, NULL);

  desc->exec = exec;
  desc->can_exec = can_exec;

  return desc;
}

static command_desc_t* command_desc_create(view_model_exec_t exec, view_model_can_exec_t can_exec) {
  command_desc_t* desc = TKMEM_ZALLOC(command_desc_t);

  return command_desc_init(desc, exec, can_exec);
}

typedef struct _prop_desc_t {
  void* addr;
  value_type_t type;

  view_model_get_prop_t get;
  view_model_set_prop_t set;
} prop_desc_t;

static prop_desc_t* prop_desc_init(prop_desc_t* desc, value_type_t type, void* addr,
                                   view_model_get_prop_t get, view_model_set_prop_t set) {
  return_value_if_fail(desc != NULL, NULL);

  desc->type = type;
  desc->addr = addr;
  desc->get = get;
  desc->set = set;

  return desc;
}

static prop_desc_t* prop_desc_create(value_type_t type, void* addr, view_model_get_prop_t get,
                                     view_model_set_prop_t set) {
  prop_desc_t* desc = TKMEM_ZALLOC(prop_desc_t);

  return prop_desc_init(desc, type, addr, get, set);
}

static ret_t prop_desc_set_value(view_model_delegate_t* view_model, prop_desc_t* desc,
                                 const value_t* v) {
  ret_t ret = RET_OK;
  void* obj = view_model->obj;
  if (desc->set != NULL) {
    ret = desc->set(obj, v);
  } else if (desc->addr != NULL) {
    switch (desc->type) {
      case VALUE_TYPE_INT8: {
        int8_t i8 = (int8_t)value_int(v);
        memcpy(desc->addr, &(i8), sizeof(i8));
        break;
      }
      case VALUE_TYPE_UINT8: {
        int8_t u8 = (int8_t)value_int(v);
        memcpy(desc->addr, &(u8), sizeof(u8));
        break;
      }
      case VALUE_TYPE_INT16: {
        int16_t i16 = (int16_t)value_int(v);
        memcpy(desc->addr, &(i16), sizeof(i16));
        break;
      }
      case VALUE_TYPE_UINT16: {
        uint16_t u16 = (uint16_t)value_int(v);
        memcpy(desc->addr, &(u16), sizeof(u16));
        break;
      }
      case VALUE_TYPE_INT32: {
        int32_t i32 = (int32_t)value_int(v);
        memcpy(desc->addr, &(i32), sizeof(i32));
        break;
      }
      case VALUE_TYPE_UINT32: {
        uint32_t u32 = (uint32_t)value_int(v);
        memcpy(desc->addr, &(u32), sizeof(u32));
        break;
      }
      case VALUE_TYPE_INT64: {
        int64_t i64 = (int64_t)value_int(v);
        memcpy(desc->addr, &(i64), sizeof(i64));
        break;
      }
      case VALUE_TYPE_UINT64: {
        uint64_t u64 = (uint64_t)value_int(v);
        memcpy(desc->addr, &(u64), sizeof(u64));
        break;
      }
      case VALUE_TYPE_FLOAT: {
        float_t f = (float_t)value_float(v);
        memcpy(desc->addr, &(f), sizeof(f));
        break;
      }
      case VALUE_TYPE_FLOAT32: {
        float f = (float)value_float(v);
        memcpy(desc->addr, &(f), sizeof(f));
        break;
      }
      case VALUE_TYPE_DOUBLE: {
        double f = (double)value_float(v);
        memcpy(desc->addr, &(f), sizeof(f));
        break;
      }
      case VALUE_TYPE_BOOL: {
        bool_t b = (bool_t)value_bool(v);
        memcpy(desc->addr, &(b), sizeof(b));
        break;
      }
      case VALUE_TYPE_STRING: {
        char** str = (char**)desc->addr;
        if (str_from_value(&(view_model->temp), v) == RET_OK) {
          *str = tk_str_copy(*str, view_model->temp.str);
        } else {
          ret = RET_FAIL;
        }
        break;
      }
      default: {
        ret = RET_FAIL;
        assert(!"not supproted");
      }
    }
  } else {
    ret = RET_FAIL;
  }

  return ret;
}

static ret_t prop_desc_get_value(void* obj, prop_desc_t* desc, value_t* v) {
  ret_t ret = RET_OK;
  if (desc->get != NULL) {
    ret = desc->get(obj, v);
  } else if (desc->addr != NULL) {
    switch (desc->type) {
      case VALUE_TYPE_INT8: {
        value_set_int8(v, *(int8_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_UINT8: {
        value_set_uint8(v, *(uint8_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_INT16: {
        value_set_int16(v, *(int16_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_UINT16: {
        value_set_uint16(v, *(uint16_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_INT32: {
        value_set_int32(v, *(int32_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_UINT32: {
        value_set_uint32(v, *(uint32_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_INT64: {
        value_set_int64(v, *(int64_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_UINT64: {
        value_set_uint64(v, *(uint64_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_FLOAT: {
        value_set_float(v, *(float_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_FLOAT32: {
        value_set_float32(v, *(float*)(desc->addr));
        break;
      }
      case VALUE_TYPE_DOUBLE: {
        value_set_double(v, *(double*)(desc->addr));
        break;
      }
      case VALUE_TYPE_BOOL: {
        value_set_bool(v, *(bool_t*)(desc->addr));
        break;
      }
      case VALUE_TYPE_STRING: {
        char** str = (char**)desc->addr;
        value_set_str(v, *str);
        break;
      }
      default: {
        ret = RET_FAIL;
        assert(!"not supproted");
      }
    }
  } else {
    ret = RET_FAIL;
  }

  return ret;
}

static ret_t tk_visit_free(void* ctx, const void* data) {
  named_value_t* iter = (named_value_t*)data;

  TKMEM_FREE(iter->value.value.ptr);
  iter->value.value.ptr = NULL;

  return RET_OK;
}

static ret_t view_model_delegate_on_destroy(object_t* obj) {
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);

  if (view_model->obj_destroy != NULL) {
    view_model->obj_destroy(view_model->obj);
  }

  str_reset(&(view_model->temp));
  object_foreach_prop(view_model->props, tk_visit_free, NULL);
  object_foreach_prop(view_model->commands, tk_visit_free, NULL);
  object_unref(view_model->props);
  object_unref(view_model->commands);

  return RET_OK;
}

static int32_t view_model_delegate_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_delegate_remove_prop(object_t* obj, const char* name) {
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);

  return object_remove_prop(view_model->props, name);
}

static prop_desc_t* view_model_delegate_get_prop_desc(object_t* obj, const char* name) {
  value_t v;
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);
  if (object_get_prop(view_model->props, name, &v) == RET_OK) {
    return (prop_desc_t*)value_pointer(&v);
  } else {
    return NULL;
  }
}

static ret_t view_model_delegate_set_prop(object_t* obj, const char* name, const value_t* v) {
  prop_desc_t* prop_desc = view_model_delegate_get_prop_desc(obj, name);
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);
  return_value_if_fail(prop_desc != NULL, RET_NOT_FOUND);

  return prop_desc_set_value(view_model, prop_desc, v);
}

static ret_t view_model_delegate_get_prop(object_t* obj, const char* name, value_t* v) {
  prop_desc_t* prop_desc = view_model_delegate_get_prop_desc(obj, name);
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);
  return_value_if_fail(prop_desc != NULL, RET_NOT_FOUND);

  return prop_desc_get_value(view_model->obj, prop_desc, v);
}

static command_desc_t* view_model_delegate_get_command_desc(object_t* obj, const char* name) {
  value_t v;
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);
  if (object_get_prop(view_model->commands, name, &v) == RET_OK) {
    return (command_desc_t*)value_pointer(&v);
  } else {
    return NULL;
  }
}

static bool_t view_model_delegate_can_exec(object_t* obj, const char* name, const char* args) {
  command_desc_t* command_desc = view_model_delegate_get_command_desc(obj, name);
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);
  return_value_if_fail(command_desc != NULL, FALSE);

  return command_desc->can_exec != NULL ? command_desc->can_exec(view_model->obj, args) : TRUE;
}

static ret_t view_model_delegate_exec(object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_OK;
  command_desc_t* command_desc = view_model_delegate_get_command_desc(obj, name);
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(obj);
  return_value_if_fail(command_desc != NULL, RET_NOT_FOUND);

  ret = command_desc->exec(view_model->obj, args);
  if (ret == RET_OBJECT_CHANGED) {
    object_notify_changed(OBJECT(obj));
  }

  return ret;
}

static const object_vtable_t s_model_delegate_vtable = {
    .type = "view_model_delegate",
    .desc = "view_model_delegate",
    .size = sizeof(view_model_delegate_t),
    .is_collection = FALSE,
    .on_destroy = view_model_delegate_on_destroy,

    .exec = view_model_delegate_exec,
    .can_exec = view_model_delegate_can_exec,
    .compare = view_model_delegate_compare,
    .get_prop = view_model_delegate_get_prop,
    .set_prop = view_model_delegate_set_prop,
    .remove_prop = view_model_delegate_remove_prop,
};

view_model_t* view_model_delegate_create(void* obj, tk_destroy_t obj_destroy) {
  object_t* o = object_create(&s_model_delegate_vtable);
  view_model_delegate_t* view_model = VIEW_MODEL_DELEGATE(view_model_init(VIEW_MODEL(o)));
  return_value_if_fail(view_model != NULL, NULL);

  view_model->obj = obj;
  view_model->obj_destroy = obj_destroy;
  view_model->props = object_default_create();
  view_model->commands = object_default_create();
  str_init(&(view_model->temp), 0);

  return VIEW_MODEL(o);
}

ret_t view_model_delegate_install_command(view_model_t* view_model, const char* name,
                                          view_model_exec_t exec, view_model_can_exec_t can_exec) {
  value_t v;
  command_desc_t* desc = NULL;
  view_model_delegate_t* m = VIEW_MODEL_DELEGATE(view_model);
  return_value_if_fail(view_model != NULL && name != NULL && exec != NULL, RET_BAD_PARAMS);

  if (desc != NULL) {
    command_desc_init(desc, exec, can_exec);

    return RET_OK;
  } else {
    desc = command_desc_create(exec, can_exec);
    return_value_if_fail(desc != NULL, RET_OOM);

    value_set_pointer(&v, desc);
    return object_set_prop(m->commands, name, &v);
  }
}

ret_t view_model_delegate_install_prop(view_model_t* view_model, const char* name,
                                       value_type_t type, void* addr, view_model_get_prop_t get,
                                       view_model_set_prop_t set) {
  value_t v;
  prop_desc_t* desc = view_model_delegate_get_prop_desc(OBJECT(view_model), name);
  view_model_delegate_t* m = VIEW_MODEL_DELEGATE(view_model);
  return_value_if_fail(view_model != NULL && name != NULL, RET_BAD_PARAMS);

  if (desc != NULL) {
    prop_desc_init(desc, type, addr, get, set);

    return RET_OK;
  } else {
    desc = prop_desc_create(type, addr, get, set);
    return_value_if_fail(desc != NULL, RET_OOM);

    value_set_pointer(&v, desc);
    return object_set_prop(m->props, name, &v);
  }
}
