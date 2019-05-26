const fs = require('fs')
const path = require('path')
const utils = require('./utils')

class CodeGen {
  genHeader(json) {
    let clsName = json.name;
    let propsDecl = utils.genPropDecls(json);
    let clsNameUpper = clsName.toUpperCase();

    let result =
      `
#ifndef TK_${clsNameUpper}_H
#define TK_${clsNameUpper}_H

#include "tkc/darray.h"
#include "mvvm/base/view_model_array.h"

BEGIN_C_DECLS

/**
 * @class ${clsName}_t
 *
 * ${json.desc || ''}
 *
 */
typedef struct _${clsName}_t {
${propsDecl}
} ${clsName}_t;

/**
 * @class ${clsName}s_view_model_t
 *
 * view model of ${json.name}
 *
 */
typedef struct _${clsName}s_view_model_t {
  view_model_array_t view_model_array;

  /*model object*/
  darray_t ${clsName}s;
} ${clsName}s_view_model_t;

/**
 * @method ${clsName}s_view_model_create
 * 创建${clsName} view model对象。
 *
 * @annotation ["constructor"]
 * @param {navigator_request_t*} req 请求参数。
 *
 * @return {view_model_t} 返回view_model_t对象。
 */
view_model_t* ${clsName}s_view_model_create(navigator_request_t* req);

/*public for test*/

${clsName}_t* ${clsName}_create(void);

ret_t ${clsName}s_view_model_clear(view_model_t* view_model);
uint32_t ${clsName}s_view_model_size(view_model_t* view_model);
ret_t ${clsName}s_view_model_remove(view_model_t* view_model, uint32_t index);
ret_t ${clsName}s_view_model_add(view_model_t* view_model, ${clsName}_t* ${clsName});
${clsName}_t* ${clsName}s_view_model_get(view_model_t* view_model, uint32_t index);

END_C_DECLS

#endif /*TK_${clsNameUpper}_H*/
`
    return result;
  }

  genGetProps(json) {
    const clsName = json.name;
    const dispatch = utils.genGetPropsDispatch(json);

    const result =
      `
static ret_t ${clsName}s_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  ${clsName}_t* ${clsName} = NULL;
  view_model_t* vm = VIEW_MODEL(obj);

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, ${clsName}s_view_model_size(VIEW_MODEL(obj)));

    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  ${clsName} = ${clsName}s_view_model_get(vm, index);
  return_value_if_fail(${clsName} != NULL, RET_BAD_PARAMS);

${dispatch}
  } else if (tk_str_eq("style", name)) {
    value_set_str(v, index % 2 ? "odd" : "even");
  } else {
    log_debug("not found %s\\n", name);
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}

`
    return result;
  }

  genSetProps(json) {
    const clsName = json.name;
    const dispatch = utils.genSetPropsDispatch(json);

    const result =
      `
static ret_t ${clsName}s_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  ${clsName}_t* ${clsName} = NULL;
  view_model_t* vm = VIEW_MODEL(obj);

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(vm, value_int(v));

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  ${clsName} = ${clsName}s_view_model_get(vm, index);
  return_value_if_fail(${clsName} != NULL, RET_BAD_PARAMS);

${dispatch}
  } else {
    log_debug("not found %s\\n", name);
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
      let str = '  } else if (';
      str += `tk_str_eq("${cmdName}", name)) {\n`
      str += `    return ${clsName}_${cmdName}(${clsName}, args);`;
      return str;
    }).join('\n');

    const result =
      `
static ret_t ${clsName}s_view_model_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* vm = VIEW_MODEL(obj);
  ${clsName}_t* ${clsName} = NULL;

  if (tk_str_ieq(name, "add")) {
    ENSURE(${clsName}s_view_model_add(vm, ${clsName}_create()) == RET_OK);
    return RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, "clear")) {
    ENSURE(${clsName}s_view_model_clear(vm) == RET_OK);
    return RET_ITEMS_CHANGED;
  }

  ${clsName} = ${clsName}s_view_model_get(vm, index);
  return_value_if_fail(${clsName} != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, "remove")) {
    ENSURE(${clsName}s_view_model_remove(vm, index) == RET_OK);
    return RET_ITEMS_CHANGED;
${dispatch}
  } else {
    log_debug("not found %s\\\n", name);
    return RET_NOT_FOUND;
  }
}
`
    return result;
  }

  genCanExec(json) {
    const clsName = json.name;
    const dispatch = json.cmds.map((cmd, index) => {
      const cmdName = cmd.name;
      let str = '  } else if (';
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
static bool_t ${clsName}s_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* vm = VIEW_MODEL(obj);
  ${clsName}_t* ${clsName} = NULL;

  if (tk_str_ieq(name, "add")) {
    return TRUE;
  } else if (tk_str_ieq(name, "clear")) {
    return ${clsName}s_view_model_size(vm) > 0;
  }

  ${clsName} = ${clsName}s_view_model_get(vm, index);
  return_value_if_fail(${clsName} != NULL, FALSE);

  if (tk_str_ieq(name, "remove")) {
    return index < ${clsName}s_view_model_size(vm);
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
static ret_t ${clsName}s_view_model_on_destroy(object_t* obj) {
  ${clsName}s_view_model_t* vm = (${clsName}s_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);
  
  ${clsName}s_view_model_clear(VIEW_MODEL(obj));
  darray_deinit(&(vm->${clsName}s));

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_${clsName}s_view_model_vtable = {
  .type = "${clsName}",
  .desc = "${clsDesc}",
  .is_collection = TRUE,
  .size = sizeof(${clsName}s_view_model_t),
  .exec = ${clsName}s_view_model_exec,
  .can_exec = ${clsName}s_view_model_can_exec,
  .get_prop = ${clsName}s_view_model_get_prop,
  .set_prop = ${clsName}s_view_model_set_prop,
  .on_destroy = ${clsName}s_view_model_on_destroy
};

view_model_t* ${clsName}s_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_${clsName}s_view_model_vtable);
  view_model_t* vm = view_model_array_init(VIEW_MODEL(obj));
  ${clsName}s_view_model_t* ${clsName}_vm = (${clsName}s_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  darray_init(&(${clsName}_vm->${clsName}s), 100, 
    (tk_destroy_t)${clsName}_destroy, (tk_compare_t)${clsName}_cmp);

  return vm;
}
`
    return result;
  }

  genContent(json) {
    const clsName = json.name;
    let result = utils.genModelCommonFuncs(json);

    if (json.props && json.props.length) {
      result += utils.genPropFuncs(json);
    }

    if (json.cmds && json.cmds.length) {
      result += utils.genCmdFuncs(json);
    }

    result += `
/***************${clsName}s_view_model***************/

ret_t ${clsName}s_view_model_remove(view_model_t* view_model, uint32_t index) {
  ${clsName}s_view_model_t* ${clsName}_vm = (${clsName}s_view_model_t*)(view_model);
  return_value_if_fail(${clsName}_vm != NULL, RET_BAD_PARAMS);

  return darray_remove_index(&(${clsName}_vm->${clsName}s), index);
}

ret_t ${clsName}s_view_model_add(view_model_t* view_model, ${clsName}_t* ${clsName}) {
  ${clsName}s_view_model_t* ${clsName}_vm = (${clsName}s_view_model_t*)(view_model);
  return_value_if_fail(${clsName}_vm != NULL && ${clsName} != NULL, RET_BAD_PARAMS);

  return darray_push(&(${clsName}_vm->${clsName}s), ${clsName});
}

uint32_t ${clsName}s_view_model_size(view_model_t* view_model) {
  ${clsName}s_view_model_t* ${clsName}_vm = (${clsName}s_view_model_t*)(view_model);
  return_value_if_fail(${clsName}_vm != NULL, 0); 

  return ${clsName}_vm->${clsName}s.size;
}

ret_t ${clsName}s_view_model_clear(view_model_t* view_model) {
  ${clsName}s_view_model_t* ${clsName}_vm = (${clsName}s_view_model_t*)(view_model);
  return_value_if_fail(${clsName}_vm != NULL, 0); 

  return darray_clear(&(${clsName}_vm->${clsName}s));
}

${clsName}_t* ${clsName}s_view_model_get(view_model_t* view_model, uint32_t index) {
  ${clsName}s_view_model_t* ${clsName}_vm = (${clsName}s_view_model_t*)(view_model);
  return_value_if_fail(${clsName}_vm != NULL, 0); 

  return_value_if_fail(${clsName}_vm != NULL && index < ${clsName}s_view_model_size(view_model), NULL);

  return (${clsName}_t*)(${clsName}_vm->${clsName}s.elms[index]);
}
`
    result += this.genSetProps(json);
    result += this.genGetProps(json);

    result += this.genCanExec(json);
    result += this.genExec(json);

    result += this.genVTable(json);

    return result;
  }

  genJson(json) {
    utils.saveResult(json.name+'s', json.includes, this.genHeader(json), this.genContent(json));
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
