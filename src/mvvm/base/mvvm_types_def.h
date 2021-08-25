﻿/**
 * File:   types_def.h
 * Author: AWTK Develop Team
 * Brief:  basic types definitions.
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
 * 2019-01-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MVVM_TYPES_DEF_H
#define TK_MVVM_TYPES_DEF_H

#include "tkc/types_def.h"

struct _binding_context_t;
typedef struct _binding_context_t binding_context_t;

#define WIDGET_PROP_V_MODEL "v-model"
#define WIDGET_PROP_V_MODEL_CHILDREN "v-model-children"
#define WIDGET_PROP_V_FOR_ITEMS "v-for-items"

#endif /*TK_MVVM_TYPES_DEF_H*/
