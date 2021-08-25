#include "awtk.h"
#include "mvvm_app.inc"

#define GLOBAL_INIT() mvvm_app_init()
#define GLOBAL_EXIT() mvvm_app_deinit()

BEGIN_C_DECLS
#ifdef AWTK_WEB
#include "assets.inc"
#else /*AWTK_WEB*/
#include "../res/assets.inc"
#endif /*AWTK_WEB*/
END_C_DECLS

extern ret_t application_init(void);

extern ret_t application_exit(void);

#include "awtk_main.inc"
