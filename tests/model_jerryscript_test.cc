#include "mvvm/jerryscript/model_jerryscript.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ModelJerryScript, get_prop) {
  const char* code = "var test = {a:1, b:true, name:'awtk'};";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_get_prop_int(obj, "a", 0), 1);
  ASSERT_EQ(object_get_prop_int(obj, "b", 0), 1);
  ASSERT_EQ(object_get_prop_int(obj, "c", 0), 0);
  ASSERT_EQ(string(object_get_prop_str(obj, "name")), string("awtk"));

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, get_prop_global) {
  const char* code = "var a=1; var b = true; var name ='awtk';";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_get_prop_int(obj, "a", 0), 1);
  ASSERT_EQ(object_get_prop_int(obj, "b", 0), 1);
  ASSERT_EQ(object_get_prop_int(obj, "c", 0), 0);
  ASSERT_EQ(string(object_get_prop_str(obj, "name")), string("awtk"));

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, set_prop) {
  const char* code = "var test = {a:1, b:true, name:'awtk'};";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_set_prop_int(obj, "a", 11), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "b", 22), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "c", 33), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "age", 88), RET_OK);
  ASSERT_EQ(object_set_prop_str(obj, "name", "zlg"), RET_OK);

  ASSERT_EQ(object_get_prop_int(obj, "a", 0), 11);
  ASSERT_EQ(object_get_prop_int(obj, "b", 0), 22);
  ASSERT_EQ(object_get_prop_int(obj, "c", 0), 33);
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 88);
  ASSERT_EQ(string(object_get_prop_str(obj, "name")), string("zlg"));

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, set_prop_global) {
  const char* code = "var a=1; var b = true; var name ='awtk';";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_set_prop_int(obj, "a", 11), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "b", 22), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "c", 33), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "age", 88), RET_OK);
  ASSERT_EQ(object_set_prop_str(obj, "name", "zlg"), RET_OK);

  ASSERT_EQ(object_get_prop_int(obj, "a", 0), 11);
  ASSERT_EQ(object_get_prop_int(obj, "b", 0), 22);
  ASSERT_EQ(object_get_prop_int(obj, "c", 0), 33);
  ASSERT_EQ(object_get_prop_int(obj, "age", 0), 88);
  ASSERT_EQ(string(object_get_prop_str(obj, "name")), string("zlg"));

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, exec) {
  const char* code =
      "var test = {save: function(args) {print(args); return 0;}, canSave: function(args) "
      "{print(args); return true;}};";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_can_exec(obj, "save", "awtk\n"), TRUE);
  ASSERT_EQ(object_exec(obj, "save", "awtk\n"), RET_OK);

  ASSERT_EQ(object_can_exec(obj, "load", "awtk\n"), FALSE);
  ASSERT_NE(object_exec(obj, "load", "awtk\n"), RET_OK);

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, exec_global) {
  const char* code =
      "function save(args) {print(args); return 0;}\nfunction canSave(args) {print(args); return "
      "true;}";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_can_exec(obj, "save", "awtk\n"), TRUE);
  ASSERT_EQ(object_exec(obj, "save", "awtk\n"), RET_OK);

  ASSERT_EQ(object_can_exec(obj, "load", "awtk\n"), FALSE);
  ASSERT_NE(object_exec(obj, "load", "awtk\n"), RET_OK);

  object_unref(OBJECT(model));
}

static ret_t on_model_changed(void* ctx, event_t* e) {
  int* n = (int*)ctx;

  *n = *n + 1;
  log_debug("on_model_changed\n");

  return RET_OK;
}

TEST(ModelJerryScript, notifyPropsChanged) {
  int n = 0;
  const char* code =
      "var test = {save: function(args) {print(args); test.count++; "
      "this.notifyPropsChanged();return 0;}, count:0}";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, on_model_changed, &n);

  ASSERT_EQ(object_can_exec(obj, "save", "awtk\n"), TRUE);
  ASSERT_EQ(object_exec(obj, "save", "awtk\n"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 1);
  ASSERT_EQ(n, 1);

  ASSERT_EQ(object_exec(obj, "save", "awtk\n"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 2);
  ASSERT_EQ(n, 2);

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, onMount) {
  const char* code =
      "var test = {onMount: function(args) {print('onMount js'); test.count++; "
      "this.notifyPropsChanged();return 0;}, count:0}";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(model_on_mount(model), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 1);

  ASSERT_EQ(model_on_mount(model), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 2);

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, onWillUnmount) {
  const char* code =
      "var test = {onWillUnmount: function(args) {print('onWillUnmount js'); test.count++; "
      "this.notifyPropsChanged();return 0;}, count:0}";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(model_on_will_unmount(model), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 1);

  ASSERT_EQ(model_on_will_unmount(model), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 2);

  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, onUnmount) {
  const char* code =
      "var test = {onUnmount: function(args) {print('onUnmount js'); test.count++; "
      "this.notifyPropsChanged();return 0;}, count:0}";
  model_t* model = model_jerryscript_create("test", code, strlen(code), NULL);
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(model_on_unmount(model), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 1);

  ASSERT_EQ(model_on_unmount(model), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 2);

  object_unref(OBJECT(model));
}

static ret_t dummy_on_result(navigator_request_t* req, const value_t* result) {
  value_copy(&(req->result), result);

  return RET_OK;
}

TEST(ModelJerryScript, onWillmount) {
  const char* code =
      "var test = {onWillMount: function(req) {print('onWillMount js'); test.count = req.count; "
      "this.name = req.name;"
      "this.notifyPropsChanged();"
      "print('xxxxxxxxxxx:' + req.onResult);"
      "req.onResult(req.count);"
      "return 0;}, count:0}";
  navigator_request_t* req = navigator_request_create("test", dummy_on_result);
  object_set_prop_int(OBJECT(req), "count", 100);
  object_set_prop_str(OBJECT(req), "name", "abc");
  model_t* model = model_jerryscript_create("test", code, strlen(code), req);

  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(model_on_will_mount(model, req), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 100);
  ASSERT_EQ(string(object_get_prop_str(obj, "name")), "abc");
  ASSERT_EQ(value_int(&(req->result)), 100);

  object_unref(OBJECT(req));
  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, create1) {
  const char* code =
      "function Test1(req) {this.count=req.count}"
      "function createTest1(req) {return new Test1(req);}";

  navigator_request_t* req = navigator_request_create("test1", dummy_on_result);
  object_set_prop_int(OBJECT(req), "count", 100);
  object_set_prop_str(OBJECT(req), "name", "abc");
  model_t* model = model_jerryscript_create("test1", code, strlen(code), req);

  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 100);

  object_unref(OBJECT(req));
  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, create2) {
  const char* code =
      "function Test2(req) {this.count=req.count}"
      "function create_test2(req) {return new Test2(req);}";

  navigator_request_t* req = navigator_request_create("test2", dummy_on_result);
  object_set_prop_int(OBJECT(req), "count", 1123);
  object_set_prop_str(OBJECT(req), "name", "abc");
  model_t* model = model_jerryscript_create("test2", code, strlen(code), req);

  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 1123);

  object_unref(OBJECT(req));
  object_unref(OBJECT(model));
}

TEST(ModelJerryScript, invalid) {
  const char* code = "var a = 1;";
  model_t* model = model_jerryscript_create("notexist", code, strlen(code), NULL);

  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_get_prop_int(obj, "count", 0), 0);

  object_unref(OBJECT(model));
}
