/**
 * File:   utils.c
 * Author: AWTK Develop Team
 * Brief:  utils
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

const char* tk_destruct_array_prop_name(const char* name, uint32_t* index) {
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

ret_t str_random(str_t* str, const char* format, uint32_t max) {
  char buff[256];
  return_value_if_fail(str != NULL && format != NULL && max > 0, RET_BAD_PARAMS);

  tk_snprintf(buff, sizeof(buff) - 1, format, random() % max);

  return str_set(str, buff);
}

bool_t tk_is_valid_prop_name(const char* name) {
  const char* p = name;
  return_value_if_fail(name != NULL, FALSE);

  if (tk_str_start_with(name, "item.")) {
    return TRUE;
  }

  while (*p) {
    if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') ||
        *p == '_' || *p == '.' || *p == '[' || *p == ']') {
      p++;
    } else {
      return FALSE;
    }
  }

  return TRUE;
}
