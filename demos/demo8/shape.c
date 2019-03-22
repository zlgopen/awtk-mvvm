/**
 * File:   shape.c
 * Author: AWTK Develop Team
 * Brief:  shape
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
 * 2019-02-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/view_model_delegate.h"

#include "shape.h"

static ret_t shape_save(shape_t* t, const char* args) {
  log_debug("name=%s x=%d y=%d w=%d h=%d align=%d type=%d opacity=%d\n", t->name, t->x, t->y, t->w,
            t->h, t->align, t->type, t->opacity);

  return RET_OK;
}

static ret_t shape_set_type(shape_t* t, const char* args) {
  t->type = tk_atoi(args);

  object_notify_changed(OBJECT(t));

  return RET_OBJECT_CHANGED;
}

static bool_t shape_can_save(shape_t* t, const char* args) {
  return t->name != NULL && t->name[0] != '\0';
}

static ret_t shape_get_overview(shape_t* t, value_t* v) {
  char str[128];
  tk_snprintf(str, sizeof(str) - 1, "%s type=%d align=%d opacity=%d (%d %d %d %d)",
              t->name ? t->name : "(noname)", t->type, t->align, t->x, t->y, t->w, t->h,
              t->opacity);

  str_set(&(t->overview), str);
  value_set_str(v, t->overview.str);

  return RET_OK;
}

static ret_t shape_destroy(shape_t* t) {
  TKMEM_FREE(t->name);
  TKMEM_FREE(t);

  return RET_OK;
}

view_model_t* shape_create(navigator_request_t* req) {
  shape_t* t = TKMEM_ZALLOC(shape_t);
  view_model_t* view_model = view_model_delegate_create(t, (tk_destroy_t)shape_destroy);

  t->x = 1;
  t->y = 2;
  t->w = 3;
  t->h = 4;
  t->type = 1;

  VIEW_MODEL_SIMPLE_PROP(view_model, "type", VALUE_TYPE_INT32, &(t->type));
  VIEW_MODEL_SIMPLE_PROP(view_model, "x", VALUE_TYPE_INT32, &(t->x));
  VIEW_MODEL_SIMPLE_PROP(view_model, "y", VALUE_TYPE_INT32, &(t->y));
  VIEW_MODEL_SIMPLE_PROP(view_model, "w", VALUE_TYPE_INT32, &(t->w));
  VIEW_MODEL_SIMPLE_PROP(view_model, "h", VALUE_TYPE_INT32, &(t->h));
  VIEW_MODEL_SIMPLE_PROP(view_model, "align", VALUE_TYPE_INT32, &(t->align));
  VIEW_MODEL_SIMPLE_PROP(view_model, "opacity", VALUE_TYPE_INT32, &(t->opacity));

  VIEW_MODEL_SIMPLE_PROP(view_model, "name", VALUE_TYPE_STRING, &(t->name));
  VIEW_MODEL_PROP(view_model, "overview", shape_get_overview, NULL);

  VIEW_MODEL_COMMAND(view_model, "save", shape_save, shape_can_save);
  VIEW_MODEL_COMMAND(view_model, "setType", shape_set_type, NULL);

  return view_model;
}
