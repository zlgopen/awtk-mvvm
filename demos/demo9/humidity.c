/**
 * File:   humidity.c
 * Author: AWTK Develop Team
 * Brief:  humidity
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
#include "humidity.h"

humidity_t* humidity_create(void) {
  return TKMEM_ZALLOC(humidity_t);
}

ret_t humidity_destroy(humidity_t* humidity) {
  return_value_if_fail(humidity != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(humidity);

  return RET_OK;
}

ret_t humidity_apply(humidity_t* humidity) {
  return_value_if_fail(humidity != NULL, RET_BAD_PARAMS);

  humidity->saved_value = humidity->value;

  return RET_OK;
}

bool_t humidity_can_apply(humidity_t* humidity) {
  return_value_if_fail(humidity != NULL, FALSE);

  return humidity->saved_value != humidity->value;
}
