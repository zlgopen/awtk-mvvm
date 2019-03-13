/**
 * File:   utils.c
 * Author: AWTK Develop Team
 * Brief:  utils
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
 * 2019-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/base/utils.h"

const char* destruct_array_prop_name(const char* name, uint32_t* index) {
  const char* prop = NULL;
  return_value_if_fail(name != NULL && name[0] == '[', NULL);

  prop = strstr(name, "].");
  return_value_if_fail(prop != NULL, NULL);

  if (index != NULL) {
    *index = tk_atoi(name + 1);
  }

  return prop + 2;
}
