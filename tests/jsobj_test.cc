#include "mvvm/jerryscript/jsobj.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(JsValue, pointer) {
  int a = 123;
  jerry_value_t jsvalue = jerry_value_from_pointer(&a);
  int* ap = (int*)jerry_value_to_pointer(jsvalue);
  ASSERT_EQ(ap, &a);

  jerry_release_value(jsvalue);
}

TEST(JsValue, int) {
  value_t v;
  value_t v1;
  value_set_int(&v, 123);
  jerry_value_t jsvalue = jerry_value_from_value(&v, NULL);

  ASSERT_EQ(jerry_value_to_value(jsvalue, &v1, NULL), RET_OK);
  ASSERT_EQ(value_int(&v), value_int(&v1));

  jerry_release_value(jsvalue);
}

TEST(JsValue, float) {
  value_t v;
  value_t v1;
  value_set_float(&v, 123);
  jerry_value_t jsvalue = jerry_value_from_value(&v, NULL);

  ASSERT_EQ(jerry_value_to_value(jsvalue, &v1, NULL), RET_OK);
  ASSERT_EQ(value_float(&v), value_float(&v1));

  jerry_release_value(jsvalue);
}

TEST(JsValue, str) {
  value_t v;
  value_t v1;
  str_t str;
  str_init(&str, 0);
  value_set_str(&v, "123");
  jerry_value_t jsvalue = jerry_value_from_value(&v, &str);

  ASSERT_EQ(jerry_value_to_value(jsvalue, &v1, &str), RET_OK);
  ASSERT_EQ(string(value_str(&v)), string(value_str(&v1)));

  jerry_release_value(jsvalue);
}

TEST(JsValue, wstr) {
  value_t v;
  value_t v1;
  str_t str;
  str_init(&str, 0);
  value_set_wstr(&v, L"123");
  jerry_value_t jsvalue = jerry_value_from_value(&v, &str);

  ASSERT_EQ(jerry_value_to_value(jsvalue, &v1, &str), RET_OK);
  ASSERT_EQ(string("123"), string(value_str(&v1)));

  jerry_release_value(jsvalue);
}

TEST(JsValue, request) {
  navigator_request_t* req = navigator_request_create("target", NULL);
  object_set_prop_int(OBJECT(req), "int", 100);
  object_set_prop_str(OBJECT(req), "str", "str");
  jerry_value_t value = jerry_value_from_navigator_request(req);
  navigator_request_t* req1 = jerry_value_to_navigator_request(value);

  ASSERT_EQ(object_get_prop_int(OBJECT(req), "int", 0),
            object_get_prop_int(OBJECT(req1), "int", 0));
  ASSERT_STREQ(object_get_prop_str(OBJECT(req), "str"), object_get_prop_str(OBJECT(req1), "str"));

  jerry_release_value(value);
  object_unref(OBJECT(req));
}
