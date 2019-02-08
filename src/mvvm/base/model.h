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
 * 2019-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MODEL_H
#define TK_MODEL_H

#include "tkc/object.h"
#include "mvvm/base/navigator_request.h"

BEGIN_C_DECLS

struct _model_t;
typedef struct _model_t model_t;

typedef ret_t (*model_on_will_mount_t)(model_t* model, navigator_request_t* req);
typedef ret_t (*model_on_mount_t)(model_t* model);
typedef ret_t (*model_on_will_unmount_t)(model_t* model);
typedef ret_t (*model_on_unmount_t)(model_t* model);

typedef model_t* (*model_create_t)(void* args);

typedef struct _model_vtable_t {
  model_on_will_mount_t on_will_mount;
  model_on_mount_t on_mount;
  model_on_will_unmount_t on_will_unmount;
  model_on_unmount_t on_unmount;
} model_vtable_t;

/**
 * @class model_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 模型的基类。
 *
 */
struct _model_t {
  object_t object;
  const model_vtable_t* vt;
};

ret_t model_on_will_mount(model_t* model, navigator_request_t* req);
ret_t model_on_mount(model_t* model);
ret_t model_on_will_unmount(model_t* model);
ret_t model_on_unmount(model_t* model);

typedef enum _model_event_type_t {
  EVT_MODEL_MOUNT = 0xff,
  EVT_MODEL_WILL_MOUNT,
  EVT_MODEL_UNMOUNT,
  EVT_MODEL_WILL_UNMOUNT,
} model_event_type_t;

typedef struct _model_will_mount_event_t {
  event_t e;
  navigator_request_t* req;
} model_will_mount_event_t;

#define MODEL(model) ((model_t*)(model))

model_t* model_dummy_create(void* args);

END_C_DECLS

#endif /*TK_MODEL_H*/
