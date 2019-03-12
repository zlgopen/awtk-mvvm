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

typedef model_t* (*model_create_t)(navigator_request_t* req);

typedef struct _model_vtable_t {
  model_on_will_mount_t on_will_mount;
  model_on_mount_t on_mount;
  model_on_will_unmount_t on_will_unmount;
  model_on_unmount_t on_unmount;
} model_vtable_t;

/**
 * @class model_t
 * @parent object_t
 *
 * 模型的基类。
 *
 */
struct _model_t {
  object_t object;

  /*private*/
  const model_vtable_t* vt;
};

/**
 * @method model_on_will_mount
 * 打开视图即将加载模型时通知model。
 *
 * @param {model_t*} model model对象。
 * @param {navigator_request_t*} req request对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_on_will_mount(model_t* model, navigator_request_t* req);

/**
 * @method model_on_mount
 * 视图与模型绑定完成后通知模型。
 *
 * @param {model_t*} model model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_on_mount(model_t* model);

/**
 * @method model_on_will_unmount
 * 视图即将关闭时通知模型。
 *
 * @param {model_t*} model model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_on_will_unmount(model_t* model);

/**
 * @method model_on_unmount
 * 视图销毁时通知模型。
 *
 * @param {model_t*} model model对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_on_unmount(model_t* model);

#define MODEL(model) ((model_t*)(model))

/**
 * @enum model_event_type_t
 *
 * 模型相关的事件。
 *
 */
typedef enum _model_event_type_t {
  /**
   * @const EVT_MODEL_WILL_MOUNT
   *
   * 打开视图即将加载模型时通知model。
   */
  EVT_MODEL_WILL_MOUNT = 0xff,
  /**
   * @const EVT_MODEL_MOUNT
   *
   * 视图与模型绑定完成后通知模型。
   */
  EVT_MODEL_MOUNT,
  /**
   * @const EVT_MODEL_WILL_UNMOUNT
   *
   * 视图即将关闭时通知模型。
   */
  EVT_MODEL_WILL_UNMOUNT,
  /**
   * @const EVT_MODEL_UNMOUNT
   *
   * 视图销毁时通知模型。
   */
  EVT_MODEL_UNMOUNT,
} model_event_type_t;

/**
 * @class model_will_mount_event_t
 * @parent event_t
 * 打开视图即将加载模型时通知model时的数据结构。
 *
 */
typedef struct _model_will_mount_event_t {
  event_t e;

  /**
   * @property {navigator_request_t*} req
   * 请求对象。
   */
  navigator_request_t* req;
} model_will_mount_event_t;

END_C_DECLS

#endif /*TK_MODEL_H*/
