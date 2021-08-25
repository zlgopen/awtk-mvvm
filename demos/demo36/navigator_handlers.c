
#include "awtk.h"
#include "mvvm/base/navigator.h"

static ret_t button_on_click(void* ctx, event_t* e) {
  window_close(WIDGET(ctx));
  return RET_OK;
}

static ret_t mywindow_on_request(navigator_handler_t* handler, navigator_request_t* req) {
#ifndef AWTK_NOGUI
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* ok = button_create(win, 0, 0, 0, 0);

  widget_set_text(ok, L"Close");
  widget_set_prop_str(win, WIDGET_PROP_ANIM_HINT, "htranslate");
  widget_set_self_layout_params(ok, "center", "middle", "50%", "30");
  widget_on(ok, EVT_CLICK, button_on_click, win);

  widget_layout(win);
#endif /*AWTK_NOGUI*/

  return RET_OK;
}

ret_t register_custom_handlers(void) {
  navigator_register_handler(navigator(), "mywindow",
                             navigator_handler_create(mywindow_on_request));

  return RET_OK;
}
