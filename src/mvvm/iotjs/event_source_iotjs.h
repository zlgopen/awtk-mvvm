/**
 * File:   event_source_iotjs.h
 * Author: AWTK Develop Team
 * Brief:  event source iotjs
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-02-14 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#ifndef TK_EVENT_SOURCE_IDLE_H
#define TK_EVENT_SOURCE_IDLE_H

#include "tkc/types_def.h"
#include "tkc/event_source.h"

BEGIN_C_DECLS

struct _event_source_t;
typedef struct _event_source_t event_source_iotjs_t;

/*private*/
event_source_t* event_source_iotjs_create();

#define EVENT_SOURCE_IDLE(obj) ((event_source_iotjs_t*)(obj))

END_C_DECLS

#endif /*TK_EVENT_SOURCE_IDLE_H*/
