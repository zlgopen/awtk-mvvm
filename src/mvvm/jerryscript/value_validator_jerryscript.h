/**
 * File:   value_validator_jerryscript.h
 * Author: AWTK Develop Team
 * Brief:  jerryscript implemented value_validator
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
 * 2019-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VALUE_VALIDATOR_JERRYSCRIPT_H
#define TK_VALUE_VALIDATOR_JERRYSCRIPT_H

#include "tkc/str.h"
#include "mvvm/base/value_validator.h"

BEGIN_C_DECLS

/**
 * @class value_validator_jerryscript_t
 * @parent value_validator_t
 *
 * jerry script implemented value_validator
 *
 */
typedef struct _value_validator_jerryscript_t {
  value_validator_t value_validator;
} value_validator_jerryscript_t;

#define VALUE_VALIDATOR_JERRYSCRIPT(c) ((value_validator_jerryscript_t*)c)

ret_t value_validator_jerryscript_init(void);

ret_t value_validator_jerryscript_deinit(void);

END_C_DECLS

#endif /*TK_VALUE_VALIDATOR_JERRYSCRIPT_H*/
