#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/navigator.h"
#include "custom_handlers.h"

/***************custom_handlers***************/;

custom_handlers_t* custom_handlers_create(void) {
  custom_handlers_t* custom_handlers = TKMEM_ZALLOC(custom_handlers_t);
  return_value_if_fail(custom_handlers != NULL, NULL);

  return custom_handlers;
}

static ret_t custom_handlers_destroy(custom_handlers_t* custom_handlers) {
  return_value_if_fail(custom_handlers != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(custom_handlers);

  return RET_OK;
}

static ret_t custom_handlers_show_toast(custom_handlers_t* custom_handlers, const char* args) {
  navigator_toast("it is awtk toast!", 3000);

  return RET_OBJECT_CHANGED;
}

static ret_t custom_handlers_show_info(custom_handlers_t* custom_handlers, const char* args) {
  navigator_info("Info", "Download done!");

  return RET_OBJECT_CHANGED;
}

static ret_t custom_handlers_show_confirm(custom_handlers_t* custom_handlers, const char* args) {
  navigator_confirm("Confirm", "Are you sure to quit!");

  return RET_OBJECT_CHANGED;
}

static ret_t custom_handlers_choose(custom_handlers_t* custom_handlers, const char* args) {
  str_t str;
  str_init(&str, 0);
  str_set(&str, "./");

  if (tk_str_eq(args, "file_for_save")) {
    navigator_pick_file("Choose...", ".txt", TRUE, &str);
  } else if (tk_str_eq(args, "file_for_open")) {
    navigator_pick_file("Choose...", ".txt", FALSE, &str);
  } else {
    navigator_pick_dir("Choose...", &str);
  }

  log_debug("Choosed: %s\n", str.str);
  str_reset(&str);

  return RET_OBJECT_CHANGED;
}

static ret_t custom_handlers_open_mywindow(custom_handlers_t* custom_handlers, const char* args) {
  navigator_to("string?request=mywindow");

  return RET_OBJECT_CHANGED;
}

/***************custom_handlers_view_model***************/

static ret_t custom_handlers_view_model_set_prop(tk_object_t* obj, const char* name,
                                                 const value_t* v) {
  log_debug("not found %s\n", name);
  return RET_NOT_FOUND;
}

static ret_t custom_handlers_view_model_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  log_debug("not found %s\n", name);
  return RET_NOT_FOUND;
}

static bool_t custom_handlers_view_model_can_exec(tk_object_t* obj, const char* name,
                                                  const char* args) {
  if (tk_str_eq("show_toast", name)) {
    return TRUE;
  } else if (tk_str_eq("show_info", name)) {
    return TRUE;
  } else if (tk_str_eq("show_confirm", name)) {
    return TRUE;
  } else if (tk_str_eq("open_mywindow", name)) {
    return TRUE;
  } else if (tk_str_eq("choose", name)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

static ret_t custom_handlers_view_model_exec(tk_object_t* obj, const char* name, const char* args) {
  custom_handlers_view_model_t* vm = (custom_handlers_view_model_t*)(obj);
  custom_handlers_t* custom_handlers = vm->custom_handlers;

  if (tk_str_eq("show_toast", name)) {
    return custom_handlers_show_toast(custom_handlers, args);
  } else if (tk_str_eq("show_info", name)) {
    return custom_handlers_show_info(custom_handlers, args);
  } else if (tk_str_eq("show_confirm", name)) {
    return custom_handlers_show_confirm(custom_handlers, args);
  } else if (tk_str_eq("choose", name)) {
    return custom_handlers_choose(custom_handlers, args);
  } else if (tk_str_eq("open_mywindow", name)) {
    return custom_handlers_open_mywindow(custom_handlers, args);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }
}

static ret_t custom_handlers_view_model_on_destroy(tk_object_t* obj) {
  custom_handlers_view_model_t* vm = (custom_handlers_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  custom_handlers_destroy(vm->custom_handlers);

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_custom_handlers_view_model_vtable = {
    .type = "custom_handlers",
    .desc = "custom_handlers",
    .size = sizeof(custom_handlers_view_model_t),
    .exec = custom_handlers_view_model_exec,
    .can_exec = custom_handlers_view_model_can_exec,
    .get_prop = custom_handlers_view_model_get_prop,
    .set_prop = custom_handlers_view_model_set_prop,
    .on_destroy = custom_handlers_view_model_on_destroy};

view_model_t* custom_handlers_view_model_create(navigator_request_t* req) {
  tk_object_t* obj = tk_object_create(&s_custom_handlers_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  custom_handlers_view_model_t* custom_handlers_view_model = (custom_handlers_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  custom_handlers_view_model->custom_handlers = custom_handlers_create();
  ENSURE(custom_handlers_view_model->custom_handlers != NULL);

  return vm;
}
