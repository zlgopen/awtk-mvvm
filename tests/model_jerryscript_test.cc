#include "mvvm/jerryscript/model_jerryscript.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

class JerryscriptObj {
 public:
  JerryscriptObj() {
    model_jerryscript_init();
  }

  ~JerryscriptObj() {
    model_jerryscript_deinit();
  }
};

static JerryscriptObj sJerryscriptObj;

TEST(ModelJerryScript, get_prop) {
  const char* code = "var test = {a:1, b:true, name:'awtk'};";
  model_t* model = model_jerryscript_create("test", code, strlen(code));
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
  model_t* model = model_jerryscript_create("test", code, strlen(code));
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
  model_t* model = model_jerryscript_create("test", code, strlen(code));
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
  model_t* model = model_jerryscript_create("test", code, strlen(code));
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
  model_t* model = model_jerryscript_create("test", code, strlen(code));
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
  model_t* model = model_jerryscript_create("test", code, strlen(code));
  object_t* obj = OBJECT(model);
  ASSERT_NE(obj, OBJECT(NULL));

  ASSERT_EQ(object_can_exec(obj, "save", "awtk\n"), TRUE);
  ASSERT_EQ(object_exec(obj, "save", "awtk\n"), RET_OK);

  ASSERT_EQ(object_can_exec(obj, "load", "awtk\n"), FALSE);
  ASSERT_NE(object_exec(obj, "load", "awtk\n"), RET_OK);

  object_unref(OBJECT(model));
}
