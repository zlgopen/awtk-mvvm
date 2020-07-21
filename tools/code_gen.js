const fs = require('fs')
const path = require('path')

class CodeGen {
  isModel(obj) {
    return obj.annotation && obj.annotation.model;
  }
  
  isCpp(obj) {
    return obj.annotation && obj.annotation.cpp;
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

  underLineName(name) {
    name = name.replace(/[A-Z]/g, r => {
      r = '_' + r;
      return r.toLowerCase();
    }); 
    name = name.replace(/^_/, '');

    return name;
  }

  upperCamelName(name) {
    name = name.replace(/_t$/, '');
    name = name.replace(/(^|_)[a-z]/g, r => {
      if (r.length > 1) {
        r = r.substr(1);
      }   

      return r.toUpperCase();
    }); 

    return name;
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

  toSetterName(cls, name) {
    let clsName = this.toClassName(cls.name);
    if(this.isCpp(cls)) {
      return this.upperCamelName('set_' + name);
    } else {
      return clsName + '_set_' + name;
    }
  }
  
  toCanExecName(cls, name) {
    let clsName = this.toClassName(cls.name);
    if(this.isCpp(cls)) {
      return 'Can' + name;
    } else {
      return clsName + '_can_' + name;
    }
  }
  
  toGetterName(cls, name) {
    let clsName = this.toClassName(cls.name);
    if(this.isCpp(cls)) {
      return this.upperCamelName('get_' + name);
    } else {
      return clsName + '_get_' + name;
    }
  }

  hasSetterFor(cls, name) {
    let methodName = this.toSetterName(cls, name); 

    return cls.methods.find(iter => (iter.name === methodName));
  }

  hasGetterFor(cls, name) {
    let methodName = this.toGetterName(cls, name); 

    return cls.methods.find(iter => (iter.name === methodName));
  }

  genIncludes(name) {
    let result = `#include "tkc/mem.h"\n`;
    result += `#include "tkc/utils.h"\n`;
    result += `#include "${name.toLowerCase()}.h"`;

    return result;
  }

  saveResult(name, cpp, includes, header, content) {
    if(cpp) {
      fs.writeFileSync(`${name}.h`, "\ufeff" + header);
      fs.writeFileSync(`${name}.cpp`, "\ufeff" + content);
      console.log(`output to ${name}.h and ${name}.cpp`);
    } else {
      fs.writeFileSync(`${name}.h`, "\ufeff" + header);
      fs.writeFileSync(`${name}.c`, "\ufeff" + content);
      console.log(`output to ${name}.h and ${name}.c`);
    }
  }

  genJson(json) {
    console.log('-----------------------------------------------------');
    json.forEach(iter => {
      let includes = iter.includes;
      let header = this.genHeader(iter);
      let content = this.genContent(iter);
      let name = this.toViewModelClassName(iter.name);
      this.saveResult(name, this.isCpp(iter), includes, header, content);
    });
  }

  toClassName(name) {
    return name.replace(/_t$/, '');
  }
  
  toObjName(name) {
    return 'a'+name.replace(/_t$/, '');
  }
  
  toViewModelClassName(name) {
    let clsName = this.underLineName(this.toClassName(name));

    return `${clsName}_view_model`
  }
  
  toViewModelClassType(name) {
    let clsName = this.underLineName(this.toClassName(name));

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
        } else if (type.indexOf('string') >= 0) {
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
        }else if (type.indexOf('string') >= 0) {
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

  genConstructorC(json) {
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
  
  genConstructorCPP(json) {
    const info = this.findMethod(json, json.name);
    if(info && info.params.length == 1) {
      return `new ${json.name}(req)`;
    }
    return `new ${json.name}()`;
  }
  
  genConstructor(json) {
    if(this.isCpp(json)) {
      return this.genConstructorCPP(json);
    } else {
      return this.genConstructorC(json);
    }
  }

  genDestructorC(json) {
    const clsName = this.toClassName(json.name);

    if (this.hasSingleton(json)) {
      return `TK_SET_NULL`;
    } else if (this.hasCreate(json)) {
      return `${clsName}_destroy`;
    } else {
      return `TKMEM_FREE`;
    }
  }
  
  genDestructorCPP(json) {
    return `delete `;
  }

  genDestructor(json) {
    if(this.isCpp(json)) {
      return this.genDestructorCPP(json);
    } else {
      return this.genDestructorC(json);
    }
  }
  
  genGetProp(cls, prop) {
    let value = '';
    const clsName = this.toClassName(cls.name);
    const objName = this.toObjName(cls.name);
    const isCpp = this.isCpp(cls);

    if (this.hasGetterFor(cls, prop.name)) {
      const getter = this.toGetterName(cls, prop.name);
      if(isCpp) {
        value = `${objName}->${getter}()`
      } else {
        value = `${getter}(${objName})`
      }
    } else if (this.isReadable(prop)) {
      if (prop.type === 'str_t') {
        value = `${objName}->${prop.name}.str`
      } else if (prop.type.indexOf('string') >= 0) {
        value = `${objName}->${prop.name}.c_str()`
      } else {
        value = `${objName}->${prop.name}`
      }
    } else {
      return '';
    }

    return this.genToValue(prop.type, value);
  }

  genGetPropDispatch(cls) {
    const clsName = this.toClassName(cls.name);
    const props = cls.props || cls.properties || [];

    let result = props.map((iter, index) => {
      let getProp = '';
      if (index) {
        getProp = `\n  } else if (tk_str_ieq("${iter.name}", name)) {\n`
      } else {
        getProp = `  if (tk_str_ieq("${iter.name}", name)) {\n`
      }
      getProp += `     ${this.genGetProp(cls, iter)}\n`;
      getProp += '     return RET_OK;';
      return getProp;
    }).join('');

    result += '\n  }';

    return result;
  }
  
  genSetProp(cls, prop) {
    let result = '';
    const clsName = this.toClassName(cls.name);
    const objName = this.toObjName(cls.name);
    const isCpp = this.isCpp(cls);
    let value = this.genFromValue(prop.type, prop.name);

    if (this.hasSetterFor(cls, prop.name)) {
      const setter = this.toSetterName(cls, prop.name);
      if(isCpp) {
        result += `${objName}->${setter}(${value});\n`
      } else {
        result += `${setter}(${objName}, ${value});\n`
      }
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

  genSetPropDispatch(cls) {
    const clsName = this.toClassName(cls.name);
    const props = cls.props || cls.properties || [];
    let result = props.map((iter, index) => {
      let setProp = '';
      if (index) {
        setProp = `\n  } else if (tk_str_ieq("${iter.name}", name)) {\n`
      } else {
        setProp = `  if (tk_str_ieq("${iter.name}", name)) {\n`
      }
      setProp += `     ${this.genSetProp(cls, iter)}\n`;
      setProp += '     return RET_OK;';
      return setProp;
    }).join('');

    result += '\n  }';

    return result;
  }

  genCanExec(cls, cmd) {
    const clsName = this.toClassName(cls.name);
    const objName = this.toObjName(cls.name);
    const cmdName = cmd.name.replace(`${clsName}_`, '');
    const name = this.toCanExecName(cls, cmdName);
    const isCpp = this.isCpp(cls);

    cmd = this.findMethod(cls, name);
    if (cmd) {
      if (cmd.params.length == 1) {
        if(isCpp) {
          let type = cmd.params[0].type;
          let args = this.genCmdArg(type);
          return `${objName}->${name}(${args});`;
        } else {
          return `${name}(${objName});`;
        }
      } else if (cmd.params.length == 2) {
        let type = cmd.params[1].type;
        let args = this.genCmdArg(type);
        return `${name}(${objName}, ${args});`;
      } else if (cmd.params.length == 0 && isCpp) {
        return `${objName}->${name}();`;
      } else {
        return 'TRUE;';
      }
    } else {
      return 'TRUE;';
    }
  }

  genCanExecDispatch(cls) {
    const clsType = cls.name;
    const clsName = this.toClassName(cls.name);
    const objName = this.toObjName(cls.name);
    const vmClsName = this.toViewModelClassName(cls.name);
    const vmClsType = this.toViewModelClassType(cls.name);

    let commands = cls.commands || cls.methods || [];
    commands = commands.filter(iter => this.isCommand(iter));

    if (commands.length > 0) {
      let result = ` 
  ${vmClsType}* vm = (${vmClsType}*)(obj);
  ${clsType}* ${objName} = vm->${objName};
`;

      result += commands.map((iter, index) => {
        let exec = '';
        let cmdName = iter.name.replace(`${clsName}_`, '');
        if(typeof(iter.annotation.command) === 'string') {
          cmdName = iter.annotation.command;
        }

        if (index) {
          exec = `\n  } else if (tk_str_ieq("${cmdName}", name)) {\n`
        } else {
          exec = `  if (tk_str_ieq("${cmdName}", name)) {\n`
        }
        exec += `    return ${this.genCanExec(cls, iter)}\n`;
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
    } else if (type == 'char') {
      args = 'args[0]';
    } else if (type.indexOf('bool') >= 0) {
      args = 'tk_atob(args)';
    }

    return args;
  }

  genExec(cls, cmd) {
    const clsName = this.toClassName(cls.name);
    const objName = this.toObjName(cls.name);
    const name = cmd.name;
    const isCpp = this.isCpp(cls);

    if (cmd.params.length == 1) {
      if(isCpp) {
      let type = cmd.params[0].type;
      let args = this.genCmdArg(type);
        return `${objName}->${name}(${args});`;
      } else {
        return `${cmd.name}(${objName});`;
      }
    } else if (cmd.params.length == 2) {
      let type = cmd.params[1].type;
      let args = this.genCmdArg(type);
      return `${cmd.name}(${objName}, ${args});`;
    } else if (cmd.params.length == 0 && isCpp) {
      return `${objName}->${name}();`;
    } else {
      return 'RET_FAIL;';
    }
  }

  genExecDispatch(cls) {
    const clsType = cls.name;
    const clsName = this.toClassName(cls.name);
    const objName = this.toObjName(cls.name);
    const vmClsName = this.toViewModelClassName(cls.name);
    const vmClsType = this.toViewModelClassType(cls.name);
    let commands = cls.commands || cls.methods || [];
    commands = commands.filter(iter => this.isCommand(iter));

    if (commands.length > 0) {
      let result = ` 
  ${vmClsType}* vm = (${vmClsType}*)(obj);
  ${clsType}* ${objName} = vm->${objName};
`;
      result += commands.map((iter, index) => {
        let exec = '';
        let cmdName = iter.name.replace(`${clsName}_`, '');

        if(typeof(iter.annotation.command) === 'string') {
          cmdName = iter.annotation.command;
        }

        if (index) {
          exec = `\n  } else if (tk_str_ieq("${cmdName}", name)) {\n`
        } else {
          exec = `  if (tk_str_ieq("${cmdName}", name)) {\n`
        }
        exec += `    return ${this.genExec(cls, iter)}\n`;
        return exec;
      }).join('');

      result += '  }';

      return result;
    } else {
      return '';
    }
  }

  genOffEvents(cls) {
    let result = '';
    if(cls.parent === 'emitter_t' || cls.parent === 'object_t') {
      let objName = this.toObjName(cls.name);
      result += `emitter_off_by_ctx(EMITTER(vm->${objName}), vm);`
    }

    return result;
  }

  genForwardEvents(cls) {
    let result = '';
    if(cls.parent === 'emitter_t' || cls.parent === 'object_t') {
      let objName = this.toObjName(cls.name);
      result += `
  emitter_on(EMITTER(${objName}), EVT_PROPS_CHANGED, (event_func_t)emitter_dispatch, vm);
  emitter_on(EMITTER(${objName}), EVT_ITEMS_CHANGED, (event_func_t)emitter_dispatch, vm);
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
