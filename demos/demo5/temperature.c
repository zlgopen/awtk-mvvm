/**
 * File:   temperature.c
 * Author: AWTK Develop Team
 * Brief:  temperature
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
 * 2021-05-13 Liu YuXin <liuyuxin@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "temperature.h"

temperature_t* temperature_create(void) {
  temperature_t* temperature = TKMEM_ZALLOC(temperature_t);
  return_value_if_fail(temperature != NULL, NULL);

  temperature->value = 20;

  return temperature;
}

ret_t temperature_destroy(temperature_t* temperature) {
  return_value_if_fail(temperature != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(temperature);

  return RET_OK;
}
