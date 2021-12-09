#include "mvvm/base/view_model_factory.h"
#include "mvvm/jerryscript/jerry_script_helper.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ModelJerryScript, get_prop) {
  const char* code =
      "ViewModel('test', {\
         data: {a:1, b:true, name:'awtk'}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(tk_object_get_prop_int(obj, "a", 0), 1);
  ASSERT_EQ(tk_object_get_prop_int(obj, "b", 0), 1);
  ASSERT_EQ(tk_object_get_prop_int(obj, "c", 0), 0);
  ASSERT_EQ(string(tk_object_get_prop_str(obj, "name")), string("awtk"));

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, array) {
  const char* code =
      "ViewModel('test_array', {\
         data: {items: [{'name':'a', 'stock':1}, {'name':'b', 'stock':2}, {'name':'c', 'stock':3}]}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test_array", TRUE);
  view_model = view_model_factory_create_model("test_array", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.length", 0), 3);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.[0].stock", 0), 1);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.[1].stock", 0), 2);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.[2].stock", 0), 3);

  ASSERT_EQ(tk_object_set_prop_int_by_path(obj, "items.[0].stock", 4), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int_by_path(obj, "items.[1].stock", 5), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int_by_path(obj, "items.[2].stock", 6), RET_OK);

  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.[0].stock", 0), 4);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.[1].stock", 0), 5);
  ASSERT_EQ(tk_object_get_prop_int_by_path(obj, "items.[2].stock", 0), 6);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, set_prop) {
  const char* code =
      "ViewModel('test', {\
         data: {a:1, b:true, name:'awtk'}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(tk_object_set_prop_int(obj, "a", 11), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(obj, "b", 22), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(obj, "c", 33), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(obj, "age", 88), RET_OK);
  ASSERT_EQ(tk_object_set_prop_str(obj, "name", "zlg"), RET_OK);

  ASSERT_EQ(tk_object_get_prop_int(obj, "a", 0), 11);
  ASSERT_EQ(tk_object_get_prop_int(obj, "b", 0), 22);
  ASSERT_EQ(tk_object_get_prop_int(obj, "c", 0), 33);
  ASSERT_EQ(tk_object_get_prop_int(obj, "age", 0), 88);
  ASSERT_EQ(string(tk_object_get_prop_str(obj, "name")), string("zlg"));

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, exec) {
  const char* code =
      "ViewModel('test', {\
         methods: {save: function(args) {print(args); return 0;}, canSave: function(args) {print(args); return true;}}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(tk_object_can_exec(obj, "save", "awtk\n"), TRUE);
  ASSERT_EQ(tk_object_exec(obj, "save", "awtk\n"), RET_OK);

  ASSERT_EQ(tk_object_can_exec(obj, "load", "awtk\n"), FALSE);
  ASSERT_NE(tk_object_exec(obj, "load", "awtk\n"), RET_OK);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, exec1) {
  const char* code =
      "ViewModel('test', {\
         methods: {save1: function(args) {return 10;}, canSave1: function(args) {print(args); return true;}}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(tk_object_can_exec(obj, "save1", "awtk\n"), TRUE);
  ASSERT_EQ(tk_object_exec(obj, "save1", "awtk\n"), 10);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, exec2) {
  const char* code =
      "ViewModel('test', {\
         methods: {save2: function(args) {return 0;}, canSave2: function(args) {print(args); return true;}}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(tk_object_can_exec(obj, "save2", "awtk\n"), TRUE);
  ASSERT_EQ(tk_object_exec(obj, "save2", "awtk\n"), 0);

  tk_object_unref(TK_OBJECT(view_model));
}

static ret_t on_view_model_changed(void* ctx, event_t* e) {
  int* n = (int*)ctx;

  *n = *n + 1;
  log_debug("on_view_model_changed\n");

  return RET_OK;
}

TEST(ModelJerryScript, notifyPropsChanged) {
  int n = 0;
  const char* code =
      "ViewModel('test', {\
         data: {count:0},\
         methods: {\
           save: function(args) {print(args); this.count++; this.notifyPropsChanged(); return 0;}\
         }\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, on_view_model_changed, &n);

  ASSERT_EQ(tk_object_can_exec(obj, "save", "awtk\n"), TRUE);
  ASSERT_EQ(tk_object_exec(obj, "save", "awtk\n"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 1);
  ASSERT_EQ(n, 1);

  ASSERT_EQ(tk_object_exec(obj, "save", "awtk\n"), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 2);
  ASSERT_EQ(n, 2);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, onWillmount) {
  const char* code =
      "ViewModel('test', {\
         data: {count:0},\
         onWillMount: function(req) {\
           print('onWillMount js');\
           this.count = req.count;\
           this.name = req.name;\
           this.notifyPropsChanged();\
           return 0;\
         }\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  navigator_request_t* req = navigator_request_create("test", NULL);
  tk_object_set_prop_int(TK_OBJECT(req), "count", 100);
  tk_object_set_prop_str(TK_OBJECT(req), "name", "abc");

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", req);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(view_model_on_will_mount(view_model, req), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 100);
  ASSERT_EQ(string(tk_object_get_prop_str(obj, "name")), "abc");

  tk_object_unref(TK_OBJECT(req));
  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, onMount) {
  const char* code =
      "ViewModel('test', {\
         data: {count:0},\
         onMount: function() {print('onMount js'); this.count++; this.notifyPropsChanged(); return 0;}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(view_model_on_mount(view_model), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 1);

  ASSERT_EQ(view_model_on_mount(view_model), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 2);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, onWillUnmount) {
  const char* code =
      "ViewModel('test', {\
         data: {count:0},\
         onWillUnmount: function() {print('onWillUnmount js'); this.count++; this.notifyPropsChanged(); return 0;}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(view_model_on_will_unmount(view_model), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 1);

  ASSERT_EQ(view_model_on_will_unmount(view_model), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 2);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, onUnmount) {
  const char* code =
      "ViewModel('test', {\
         data: {count:0},\
         onUnmount: function() {print('onUnmount js'); this.count++; this.notifyPropsChanged(); return 0;}\
       });";
  view_model_t* view_model;
  tk_object_t* obj;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  view_model = view_model_factory_create_model("test", NULL);
  obj = TK_OBJECT(view_model);
  ASSERT_NE(obj, TK_OBJECT(NULL));

  ASSERT_EQ(view_model_on_unmount(view_model), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 1);

  ASSERT_EQ(view_model_on_unmount(view_model), RET_OK);
  ASSERT_EQ(tk_object_get_prop_int(obj, "count", 0), 2);

  tk_object_unref(TK_OBJECT(view_model));
}

TEST(ModelJerryScript, invalid) {
  view_model_t* view_model = view_model_factory_create_model("notexist", NULL);
  tk_object_t* obj = TK_OBJECT(view_model);
  ASSERT_EQ(obj, TK_OBJECT(NULL));
}
