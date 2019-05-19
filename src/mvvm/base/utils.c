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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"

const char* destruct_array_prop_name(const char* name, uint32_t* index) {
  const char* prop = NULL;
  return_value_if_fail(name != NULL, NULL);

  prop = strstr(name, "].");
  if (prop != NULL && name[0] == '[') {
    if (index != NULL) {
      *index = tk_atoi(name + 1);
    }

    return prop + 2;
  } else {
    return name;
  }
}

char* str_gen_random(const char* format) {
  char buff[1024];
  return_value_if_fail(format != NULL, NULL);

  tk_snprintf(buff, sizeof(buff) - 1, format, random() % 10000);

  return tk_strdup(buff);
}
