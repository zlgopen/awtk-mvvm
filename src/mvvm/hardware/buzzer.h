/**
 * File:   buzzer.h
 * Author: AWTK Develop Team
 * Brief:  buzzer
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
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUZZER_H
#define TK_BUZZER_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _buzzer_t;
typedef struct _buzzer_t buzzer_t;

/**
 * @class buzzer_t
 * @parent object_t
 *
 * 蜂鸣器。
 */
struct _buzzer_t {
  object_t object;

  /**
   * @property {int32_t} volume
   * @annotation ["set_prop","get_prop"]
   * 音量(0-100)。
   */
  int32_t volume;

  /**
   * @property {int32_t} freq
   * @annotation ["set_prop","get_prop"]
   * 频率。
   */
  int32_t freq;

  /**
   * @property {bool_t} on
   * @annotation ["set_prop","get_prop"]
   * 开启。
   */
  bool_t on;
};

#define BUZZER(object) ((buzzer_t*)(object))

#define BUZZER_TYPE "buzzer"

#define BUZZER_PROP_ON "on"
#define BUZZER_PROP_FREQ "freq"
#define BUZZER_PROP_VOLUME "volume"
END_C_DECLS

#endif /*TK_BUZZER_H*/
