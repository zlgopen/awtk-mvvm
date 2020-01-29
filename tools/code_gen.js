const fs = require('fs')
const path = require('path')

class CodeGen {
  isModel(obj) {
    return obj.annotation && obj.annotation.model;
  }

  isCollectionModel(obj) {
    return obj.annotation && obj.annotation.model &&
      obj.annotation.collection;
  }

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
    console.log('-----------------------------------------------------');
    json.forEach(iter => {
      let includes = iter.includes;
      let header = this.genHeader(iter);
      let content = this.genContent(iter);
      let name = this.toViewModelClassName(iter.name);
      this.saveResult(name, includes, header, content);
    });
  }

  toClassName(name) {
    return name.replace(/_t$/, '');
  }
  
  toObjName(name) {
    return 'a'+name.replace(/_t$/, '');
  }
  
  toViewModelClassName(name) {
    let clsName = this.toClassName(name).toLowerCase();

    return `${clsName}_view_model`
  }
  
  toViewModelClassType(name) {
    let clsName = this.toClassName(name).toLowerCase();

    return `${clsName}_view_model_t`
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
        if (type.indexOf('char*') >= 0) {
          return `value_set_str(v, ${name});`;
        } else if (type.indexOf('int') >= 0) {
          return `value_set_int(v, ${name});`;
        } else if (type.indexOf('long') >= 0) {
          return `value_set_int(v, ${name});`;
        } else if (type.indexOf('void*') >= 0) {
          return `value_set_pointer(v, ${name});`;
        } else if (type === 'str_t') {
          return `value_set_str(v, ${name});`;
        } else if (type === 'time_t') {
          return `value_set_int64(v, ${name});`;
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
        if (type.indexOf('char*') >= 0) {
          typeName = 'str';
        } else if (type.indexOf('void*') >= 0) {
          typeName = 'pointer';
        } else if (type === 'str_t') {
          typeName = 'str';
        } else if (type.indexOf('int') >= 0) {
          typeName = 'int';
        } else if (type.indexOf('long') >= 0) {
          typeName = 'int';
        } else if (type === 'time_t') {
          typeName = 'int64';
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

  genConstructor(json) {
    let clsName = this.toClassName(json.name);

    if (this.hasSingleton(json)) {
      return `${clsName}()`;
    } else if (this.hasCreate(json)) {
      const name = `${clsName}_create`;
      const info = this.findMethod(json, name);
      if (info.params.length === 1) {
        return `${name}(req)`;
      } else {
        return `${name}()`;
      }
    } else {
      return `TKMEM_ZALLOC(${clsName}_t)`;
    }
  }

  genDestructor(json) {
    const clsName = this.toClassName(json.name);

    if (this.hasSingleton(json)) {
      return `TK_SET_NULL`;
    } else if (this.hasCreate(json)) {
      return `${clsName}_destroy`;
    } else {
      return `TKMEM_FREE`;
    }
  }

  genSetProp(json, prop) {
    let result = '';
    const clsName = this.toClassName(json.name);
    const objName = this.toObjName(json.name);
    let value = this.genFromValue(prop.type, prop.name);

    if (this.hasSetterFor(json, prop.name)) {
      result += `${clsName}_set_${prop.name}(${objName}, ${value});\n`
    } else if (this.isWritable(prop)) {
      if (prop.type.indexOf('char*') >= 0) {
        result += `${objName}->${prop.name} = tk_str_copy(${objName}->${prop.name}, ${value});\n`
      } else if (prop.type === 'str_t') {
        result += `str_set(&(${objName}->${prop.name}), ${value});\n`

      } else {
        result += `${objName}->${prop.name} = ${value};\n`
      }
    } else {
      result = '';
    }

    return result;
  }

  genSetPropDispatch(json) {
    const clsName = this.toClassName(json.name);
    const props = json.props || json.properties || [];
    let result = props.map((iter, index) => {
      let setProp = '';
      if (index) {
        setProp = `\n  } else if (tk_str_eq("${iter.name}", name)) {\n`
      } else {
        setProp = `  if (tk_str_eq("${iter.name}", name)) {\n`
      }
      setProp += `     ${this.genSetProp(json, iter)}\n`;
      setProp += '     return RET_OK;';
      return setProp;
    }).join('');

    result += '\n  }';

    return result;
  }

  genCanExec(json, cmd) {
    const clsName = this.toClassName(json.name);
    const objName = this.toObjName(json.name);
    const cmdName = cmd.name.replace(`${clsName}_`, '');
    const name = `${clsName}_can_${cmdName}`;

    cmd = this.findMethod(json, name);
    if (cmd) {
      if (cmd.params.length == 1) {
        return `${name}(${objName});`;
      } else if (cmd.params.length == 2) {
        let type = cmd.params[1].type;
        let args = this.genCmdArg(type);
        return `${name}(${objName}, ${args});`;
      } else {
        return 'TRUE;';
      }
    } else {
      return 'TRUE;';
    }
  }

  genCanExecDispatch(json) {
    const clsName = this.toClassName(json.name);
    const objName = this.toObjName(json.name);
    let commands = json.commands || json.methods || [];
    commands = commands.filter(iter => this.isCommand(iter));

    if (commands.length > 0) {
      let result = ` 
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  ${clsName}_t* ${objName} = vm->${objName};
`;

      result += commands.map((iter, index) => {
        let exec = '';
        const cmdName = iter.name.replace(`${clsName}_`, '');

        if (index) {
          exec = `\n  } else if (tk_str_eq("${cmdName}", name)) {\n`
        } else {
          exec = `  if (tk_str_eq("${cmdName}", name)) {\n`
        }
        exec += `    return ${this.genCanExec(json, iter)}\n`;
        return exec;
      }).join('');

      result += '  }';

      return result;
    } else {
      return '';
    }
  }

  genCmdArg(type) {
    let args = 'args'
    if (type.indexOf('int') >= 0) {
      args = 'tk_atoi(args)';
    } else if (type.indexOf('float') >= 0) {
      args = 'tk_atof(args)';
    } else if (type.indexOf('bool') >= 0) {
      args = 'tk_atob(args)';
    }

    return args;
  }

  genExec(json, cmd) {
    const clsName = this.toClassName(json.name);
    const objName = this.toObjName(json.name);
    if (cmd.params.length == 1) {
      return `${cmd.name}(${objName});`;
    } else if (cmd.params.length == 2) {
      let type = cmd.params[1].type;
      let args = this.genCmdArg(type);
      return `${cmd.name}(${objName}, ${args});`;
    } else {
      return 'RET_FAIL;';
    }
  }

  genExecDispatch(json) {
    const clsName = this.toClassName(json.name);
    const objName = this.toObjName(json.name);
    let commands = json.commands || json.methods || [];
    commands = commands.filter(iter => this.isCommand(iter));

    if (commands.length > 0) {
      let result = ` 
  ${clsName}_view_model_t* vm = (${clsName}_view_model_t*)(obj);
  ${clsName}_t* ${objName} = vm->${objName};
`;
      result += commands.map((iter, index) => {
        let exec = '';
        const cmdName = iter.name.replace(`${clsName}_`, '');

        if (index) {
          exec = `\n  } else if (tk_str_eq("${cmdName}", name)) {\n`
        } else {
          exec = `  if (tk_str_eq("${cmdName}", name)) {\n`
        }
        exec += `    return ${this.genExec(json, iter)}\n`;
        return exec;
      }).join('');

      result += '  }';

      return result;
    } else {
      return '';
    }
  }

  genGetProp(json, prop) {
    let value = '';
    const clsName = this.toClassName(json.name);
    const objName = this.toObjName(json.name);

    if (this.hasGetterFor(json, prop.name)) {
      value = `${clsName}_get_${prop.name}(${objName})`
    } else if (this.isReadable(prop)) {
      if (prop.type === 'str_t') {
        value = `${objName}->${prop.name}.str`
      } else {
        value = `${objName}->${prop.name}`
      }
    } else {
      return '';
    }

    return this.genToValue(prop.type, value);
  }

  genGetPropDispatch(json) {
    const clsName = this.toClassName(json.name);
    const props = json.props || json.properties || [];

    let result = props.map((iter, index) => {
      let getProp = '';
      if (index) {
        getProp = `\n  } else if (tk_str_eq("${iter.name}", name)) {\n`
      } else {
        getProp = `  if (tk_str_eq("${iter.name}", name)) {\n`
      }
      getProp += `     ${this.genGetProp(json, iter)}\n`;
      getProp += '     return RET_OK;';
      return getProp;
    }).join('');

    result += '\n  }';

    return result;
  }
  
  genOffEvents(json) {
    let result = '';
    if(json.parent === 'emitter_t' || json.parent === 'object_t') {
      let objName = this.toObjName(json.name);
      result += `emitter_off_by_ctx(EMITTER(vm->${objName}), vm);`
    }

    return result;
  }

  genForwardEvents(json) {
    let result = '';
    if(json.parent === 'emitter_t' || json.parent === 'object_t') {
      let clsName = this.toClassName(json.name);
      result += `
  emitter_on(EMITTER(${clsName}), EVT_PROPS_CHANGED, emitter_forward, vm);
  emitter_on(EMITTER(${clsName}), EVT_ITEMS_CHANGED, emitter_forward, vm);
`
    }

    return result;
  }

  genFile(filename) {
    let json = JSON.parse(fs.readFileSync(filename).toString());

    this.genJson(this.filter(json));
  }
}

module.exports = CodeGen;
