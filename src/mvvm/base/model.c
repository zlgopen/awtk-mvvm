/**
 * File:   model.h
 * Author: AWTK Develop Team
 * Brief:  model
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/base/model.h"

ret_t model_on_will_mount(model_t* model, navigator_request_t* req) {
  model_will_mount_event_t e;
  return_value_if_fail(model != NULL && req != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  e.req = req;
  e.e = event_init(EVT_MODEL_WILL_MOUNT, model);
  emitter_dispatch(EMITTER(model), (event_t*)&e);
  if (model->vt != NULL && model->vt->on_will_mount != NULL) {
    return model->vt->on_will_mount(model, req);
  }

  return RET_OK;
}

ret_t model_on_mount(model_t* model) {
  event_t e = event_init(EVT_MODEL_MOUNT, model);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  emitter_dispatch(EMITTER(model), (event_t*)&e);
  if (model->vt != NULL && model->vt->on_mount != NULL) {
    return model->vt->on_mount(model);
  }

  return RET_OK;
}

ret_t model_on_will_unmount(model_t* model) {
  event_t e = event_init(EVT_MODEL_WILL_UNMOUNT, model);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  emitter_dispatch(EMITTER(model), (event_t*)&e);
  if (model->vt != NULL && model->vt->on_will_unmount != NULL) {
    return model->vt->on_will_unmount(model);
  }

  return RET_OK;
}

ret_t model_on_unmount(model_t* model) {
  event_t e = event_init(EVT_MODEL_WILL_UNMOUNT, model);
  return_value_if_fail(model != NULL, RET_BAD_PARAMS);

  log_debug("%s\n", __FUNCTION__);
  emitter_dispatch(EMITTER(model), (event_t*)&e);
  if (model->vt != NULL && model->vt->on_unmount != NULL) {
    return model->vt->on_unmount(model);
  }

  return RET_OK;
}

