#include "awtk.h"
#include "mvvm_app.inc"
#include "remote_ui/service/remote_ui_service.h"

BEGIN_C_DECLS
#ifdef AWTK_WEB
#include "assets.inc"
#else /*AWTK_WEB*/
#include "../res/assets.inc"
#endif /*AWTK_WEB*/
END_C_DECLS

#ifdef LINUX
#include <signal.h>
static void on_signal(int s) {
  tk_quit();
}
#endif/*LINUX*/

extern ret_t application_init(void);

extern ret_t application_exit(void);

static ret_t global_init(void) {
  mvvm_app_init();
  mvvm_app_run_default_scripts();

#ifdef TK_IS_PC
  tk_service_start(main_loop_get_event_source_manager(main_loop()), REMOTE_UI_URL,
                   remote_ui_service_create, NULL);
#endif /*TK_IS_PC*/

#ifdef LINUX
  signal(SIGINT, on_signal);
#endif /*LINUX*/  

  return RET_OK;
}

#define GLOBAL_INIT() global_init()
#define GLOBAL_EXIT() mvvm_app_deinit()

#include "awtk_main.inc"
