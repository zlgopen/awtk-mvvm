/**
 * File:  view_model.cpp
 * Author: AWTK Develop Team
 * Brief:  view_model interface for cpp
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/cpp/view_model.hpp"

namespace vm {

ViewModel::ViewModel(navigator_request_t* request) : request(request) {
}

ViewModel::~ViewModel() {
}

ret_t ViewModel::OnWillMount(navigator_request_t* request) {
  log_debug("%s\n", __FUNCTION__);

  return RET_OK;
}

ret_t ViewModel::OnMount() {
  log_debug("%s\n", __FUNCTION__);

  return RET_OK;
}

ret_t ViewModel::OnWillUnmount() {
  log_debug("%s\n", __FUNCTION__);

  return RET_OK;
}

ret_t ViewModel::OnUnmount() {
  log_debug("%s\n", __FUNCTION__);

  return RET_OK;
}
}  // namespace vm
