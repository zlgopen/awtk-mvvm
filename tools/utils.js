class Utils {
  static genToValue(type, name) {
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
      case 'void*': {
        return `value_set_pointer(v, ${name});`;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  static genFromValue(clsName, type, name) {
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
      case 'void*': {
        return `value_pointer(v)`;
      }
      default: {
        console.log(`not supported ${type} for ${name}`);
        process.exit(0);
      }
    }
  }

  static genAssignValue(clsName, name, type) {
    if (type === 'char*') {
      return `tk_str_copy(${clsName}->${name}, ${name});`;
    } else {
      return `${clsName}->${name} = ${name};`;
    }
  }

  static genModelCommonFuncs(json) {
    const clsName = json.name;
    let cmp = json.cmp ? json.cmp : "/*TODO: */\n  return 0;"
    let init = json.init ? json.init : "/*TODO: */"
    let defulatDeinit = '';
    if(json.props) {
      defulatDeinit = json.props.map(iter => {
        if(iter.type == 'char*') {
          return `  TKMEM_FREE(${clsName}->${iter.name});\n`;
        } else {
          return '';
        }
      }).join('');
    }
    let deinit = json.deinit ? json.deinit : defulatDeinit;

    let cmpFunc = '';
    if(json.collection) {
    cmpFunc = 
`
int ${clsName}_cmp(${clsName}_t* a, ${clsName}_t* b) {
  return_value_if_fail(a != NULL && b != NULL, -1);
${cmp}
}
`
    }
    let result =
      `
/***************${clsName}***************/;

${clsName}_t* ${clsName}_create(void) {
  ${clsName}_t* ${clsName} = TKMEM_ZALLOC(${clsName}_t);
  return_value_if_fail(${clsName} != NULL, NULL);

${init}

  return ${clsName};
} 

${cmpFunc}

static ret_t ${clsName}_destroy(${clsName}_t* ${clsName}) {
  return_value_if_fail(${clsName} != NULL, RET_BAD_PARAMS);

${deinit}

  TKMEM_FREE(${clsName});

  return RET_OK;
}

`
    return result;
  }
  
  static genPropFuncs(json) {
    const clsName = json.name;
    const result = json.props.map((prop, index) => {
      let str = '';
      const propName = prop.name;

      if (prop.private) {
        return str;
      }

      if (prop.fake) {
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
            `${Utils.genAssignValue(clsName, propName, prop.type)}

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
  
  static genCmdFuncs(json) {
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

}

module.exports = Utils;
