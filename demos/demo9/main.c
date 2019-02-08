/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  main
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
 * 2019-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/mvvm.h"

#include "main.h"

static ret_t navigate(void* ctx, const char* args) {
  return navigator_to(args);
}

model_t* main_create(void* args) {
  model_t* model = model_delegate_create(NULL, NULL);

  MODEL_COMMAND(model, "navigate", navigate, NULL);

  return model;
}
