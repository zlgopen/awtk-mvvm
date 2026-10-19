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
