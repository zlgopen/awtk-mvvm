/**
 * File:   shape.c
 * Author: AWTK Develop Team
 * Brief:  shape
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "shape.h"

shape_t* shape_create(void) {
  shape_t* shape = TKMEM_ZALLOC(shape_t);
  return_value_if_fail(shape != NULL, NULL);

  shape->type = 1;
  shape->x = 10;
  shape->y = 20;
  shape->w = 30;
  shape->h = 40;
  shape->opacity = 50;
  shape->text_align = 1;
  str_init(&(shape->name), 10);
  str_init(&(shape->overview), 10);
  str_set(&(shape->name), "中文");

  return shape;
}

ret_t shape_destroy(shape_t* shape) {
  return_value_if_fail(shape != NULL, RET_BAD_PARAMS);

  str_reset(&(shape->name));
  str_reset(&(shape->overview));

  TKMEM_FREE(shape);

  return RET_OK;
}

const char* shape_get_overview(shape_t* shape) {
  char buff[256];
  tk_snprintf(buff, sizeof(buff), "%s: type=%d (%d %d %d %d) opacity=%d align=%d", shape->name.str,
              shape->type, shape->x, shape->y, shape->w, shape->h, shape->opacity,
              shape->text_align);

  str_set(&(shape->overview), buff);

  return shape->overview.str;
}

ret_t shape_change_type(shape_t* shape, int32_t type) {
  shape->type = type;

  return RET_OK;
}

bool_t shape_can_save(shape_t* shape) {
  return shape->name.size > 0;
}

ret_t shape_save(shape_t* shape) {
  return RET_OK;
}

ret_t shape_set_name(shape_t* shape, const char* name) {
  str_set(&(shape->name), name);
  return RET_OK;
}
