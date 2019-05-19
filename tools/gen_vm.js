const fs = require('fs')
const path = require('path')

class CodeGen {
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
      case 'float_t':
      case 'float':
      case 'double': {
        let typeName = type.replace(/_t$/, '');
        return `value_set_${typeName}(v, ${name});`;
      }
      case 'char*': {
        return `value_set_str(v, ${name});`;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  genFromValue(clsName, type, name) {
    switch (type) {
      case 'int8_t':
      case 'int16_t':
      case 'int32_t':
      case 'int64_t':
      case 'uint8_t':
      case 'uint16_t':
      case 'uint32_t':
      case 'uint64_t':
      case 'float_t':
      case 'float':
      case 'double': {
        let typeName = type.replace(/_t$/, '');
        return `value_${typeName}(v)`;
      }
      case 'char*': {
        return `tk_str_copy(${clsName}->${name}, value_str(v))`;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  genAssignValue(clsName, name, type) {
    if (type === 'char*') {
      return `tk_str_copy(${clsName}->${name}, ${name});`;
    } else {
      return `${clsName}->${name} = ${name};`;
    }
  }

  genHeader(json) {
    let propsDecl = '';
    let clsName = json.name;
    let clsNameUpper = clsName.toUpperCase();

    if (json.props && json.props.length) {
      propsDecl = json.props.map((prop) => {
        if (prop.synthesized) {
          return '';
        } else {
          return `  ${prop.type} ${prop.name};`;
        }
      }).join('\n');
    }
    let result =
      `
#ifndef TK_${clsNameUpper}_H
#define TK_${clsNameUpper}_H

#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

/**
 * @class ${clsName}_t
 *
 * ${json.desc}
 *
 */
typedef struct _${clsName}_t{
${propsDecl}
} ${clsName}_t;

/**
 * @class ${clsName}_view_model_t
 *
 * view model of ${json.name}
 *
 */
typedef struct _${clsName}_view_model_t {
  view_model_t view_model;

  /*model object*/
  ${clsName}_t* ${clsName};
} ${clsName}_view_model_t;

/**
 * @method ${clsName}_view_model_create
 * 创建${clsName} view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* ${clsName}_view_model_create(navigator_request_t* req);

END_C_DECLS

#endif /*TK_${clsNameUpper}_H*/
`
    return result;
  }

  genGetProps(json) {
    const clsName = json.name;
    const dispatch = json.props.map((prop, index) => {
      let str = '  ';
      const propName = prop.name;

      if (prop.private) {
        return str;
      }

      if (index === 0) {
        str += 'if (';
      } else {
        str += '} else if (';
      }

      str += `tk_str_eq("${propName}", name)) {\n`
      if (prop.getter || prop.synthesized) {
        str += `    ${prop.type} ${propName} = ${clsName}_get_${propName}(${clsName});\n`;
      } else {
        str += `    ${prop.type} ${propName} = ${clsName}->${propName};\n`;
      }
      str += '    ' + this.genToValue(prop.type, propName);
      return str;
    }).join('\n');

    const result =
      `
static ret_t ${clsName}_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  ${clsName}_t* ${clsName} = vm->${clsName};

${dispatch}
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}

`
    return result;
  }

  genPropFuncs(json) {
    const clsName = json.name;
    const result = json.props.map((prop, index) => {
      let str = '';
      const propName = prop.name;

      if (prop.private) {
        return str;
      }

      if (prop.synthesized) {
        let setter = typeof (prop.setter) === 'string' ? prop.setter : 'return RET_OK;';
        let getter = typeof (prop.getter) === 'string' ? prop.getter : 'return 0;';
        str =
          `
static ${prop.type} ${clsName}_get_${propName}(${clsName}_t* ${clsName}) {
  ${getter}
}

static ret_t ${clsName}_set_${propName}(${clsName}_t* ${clsName}, ${prop.type} value) {
  ${setter}
}

`;
      } else {
        if (prop.getter) {
          let defaultGetter = `return ${clsName}->${propName};`;
          let getter = typeof (prop.getter) === 'string' ? prop.getter : defaultGetter;
          str =
            `
static ${prop.type} ${clsName}_get_${propName}(${clsName}_t* ${clsName}) {
  ${getter}
}

`;
        }

        if (prop.setter) {
          let defaultSetter =
            `${this.genAssignValue(clsName, propName, prop.type)}

  return RET_OK;`

          let setter = typeof (prop.setter) === 'string' ? prop.setter : defaultSetter;
          str +=
            `
static ret_t ${clsName}_set_${propName}(${clsName}_t* ${clsName}, ${prop.type} ${prop.name}) {
  ${setter}
}

`;
        }
      }
      return str;
    }).join("");

    return result;
  }

  genSetProps(json) {
    const clsName = json.name;
    const dispatch = json.props.map((prop, index) => {
      let str = '  ';
      const propName = prop.name;
      if (prop.private) {
        return str;
      }

      if (index === 0) {
        str += 'if (';
      } else {
        str += '} else if (';
      }
      str += `tk_str_eq("${propName}", name)) {\n`
      if (prop.setter || prop.synthesized) {
        str += `    ${clsName}_set_${propName}(${clsName}, ${this.genFromValue(clsName, prop.type, prop.name)});`;
      } else {
        str += `    ${clsName}->${propName} = ${this.genFromValue(clsName, prop.type, prop.name)};`;
      }
      return str;
    }).join('\n');

    const result =
      `
static ret_t ${clsName}_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  ${clsName}_t* ${clsName} = vm->${clsName};

${dispatch}
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}

`
    return result;
  }

  genExec(json) {
    const clsName = json.name;
    const dispatch = json.cmds.map((cmd, index) => {
      const cmdName = cmd.name;
      let str = '  ';
      if (index === 0) {
        str += 'if (';
      } else {
        str += '} else if (';
      }
      str += `tk_str_eq("${cmdName}", name)) {\n`
      str += `    return ${clsName}_${cmdName}(${clsName}, args);`;
      return str;
    }).join('\n');

    const result =
      `
static ret_t ${clsName}_view_model_exec(object_t* obj, const char* name, const char* args) {
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  ${clsName}_t* ${clsName} = vm->${clsName};

${dispatch}
  } else {
    return RET_NOT_FOUND;
  }
}
`
    return result;
  }

  genCmdFuncs(json) {
    const clsName = json.name;
    const result = json.cmds.map((cmd, index) => {
      let str = '';
      const cmdName = cmd.name;
      if (!cmd.canExec || typeof cmd.canExec === 'string') {
        let canExec = (typeof cmd.canExec === 'string') ? cmd.canExec : 'return TRUE;';
        str =
          `static bool_t ${clsName}_can_exec_${cmdName}(${clsName}_t* ${clsName}, const char* args) {
  ${canExec}
}\n\n`;
      }

      let impl = cmd.impl || '';
      str +=
        `static ret_t ${clsName}_${cmdName}(${clsName}_t* ${clsName}, const char* args) {
  ${impl}
  return RET_OBJECT_CHANGED;
}\n\n`;
      return str;
    }).join('\n');

    return result;
  }

  genCanExec(json) {
    const clsName = json.name;
    const dispatch = json.cmds.map((cmd, index) => {
      const cmdName = cmd.name;
      let str = '  ';
      if (index === 0) {
        str += 'if (';
      } else {
        str += '} else if (';
      }
      str += `tk_str_eq("${cmdName}", name)) {\n`

      if (!cmd.canExec || typeof cmd.canExec === 'string') {
        str += `    return ${clsName}_can_exec_${cmdName}(${clsName}, args);`;
      } else {
        str += `    return TRUE;`;
      }
      return str;
    }).join('\n');

    const result =
      `
static bool_t ${clsName}_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  ${clsName}_t* ${clsName} = vm->${clsName};

${dispatch}
  } else {
    return FALSE;
  }
}
`
    return result;
  }

  genVTable(json) {
    const clsName = json.name;
    const clsDesc = json.desc || clsName;
    let result =
      `
static ${clsName}_t* ${clsName}_create(void) {
  return TKMEM_ZALLOC(${clsName}_t);
} 

static ret_t ${clsName}_destroy(${clsName}_t* ${clsName}) {
  TKMEM_FREE(${clsName});

  return RET_OK;
}

static ret_t ${clsName}_view_model_on_destroy(object_t* obj) {
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  ${clsName}_destroy(vm->${clsName});

  return view_model_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_${clsName}_view_model_vtable = {
  .type = "${clsName}",
  .desc = "${clsDesc}",
  .size = sizeof(${clsName}_view_model_t),
  .exec = ${clsName}_view_model_exec,
  .can_exec = ${clsName}_view_model_can_exec,
  .get_prop = ${clsName}_view_model_get_prop,
  .set_prop = ${clsName}_view_model_set_prop,
  .on_destroy = ${clsName}_view_model_on_destroy
};

view_model_t* ${clsName}_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_${clsName}_view_model_vtable);
  view_model_t* vm = view_model_init(VIEW_MODEL(obj));
  ${clsName}_view_model_t* ${clsName}_view_model = (${clsName}_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  ${clsName}_view_model->${clsName} = ${clsName}_create();
  ENSURE(${clsName}_view_model->${clsName} != NULL);

  return vm;
}
`
    return result;
  }

  genContent(json) {
    let result = `#include "tkc/mem.h"\n`;
    result += `#include "tkc/utils.h"\n`;
    result += `#include "${json.name}.h"\n`;
    const clsName = json.name;
    if (json.props && json.props.length) {
      result += this.genPropFuncs(json);
      result += this.genSetProps(json);
      result += this.genGetProps(json);
    } else {
      result +=
        `
static ret_t ${clsName}_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t ${clsName}_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

`
    }

    if (json.cmds && json.cmds.length) {
      result += this.genCmdFuncs(json);
      result += this.genCanExec(json);
      result += this.genExec(json);
    } else {
      result +=
        `
static bool_t ${clsName}_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  return FALSE;
}

static ret_t ${clsName}_view_model_exec(object_t* obj, const char* name, const char* args) {
  return RET_NOT_IMPL;
}

`
    }

    result += this.genVTable(json);

    return result;
  }

  genJson(json) {
    let header = this.genHeader(json);
    let content = this.genContent(json);

    fs.writeFileSync(`${json.name}.h`, header);
    fs.writeFileSync(`${json.name}.c`, content);

    console.log(`output to ${json.name}.h and ${json.name}.c`);
  }

  genFile(filename) {
    this.genJson(JSON.parse(fs.readFileSync(filename).toString()));
  }

  static run(filename) {
    const gen = new CodeGen();
    gen.genFile(filename);
  }
}

if (process.argv.length < 3) {
  console.log(`Usage: node gen_vm.js idl.json`);
  process.exit(0);
}

CodeGen.run(process.argv[2]);
