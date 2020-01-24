const fs = require('fs')
const path = require('path')

class CodeGen {
  isReadable(obj) {
    return obj.annotation && obj.annotation.readable;
  }

  isWritable(obj) {
    return obj.annotation && obj.annotation.writable;
  }
  
  isCommand(obj) {
    return obj.annotation && obj.annotation.command;
  }
  
  findMethod(cls, name) {
    return cls.methods.find(iter => (iter.name === name));
  }

  hasCreate(cls) {
    let clsName = cls.name.replace(/_t$/, '');
    let methodName = clsName + '_create';

    return cls.methods.find(iter => (iter.name === methodName));
  }

  hasDestroy(cls) {
    let clsName = this.toClassName(cls.name);
    let methodName = clsName + '_destroy';

    return cls.methods.find(iter => (iter.name === methodName));
  }

  hasSingleton(cls) {
    let clsName = this.toClassName(cls.name);
    let methodName = clsName;

    return cls.methods.find(iter => (iter.name === methodName));
  }

  hasSetterFor(cls, name) {
    let clsName = this.toClassName(cls.name);
    let methodName = clsName + '_set_' + name;

    return cls.methods.find(iter => (iter.name === methodName));
  }

  hasGetterFor(cls, name) {
    let clsName = this.toClassName(cls.name);
    let methodName = clsName + '_get_' + name;

    return cls.methods.find(iter => (iter.name === methodName));
  }

  genIncludes(name) {
    let result = `#include "tkc/mem.h"\n`;
    result += `#include "tkc/utils.h"\n`;
    result += `#include "${name}.h"`;

    return result;
  }

  saveResult(name, includes, header, content) {
    fs.writeFileSync(`${name}.h`, "\ufeff" + header);
    fs.writeFileSync(`${name}.c`, "\ufeff" + content);

    console.log(`output to ${name}.h and ${name}.c`);
  }

  genJson(json) {
    json.forEach(iter => {
      let includes = iter.includes;
      let header = this.genHeader(iter);
      let content = this.genContent(iter);
      let name = this.toClassName(iter.name) + '_view_model';
      this.saveResult(name, includes, header, content);
    });
  }

  toClassName(name) {
    return name.replace(/_t$/, '');
  }

  genFileComment(json, ext) {
    const now = new Date();
    const team = json.team;
    const author = json.author || "codegen";
    const clsName = json.name;
    const desc = json.desc || "";
    const copyright = json.copyright;

    const day = now.getDate();
    const year = now.getFullYear();
    const month = now.getMonth() + 1;
    const date = json.date || `${year}-${month}-${day}`

    return `/**
 * File:   ${clsName}.${ext}
 * Author: AWTK Develop Team
 * Brief:  ${desc}
 *
 * Copyright (c) ${year} - ${year} ${copyright}
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
 * ${date} ${author} created
 *
 */
`
  }

  mapType(type) {
    if (type.indexOf("char*") >= 0) {
      return `const char*`;
    } else {
      return type;
    }
  }

  genParamDecl(type, name) {
    return `${this.mapType(type)} ${name}`;
  }

  genToValue(type, name) {
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'bool_t':
      case 'float_t':
      case 'float':
      case 'double': {
        let typeName = type.replace(/_t$/, '');
        return `value_set_${typeName}(v, ${name});`;
      }
      default: {
        if(type.indexOf('char*') >= 0) {
          return `value_set_str(v, ${name});`;
        } else if(type.indexOf('void*') >= 0) {
          return `value_set_pointer(v, ${name});`;
        } else {
          console.log(`not supported ${type} for ${name}`);
          process.exit(0);
        }
      }
    }
  }

  genFromValue(type, name) {
    let typeName = '';
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'bool_t':
      case 'float_t':
      case 'float':
      case 'double': {
        typeName = type.replace(/_t$/, '');
        break;
      }
      default: {
        if(type.indexOf('char*') >= 0) {
          typeName = 'str';
        } else if(type.indexOf('void*') >= 0) {
          typeName = 'pointer';
        } else {
          console.log(`not supported ${type} for ${name}`);
          process.exit(0);
        }
      }
    }

    return `value_${typeName}(v)`;
  }

  genAssignValue(type, name, value) {
    if (type.indexOf("char*") >= 0) {
      return `${name} = tk_str_copy(${name}, ${value});`;
    } else {
      return `${name} = ${value};`;
    }
  }
}

module.exports = CodeGen;
