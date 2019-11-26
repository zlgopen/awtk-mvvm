/**
 * File:   mvvm.h
 * Author: AWTK Develop Team
 * Brief:  mvvm awtk global functions.
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
 * 2019-02-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/awtk/mvvm_awtk.h"

ret_t mvvm_awtk_init(void) {
  navigator_register_handler(navigator(), NAVIGATOR_DEFAULT_HANDLER,
                             navigator_handler_awtk_create());

  navigator_register_handler(navigator(), NAVIGATOR_REQ_TOAST,
                             navigator_handler_awtk_toast_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_INFO, navigator_handler_awtk_info_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_WARN, navigator_handler_awtk_warn_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_CONFIRM,
                             navigator_handler_awtk_confirm_create());

  navigator_register_handler(navigator(), NAVIGATOR_REQ_HOME, navigator_handler_awtk_home_create());

  navigator_register_handler(navigator(), NAVIGATOR_REQ_BACK, navigator_handler_awtk_back_create());

#ifdef WITH_NATIVE_FILE_DIALOG
  navigator_register_handler(navigator(), NAVIGATOR_REQ_PICK_FILE,
                             navigator_handler_awtk_pick_file_create());
  navigator_register_handler(navigator(), NAVIGATOR_REQ_PICK_DIR,
                             navigator_handler_awtk_pick_dir_create());
#endif /*WITH_NATIVE_FILE_DIALOG*/

  return RET_OK;
}

ret_t mvvm_awtk_deinit(void) {
  return RET_OK;
}
