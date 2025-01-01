/**
 * File:   room_info.c
 * Author: AWTK Develop Team
 * Brief:  room_info
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "room_info.h"

room_info_t* room_info_create(const char* name) {
  room_info_t* info = TKMEM_ZALLOC(room_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->name = tk_strdup(name);
  str_init(&(info->summary), 100);

  return info;
}

const char* room_info_get_summary(room_info_t* room_info) {
  str_t* str = NULL;
  return_value_if_fail(room_info != NULL, NULL);
  str = &(room_info->summary);

  str_set(str, room_info->name);
  str_append(str, ":");
  str_append(str, " temp=");
  str_append_int(str, room_info->temp);
  str_append(str, " humidity=");
  str_append_int(str, room_info->humidity);

  return str->str;
}

ret_t room_info_destroy(room_info_t* room_info) {
  return_value_if_fail(room_info != NULL, RET_BAD_PARAMS);

  str_reset(&(room_info->summary));
  TKMEM_FREE(room_info->name);
  TKMEM_FREE(room_info);

  return RET_OK;
}
