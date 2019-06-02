
#include "awtk.h"
#include "mvvm/base/navigator.h"

static ret_t toast_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  object_t* o = OBJECT(req);
  const char* content = object_get_prop_str(o, NAVIGATOR_ARG_CONTENT);
  uint32_t duration = object_get_prop_int(o, NAVIGATOR_ARG_DURATION, 1000);

  dialog_toast(content, duration);

  return RET_OK;
}

static ret_t info_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  object_t* o = OBJECT(req);
  const char* title = object_get_prop_str(o, NAVIGATOR_ARG_TITLE);
  const char* content = object_get_prop_str(o, NAVIGATOR_ARG_CONTENT);

  dialog_info(title, content);

  return RET_OK;
}

static ret_t confirm_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t result;
  object_t* o = OBJECT(req);
  const char* title = object_get_prop_str(o, NAVIGATOR_ARG_TITLE);
  const char* content = object_get_prop_str(o, NAVIGATOR_ARG_CONTENT);
  int32_t ret = dialog_confirm(title, content);

  value_set_int32(&result, ret);
  navigator_request_on_result(req, &result);

  return RET_OK;
}

static ret_t button_on_click(void* ctx, event_t* e) {
  window_close(WIDGET(ctx));
  return RET_OK;
}

static ret_t mywindow_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);

  widget_set_text(ok, L"Close");
  widget_set_prop_str(win, WIDGET_PROP_ANIM_HINT, "htranslate");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, button_on_click, win);

  widget_layout(win);

  return RET_OK;
}

ret_t register_custom_handlers(void) {
  navigator_register_handler(navigator(), NAVIGATOR_REQ_TOAST,
                             navigator_handler_create(toast_on_request));
  navigator_register_handler(navigator(), NAVIGATOR_REQ_INFO,
                             navigator_handler_create(info_on_request));
  navigator_register_handler(navigator(), NAVIGATOR_REQ_CONFIRM,
                             navigator_handler_create(confirm_on_request));
  navigator_register_handler(navigator(), "mywindow",
                             navigator_handler_create(mywindow_on_request));

  return RET_OK;
}
