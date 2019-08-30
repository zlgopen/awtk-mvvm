/**
 * File:  value_converter.hpp
 * Author: AWTK Develop Team
 * Brief:  value converter interface for cpp
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
 * 2019-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DATA_CONVERTER_HPP
#define TK_DATA_CONVERTER_HPP

#include "mvvm/base/value_converter.h"

/**
 * @class ValueConverter
 *
 * 值转换器。
 *
 * 如果数据在View上显示的格式和在Model中保存的格式不一样，ValueConverter负责在两者之间转换。
 *
 */
class ValueConverter {
  public:
    ValueConverter() {}
    virtual ~ValueConverter() {}
  
  public:
    /**
     * @method ToModel
     * 将value转换成适合model存储的格式。
     *
     * @param {value_t*} from 源value。
     * @param {value_t*} to 转换结果。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t ToModel(const value_t* from, value_t* to) = 0;

    /**
     * @method ToView
     * 将value转换成适合view显示的格式。
     *
     * @param {value_t*} from 源value。
     * @param {value_t*} to 转换结果。
     *
     * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
     */
    virtual ret_t ToView(const value_t* from, value_t* to) = 0;
};

#endif/*TK_DATA_CONVERTER_HPP*/

