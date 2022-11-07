#include "mvvm/base/navigator_request.h"
#include "mvvm/jerryscript/jsobj_4_mvvm.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(JsValue, native_ptr) {
  jsvalue_t jsobj = JS_EMPTY_OBJ;
  tk_object_t* p = (tk_object_t*)0x12345678;
  tk_object_t* obj = jsobj_get_native_ptr(jsobj);

  ASSERT_EQ(obj, (tk_object_t*)NULL);
  ASSERT_EQ(jsobj_set_native_ptr(jsobj, p), RET_OK);

  obj = jsobj_get_native_ptr(jsobj);
  ASSERT_EQ(obj, (tk_object_t*)0x12345678);

  jsobj_delete_native_ptr(jsobj);
  obj = jsobj_get_native_ptr(jsobj);
  ASSERT_EQ(obj, (tk_object_t*)NULL);

  jsvalue_unref(jsobj);
}

TEST(JsValue, pointer) {
  value_t v1, v2;
  jsvalue_t jsvalue;

  value_set_pointer(&v1, (void*)0x12345678);
  jsvalue = jsvalue_from_value(&v1, NULL);
  ASSERT_EQ(jsvalue_check(jsvalue), RET_OK);

  ASSERT_EQ(jsvalue_to_value(jsvalue, &v2, NULL), RET_OK);
  ASSERT_EQ(value_equal(&v1, &v2), TRUE);

  jsvalue_unref(jsvalue);
}

TEST(JsValue, int) {
  value_t v1, v2;
  jsvalue_t jsvalue;

  value_set_int(&v1, 123);
  jsvalue = jsvalue_from_value(&v1, NULL);
  ASSERT_EQ(jsvalue_check(jsvalue), RET_OK);

  ASSERT_EQ(jsvalue_to_value(jsvalue, &v2, NULL), RET_OK);
  ASSERT_EQ(value_int(&v1), value_int(&v2));

  jsvalue_unref(jsvalue);
}

TEST(JsValue, float) {
  value_t v1, v2;
  jsvalue_t jsvalue;

  value_set_float(&v1, 123);
  jsvalue = jsvalue_from_value(&v1, NULL);
  ASSERT_EQ(jsvalue_check(jsvalue), RET_OK);

  ASSERT_EQ(jsvalue_to_value(jsvalue, &v2, NULL), RET_OK);
  ASSERT_EQ(value_float(&v1), value_float(&v2));

  jsvalue_unref(jsvalue);
}

TEST(JsValue, str) {
  str_t str;
  value_t v1, v2;
  jsvalue_t jsvalue;

  str_init(&str, 0);
  value_set_str(&v1, "123");
  jsvalue = jsvalue_from_value(&v1, &str);
  ASSERT_EQ(jsvalue_check(jsvalue), RET_OK);

  ASSERT_EQ(jsvalue_to_value(jsvalue, &v2, &str), RET_OK);
  ASSERT_EQ(string(value_str(&v1)), string(value_str(&v2)));

  str_reset(&str);
  jsvalue_unref(jsvalue);
}

TEST(JsValue, wstr) {
  str_t str;
  value_t v1, v2;
  jsvalue_t jsvalue;

  str_init(&str, 0);
  value_set_wstr(&v1, L"123");
  jsvalue = jsvalue_from_value(&v1, &str);
  ASSERT_EQ(jsvalue_check(jsvalue), RET_OK);

  ASSERT_EQ(jsvalue_to_value(jsvalue, &v2, &str), RET_OK);
  ASSERT_EQ(string("123"), string(value_str(&v2)));

  str_reset(&str);
  jsvalue_unref(jsvalue);
}

TEST(JsValue, request) {
  jsvalue_t jsreq = JS_EMPTY_OBJ;
  jsobj_set_prop_int(jsreq, "int", 100);
  jsobj_set_prop_str(jsreq, "str", "str");
  jsobj_set_prop_str(jsreq, NAVIGATOR_ARG_TARGET, "main");
  jsobj_set_prop_int(jsreq, NAVIGATOR_ARG_OPEN_NEW, TRUE);
  jsobj_set_prop_int(jsreq, NAVIGATOR_ARG_CLOSE_CURRENT, TRUE);

  navigator_request_t* req = jsvalue_to_navigator_request(jsreq);
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(req), "int", 0), 100);
  ASSERT_EQ(string(tk_object_get_prop_str(TK_OBJECT(req), "str")), string("str"));
  ASSERT_EQ(string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET)), string("main"));
  ASSERT_EQ(tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), TRUE);
  ASSERT_EQ(tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), TRUE);

  jsvalue_t jsreq1 = jsvalue_from_navigator_request(req);
  ASSERT_EQ(jsreq, jsreq1);

  jsvalue_unref(jsreq1);
  jsvalue_unref(jsreq);
  tk_object_unref(TK_OBJECT(req));
}

TEST(JsValue, exec) {
  str_t str;
  uint32_t i;
  value_t value;
  value_t vargs_p[3];
  jsvalue_t jsargs_p[3];
  jsvalue_t jsobj = JS_EMPTY_ARRAY;
  jsvalue_t jsret;

  str_init(&str, 0);
  value_set_int32(vargs_p, 0);
  value_set_int32(vargs_p + 1, 1);
  value_set_int32(vargs_p + 2, 2);

  ASSERT_EQ(jsobj_exec_value(jsobj, "push", vargs_p, 3, &value, &str), RET_OK);
  ASSERT_EQ(jsobj_get_prop_count(jsobj), 3);
  ASSERT_EQ(value_int32(&value), 3);

  jsargs_p[0] = jsvalue_from_number(3);
  jsargs_p[1] = jsvalue_from_number(4);
  jsargs_p[2] = jsvalue_from_number(5);
  jsret = jsobj_exec_ex_value(jsobj, "push", jsargs_p, 3);
  ASSERT_EQ(jsvalue_to_number(jsret), 6);

  for (i = 0; i < 6; i++) {
    ASSERT_EQ(jsobj_get_prop_by_index(jsobj, i, &value, &str), RET_OK);
    ASSERT_EQ(value_int32(&value), i);
  }

  for (i = 0; i < 3; i++) {
    jsvalue_unref(*(jsargs_p + i));
  }
  jsvalue_unref(jsret);
  jsvalue_unref(jsobj);
  str_reset(&str);
}
