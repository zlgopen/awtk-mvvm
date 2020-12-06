#include "gtest/gtest.h"
#include "mvvm/base/navigator_request.h"

#include <string>

using std::string;

TEST(NavigatorRequest, basic) {
  navigator_request_t* req = navigator_request_create(NAVIGATOR_REQ_TOAST, NULL);

  ASSERT_EQ(object_set_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE, "hello awtk"), RET_OK);
  ASSERT_EQ(string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)), string("hello awtk"));
  ASSERT_EQ(string(req->target), string(NAVIGATOR_REQ_TOAST));

  object_unref(OBJECT(req));
}

static string s_log;
static ret_t on_result(navigator_request_t* req, const value_t* result) {
  s_log = value_str(result);

  return RET_OK;
}

TEST(NavigatorRequest, on_result) {
  value_t v;
  navigator_request_t* req = navigator_request_create(NAVIGATOR_REQ_TOAST, on_result);

  value_set_str(&v, "hello awtk");
  navigator_request_on_result(req, &v);
  ASSERT_EQ(s_log, string("hello awtk"));

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args1) {
  navigator_request_t* req = navigator_request_create("target?key=value", on_result);
  ASSERT_STREQ(object_get_prop_str(OBJECT(req->args), "key"), "value");
  ASSERT_STREQ(req->target, "target");
  ASSERT_EQ(req->close_current, FALSE);
  ASSERT_EQ(req->open_new, TRUE);
  
  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args2) {
  navigator_request_t* req = navigator_request_create("target?key=", on_result);
  ASSERT_EQ(object_get_prop_str(OBJECT(req->args), "key"), (const char*)NULL);
  ASSERT_STREQ(req->target, "target");
  ASSERT_EQ(req->close_current, FALSE);
  ASSERT_EQ(req->open_new, TRUE);
  
  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args3) {
  navigator_request_t* req = navigator_request_create("target?key", on_result);
  ASSERT_EQ(object_get_prop_str(OBJECT(req->args), "key"), (const char*)NULL);
  ASSERT_STREQ(req->target, "target");
  ASSERT_EQ(req->close_current, FALSE);
  ASSERT_EQ(req->open_new, TRUE);

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args4) {
  navigator_request_t* req = navigator_request_create("target?close_current=true", on_result);
  ASSERT_EQ(req->close_current, TRUE);
  ASSERT_EQ(req->open_new, TRUE);
  ASSERT_STREQ(req->target, "target");
  
  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args5) {
  navigator_request_t* req = navigator_request_create("target?close_current=true&open_new=false", on_result);
  ASSERT_EQ(req->close_current, TRUE);
  ASSERT_EQ(req->open_new, FALSE);
  ASSERT_STREQ(req->target, "target");
  
  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args6) {
  navigator_request_t* req = navigator_request_create("target?close_current=true&open_new=false&path=a/b/c", on_result);
  ASSERT_EQ(req->close_current, TRUE);
  ASSERT_EQ(req->open_new, FALSE);
  ASSERT_STREQ(req->target, "target");
  ASSERT_STREQ(object_get_prop_str(OBJECT(req->args), "path"), "a/b/c");
  
  object_unref(OBJECT(req));
}
