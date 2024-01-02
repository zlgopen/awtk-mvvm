/**
 * File:   view_model_jerryscript_object_wrapper.c
 * Author: AWTK Develop Team
 * Brief:  view model formed by wrapping jerry script object
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "mvvm/jerryscript/object_js_base.h"
#include "mvvm/jerryscript/object_js_factory.h"
#include "mvvm/jerryscript/view_model_jerryscript_object_wrapper.h"

static ret_t view_model_jerryscript_on_create(view_model_t* view_model, navigator_request_t* req) {
  ret_t ret;
  jsvalue_t jsobj;
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  jsobj = jsvalue_from_obj(vm->obj);

  if (jsvalue_is_object(jsobj)) {
    jsvalue_t jsargs = jsvalue_from_navigator_request(req);
    ret = jsobj_exec_ex(jsobj, "onCreate", jsargs);
    jsvalue_unref(jsargs);
  }

  jsvalue_unref(jsobj);

  return ret;
}

static ret_t view_model_jerryscript_on_destroy(view_model_t* view_model) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_exec(vm->obj, "onDestroy", NULL);
}

static ret_t view_model_jerryscript_on_will_mount(view_model_t* view_model,
                                                  navigator_request_t* req) {
  ret_t ret;
  value_t v;
  jsvalue_t jsobj;
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  jsobj = jsvalue_from_obj(vm->obj);

  if (jsvalue_is_object(jsobj)) {
    jsvalue_t jsargs = jsvalue_from_navigator_request(req);
    ret = jsobj_exec_ex(jsobj, "onWillMount", jsargs);
    jsvalue_unref(jsargs);
  }

  jsvalue_unref(jsobj);

  return ret;
}

static ret_t view_model_jerryscript_on_mount(view_model_t* view_model) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_exec(vm->obj, "onMount", NULL);
}

static ret_t view_model_jerryscript_on_will_unmount(view_model_t* view_model) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_exec(vm->obj, "onWillUnmount", NULL);
}

static ret_t view_model_jerryscript_on_unmount(view_model_t* view_model) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_exec(vm->obj, "onUnmount", NULL);
}

const static view_model_vtable_t s_view_model_jerryscript_vtable = {
    .on_will_mount = view_model_jerryscript_on_will_mount,
    .on_mount = view_model_jerryscript_on_mount,
    .on_will_unmount = view_model_jerryscript_on_will_unmount,
    .on_unmount = view_model_jerryscript_on_unmount};

static ret_t view_model_jerryscript_object_wrapper_on_destroy(tk_object_t* obj) {
  view_model_t* view_model = VIEW_MODEL(obj);
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  view_model_jerryscript_on_destroy(view_model);

  if (vm->obj != NULL) {
    jsvalue_t jsobj = jsvalue_from_obj(vm->obj);
    js_view_model_delete_native_ptr(jsobj);
    jsvalue_unref(jsobj);
  }

  TK_OBJECT_UNREF(vm->obj);

  return view_model_deinit(view_model);
}

static int32_t view_model_jerryscript_object_wrapper_compare(tk_object_t* obj, tk_object_t* other) {
  return tk_object_compare(obj, other);
}

static ret_t view_model_jerryscript_object_wrapper_set_prop(tk_object_t* obj, const char* name,
                                                            const value_t* v) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop(vm->obj, name, v);
}

static ret_t view_model_jerryscript_object_wrapper_get_prop(tk_object_t* obj, const char* name,
                                                            value_t* v) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_get_prop(vm->obj, name, v);
}

static ret_t view_model_jerryscript_object_wrapper_remove_prop(tk_object_t* obj, const char* name) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_remove_prop(vm->obj, name);
}

static ret_t view_model_jerryscript_object_wrapper_foreach_prop(tk_object_t* obj,
                                                                tk_visit_t on_prop, void* ctx) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_foreach_prop(vm->obj, on_prop, ctx);
}

static bool_t view_model_jerryscript_object_wrapper_can_exec(tk_object_t* obj, const char* name,
                                                             const char* args) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);
  return tk_object_can_exec(vm->obj, name, args);
}

static ret_t view_model_jerryscript_object_wrapper_exec(tk_object_t* obj, const char* name,
                                                        const char* args) {
  view_model_jerryscript_object_wrapper_t* vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  return tk_object_exec(vm->obj, name, args);
}

static JSFUNC_DECL(wrap_notify_props_changed) {
  view_model_t* view_model = VIEW_MODEL(js_view_model_get_native_ptr(call_info_p->this_value));

  return jsvalue_from_number(view_model_notify_props_changed(view_model));
}

static JSFUNC_DECL(wrap_notify_items_changed) {
  ret_t ret = RET_BAD_PARAMS;

  if (args_count >= 1) {
    view_model_t* view_model = VIEW_MODEL(js_view_model_get_native_ptr(call_info_p->this_value));

    if (view_model != NULL) {
      tk_object_t* target = jsvalue_to_obj(args_p[0]);
      ret = view_model_notify_items_changed(view_model, target);
    }
  }

  return jsvalue_from_number(ret);
}

static const object_vtable_t s_obj_view_model_jerryscript_object_wrapper_vtable = {
    .type = "view_model_jerryscript_object_wrapper",
    .desc = "view_model_jerryscript_object_wrapper",
    .size = sizeof(view_model_jerryscript_object_wrapper_t),
    .is_collection = FALSE,
    .on_destroy = view_model_jerryscript_object_wrapper_on_destroy,
    .compare = view_model_jerryscript_object_wrapper_compare,
    .get_prop = view_model_jerryscript_object_wrapper_get_prop,
    .set_prop = view_model_jerryscript_object_wrapper_set_prop,
    .remove_prop = view_model_jerryscript_object_wrapper_remove_prop,
    .foreach_prop = view_model_jerryscript_object_wrapper_foreach_prop,
    .can_exec = view_model_jerryscript_object_wrapper_can_exec,
    .exec = view_model_jerryscript_object_wrapper_exec};

view_model_t* view_model_jerryscript_object_wrapper_create(jsvalue_t jsobj,
                                                           navigator_request_t* req) {
  view_model_t* view_model;
  view_model_jerryscript_object_wrapper_t* vm;
  object_js_factory_t* factory = object_js_factory();
  tk_object_t* obj = object_js_factory_create_object(factory, jsvalue_ref(jsobj), TRUE);
  return_value_if_fail(obj != NULL, NULL);

  view_model = VIEW_MODEL(tk_object_create(&s_obj_view_model_jerryscript_object_wrapper_vtable));
  vm = VIEW_MODEL_JERRYSCRIPT_OBJECT_WRAPPER(view_model);
  return_value_if_fail(vm != NULL, NULL);

  view_model_init(view_model);
  view_model->vt = &s_view_model_jerryscript_vtable;
  vm->obj = obj;

  view_model_jerryscript_on_create(view_model, req);

  if (!object_js_base_is_listener_registered(obj, TK_OBJECT(view_model))) {
    object_js_base_register_listener(obj, TK_OBJECT(view_model));
  }

  jsobj_set_prop_func(jsobj, "notifyPropsChanged", wrap_notify_props_changed);
  jsobj_set_prop_func(jsobj, "notifyItemsChanged", wrap_notify_items_changed);

  js_view_model_set_native_ptr(jsobj, view_model);

  return view_model;
}

bool_t view_model_is_jerryscript_object_wrapper(view_model_t* view_model) {
  tk_object_t* obj = TK_OBJECT(view_model);
  return obj != NULL && obj->vt == &s_obj_view_model_jerryscript_object_wrapper_vtable;
}
