const { test } = require('node:test');
const assert = require('node:assert/strict');
const { spawnSync } = require('node:child_process');
const fs = require('fs');
const os = require('os');
const path = require('path');

const genDir = __dirname;

function generate(script, idl) {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'awtk-mvvm-gen-'));
  const idlPath = path.join(dir, 'idl.json');
  fs.writeFileSync(idlPath, JSON.stringify(idl));
  const result = spawnSync(process.execPath, [path.join(genDir, script), idlPath], {
    cwd: dir,
    encoding: 'utf8',
  });
  assert.equal(result.status, 0, result.stderr || result.stdout);
  return dir;
}

const objectModel = [
  {
    type: 'class',
    methods: [],
    properties: [],
    header: 'temperature.h',
    name: 'temperature_t',
    annotation: { model: true },
  },
];

const collectionModel = [
  {
    type: 'class',
    methods: [],
    properties: [],
    header: 'book_store.h',
    name: 'book_store_t',
    annotation: { model: true, collection: 'book_t' },
  },
];

const cppObjectModel = [
  {
    type: 'class',
    methods: [],
    properties: [{ name: 'value', type: 'float' }],
    header: 'temperature.hpp',
    name: 'Temperature',
    annotation: { model: true, cpp: true },
  },
];

const cppCollectionModel = [
  {
    type: 'class',
    methods: [],
    properties: [],
    header: 'book_store.hpp',
    name: 'BookStore',
    annotation: { model: true, cpp: true, collection: 'Book' },
  },
];

test('collection view model vtable initializes copy_props and exec_ex', () => {
  const dir = generate('gen_vm_array.js', collectionModel);
  const source = fs.readFileSync(path.join(dir, 'book_store_view_model.c'), 'utf8');
  const vtable = source.slice(source.indexOf('static const object_vtable_t'));

  assert.match(
    vtable,
    /\.set_prop = book_store_view_model_set_prop,\s*\.copy_props = NULL,\s*\.remove_prop = NULL,/,
  );
  assert.match(
    vtable,
    /\.exec = book_store_view_model_exec,\s*\.exec_ex = NULL,\s*\.clone = NULL/,
  );
  assert.match(vtable, /\.is_collection = TRUE,/);
});

test('object view model vtable initializes copy_props and exec_ex', () => {
  const dir = generate('gen_vm.js', objectModel);
  const source = fs.readFileSync(path.join(dir, 'temperature_view_model.c'), 'utf8');
  const vtable = source.slice(source.indexOf('static const object_vtable_t'));

  assert.match(
    vtable,
    /\.set_prop = temperature_view_model_set_prop,\s*\.copy_props = NULL,\s*\.remove_prop = NULL,/,
  );
  assert.match(
    vtable,
    /\.exec = temperature_view_model_exec,\s*\.exec_ex = NULL,\s*\.clone = NULL/,
  );
});

function extractFunction(source, name) {
  const start = source.indexOf(`static ret_t ${name}(`);
  assert.notEqual(start, -1, `missing function ${name}`);
  const brace = source.indexOf('{', start);
  let depth = 0;
  for (let i = brace; i < source.length; i++) {
    if (source[i] === '{') depth++;
    if (source[i] === '}') {
      depth--;
      if (depth === 0) {
        return source.slice(start, i + 1);
      }
    }
  }
  assert.fail(`unbalanced braces in ${name}`);
}

test('object view model with no properties does not emit orphan braces in get/set_prop', () => {
  const dir = generate('gen_vm.js', objectModel);
  const source = fs.readFileSync(path.join(dir, 'temperature_view_model.c'), 'utf8');
  const setProp = extractFunction(source, 'temperature_view_model_set_prop');
  const getProp = extractFunction(source, 'temperature_view_model_get_prop');

  assert.match(setProp, /atemperature = .+;\s*return RET_NOT_FOUND;\s*}/);
  assert.match(getProp, /atemperature = .+;\s*return RET_NOT_FOUND;\s*}/);
  assert.doesNotMatch(setProp, /;\s*}\s*return RET_NOT_FOUND/);
  assert.doesNotMatch(getProp, /;\s*}\s*return RET_NOT_FOUND/);
});

test('collection view model with no properties does not emit orphan braces in get/set_prop', () => {
  const dir = generate('gen_vm_array.js', collectionModel);
  const source = fs.readFileSync(path.join(dir, 'book_store_view_model.c'), 'utf8');
  const setProp = extractFunction(source, 'book_store_view_model_set_prop');
  const getProp = extractFunction(source, 'book_store_view_model_get_prop');

  assert.match(setProp, /return RET_OK;\s*}\s*name = tk_destruct_array_prop_name/);
  assert.match(getProp, /return RET_OK;\s*}\s*name = tk_destruct_array_prop_name/);
  assert.doesNotMatch(setProp, /return RET_OK;\s*}\s*}\s*name = tk_destruct_array_prop_name/);
  assert.doesNotMatch(getProp, /return RET_OK;\s*}\s*}\s*name = tk_destruct_array_prop_name/);
});

test('cpp object view model vtable uses inheritance instead of designated initializers', () => {
  const dir = generate('gen_vm.js', cppObjectModel);
  const source = fs.readFileSync(path.join(dir, 'temperature_view_model.cpp'), 'utf8');

  assert.match(
    source,
    /static const struct temperature_view_model_vtable_t\s*:\s*object_vtable_t\s*\{/,
  );
  assert.match(source, /temperature_view_model_vtable_t\(\)\s*\{/);
  assert.match(source, /copy_props\s*=\s*NULL;/);
  assert.match(source, /exec_ex\s*=\s*NULL;/);
  assert.doesNotMatch(source, /\.type\s*=\s*"temperature_view_model_t"/);
  assert.match(source, /\}\s*s_temperature_view_model_vtable;/);
});

test('cpp collection view model vtable uses inheritance instead of designated initializers', () => {
  const dir = generate('gen_vm_array.js', cppCollectionModel);
  const source = fs.readFileSync(path.join(dir, 'book_store_view_model.cpp'), 'utf8');

  assert.match(
    source,
    /static const struct book_store_view_model_vtable_t\s*:\s*object_vtable_t\s*\{/,
  );
  assert.match(source, /is_collection\s*=\s*TRUE;/);
  assert.doesNotMatch(source, /\.type\s*=\s*"book_store_view_model_t"/);
});
