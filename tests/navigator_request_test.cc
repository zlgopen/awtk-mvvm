#include "gtest/gtest.h"
#include "mvvm/base/navigator_request.h"

#include <string>

using std::string;

TEST(NavigatorRequest, basic) {
  navigator_request_t* req = navigator_request_create(NULL, NULL);
  object_set_prop_str(OBJECT(req), NAVIGATOR_ARG_REQ, NAVIGATOR_REQ_TOAST);

  ASSERT_EQ(object_set_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE, "hello awtk"), RET_OK);
  ASSERT_EQ(string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TITLE)), string("hello awtk"));
  ASSERT_EQ(string(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_REQ)),
            string(NAVIGATOR_REQ_TOAST));

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
  const char* target = "string?target=target&key=value";
  navigator_request_t* req = navigator_request_create(target, on_result);

  ASSERT_STREQ(object_get_prop_str(OBJECT(req), "key"), "value");
  ASSERT_STREQ(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TARGET), "target");
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), FALSE);
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), FALSE);

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args2) {
  const char* target = "string?target=target&key=";
  navigator_request_t* req = navigator_request_create(target, on_result);

  ASSERT_EQ(object_get_prop_str(OBJECT(req), "key"), (const char*)NULL);
  ASSERT_STREQ(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TARGET), "target");
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), FALSE);
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), FALSE);

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args3) {
  const char* target = "string?target=target&key";
  navigator_request_t* req = navigator_request_create(target, on_result);

  ASSERT_EQ(object_get_prop_str(OBJECT(req), "key"), (const char*)NULL);
  ASSERT_STREQ(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TARGET), "target");
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), FALSE);
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), FALSE);

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args4) {
  const char* target = "string?target=target&close_current=true";
  navigator_request_t* req = navigator_request_create(target, on_result);

  ASSERT_STREQ(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TARGET), "target");
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), TRUE);
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), FALSE);

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args5) {
  const char* target = "string?target=target&close_current=true&open_new=true";
  navigator_request_t* req = navigator_request_create(target, on_result);

  ASSERT_STREQ(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TARGET), "target");
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), TRUE);
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), TRUE);

  object_unref(OBJECT(req));
}

TEST(NavigatorRequest, args6) {
  const char* target = "string?target=target&close_current=true&open_new=false&path=a/b/c";
  navigator_request_t* req = navigator_request_create(target, on_result);

  ASSERT_STREQ(object_get_prop_str(OBJECT(req), NAVIGATOR_ARG_TARGET), "target");
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_CLOSE_CURRENT, FALSE), TRUE);
  ASSERT_EQ(object_get_prop_bool(OBJECT(req), NAVIGATOR_ARG_OPEN_NEW, FALSE), FALSE);
  ASSERT_STREQ(object_get_prop_str(OBJECT(req), "path"), "a/b/c");

  object_unref(OBJECT(req));
}
