#include "gtest/gtest.h"
#include "mvvm/base/navigator.h"

#include <string>
using std::string;

static ret_t dummy_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_set_int(&(req->result), 123);
  navigator_request_on_result(req, &(req->result));

  return RET_OK;
}

static ret_t default_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_set_int(&(req->result), 456);
  navigator_request_on_result(req, &(req->result));

  return RET_OK;
}

TEST(Navigator, regist) {
  navigator_t* nav = navigator_create();
  navigator_handler_t* handler = navigator_handler_create(dummy_on_request);

  ASSERT_EQ(navigator_has_handler(nav, "dummy"), FALSE);
  ASSERT_EQ(navigator_register_handler(nav, "dummy", handler), RET_OK);
  ASSERT_EQ(navigator_has_handler(nav, "dummy"), TRUE);
  ASSERT_EQ(navigator_unregister_handler(nav, "dummy"), RET_OK);
  ASSERT_EQ(navigator_has_handler(nav, "dummy"), FALSE);

  tk_object_unref(TK_OBJECT(nav));
}

TEST(Navigator, no_handler) {
  navigator_t* nav = navigator_create();
  navigator_request_t* req = navigator_request_create("string?request=dummy", NULL);

  ASSERT_EQ(navigator_handle_request(nav, req), RET_NOT_FOUND);

  tk_object_unref(TK_OBJECT(nav));
}

TEST(Navigator, handler_request) {
  navigator_t* nav = navigator_create();
  navigator_request_t* req = navigator_request_create("string?request=dummy", NULL);

  navigator_register_handler(nav, "dummy", navigator_handler_create(dummy_on_request));

  ASSERT_EQ(navigator_handle_request(nav, req), RET_OK);
  ASSERT_EQ(value_int(&(req->result)), 123);

  tk_object_unref(TK_OBJECT(nav));
}

TEST(Navigator, default_handler) {
  navigator_t* nav = navigator_create();
  navigator_request_t* req = navigator_request_create("string?request=dummy", NULL);

  navigator_register_handler(nav, NAVIGATOR_DEFAULT_HANDLER,
                             navigator_handler_create(default_on_request));

  ASSERT_EQ(navigator_handle_request(nav, req), RET_OK);
  ASSERT_EQ(value_int(&(req->result)), 456);

  tk_object_unref(TK_OBJECT(nav));
}

static string s_log;
static ret_t toast_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log =
      string("toast:") + string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT));

  return RET_OK;
}

TEST(Navigator, toast) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_TOAST, navigator_handler_create(toast_on_request));
  ASSERT_EQ(navigator_toast("hello", 1000), RET_OK);
  ASSERT_EQ(s_log, string("toast:hello"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t info_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log = string("info:") + string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT));

  return RET_OK;
}

TEST(Navigator, info) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_INFO, navigator_handler_create(info_on_request));
  ASSERT_EQ(navigator_info("hello", " awtk"), RET_OK);
  ASSERT_EQ(s_log, string("info:hello awtk"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t confirm_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log = string("confirm:") +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT));

  value_set_int(&(req->result), RET_FAIL);

  navigator_request_on_result(req, &(req->result));

  return RET_OK;
}

TEST(Navigator, confirm) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_CONFIRM,
                             navigator_handler_create(confirm_on_request));
  ASSERT_EQ(navigator_confirm("hello", " awtk"), RET_FAIL);
  ASSERT_EQ(s_log, string("confirm:hello awtk"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t warn_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  s_log = string("warn:") + string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_CONTENT));

  return RET_OK;
}

TEST(Navigator, warn) {
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  navigator_register_handler(nav, NAVIGATOR_REQ_WARN, navigator_handler_create(warn_on_request));
  ASSERT_EQ(navigator_warn("hello", " awtk"), RET_OK);
  ASSERT_EQ(s_log, string("warn:hello awtk"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t pick_dir_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;

  s_log = string("pick_dir:") +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT));
  value_set_str(&v, "dir");

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, pick_dir) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_PICK_DIR,
                             navigator_handler_create(pick_dir_on_request));
  ASSERT_EQ(navigator_pick_dir("hello", &str), RET_OK);
  ASSERT_EQ(s_log, string("pick_dir:hello awtk"));
  ASSERT_EQ(string(str.str), string("dir"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t pick_color_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;
  s_log = string("pick_color:") +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT));
  value_set_str(&v, "color");
  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, pick_color) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_PICK_COLOR,
                             navigator_handler_create(pick_color_on_request));
  ASSERT_EQ(navigator_pick_color("hello", &str), RET_OK);
  ASSERT_EQ(s_log, string("pick_color:hello awtk"));
  ASSERT_EQ(string(str.str), string("color"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t pick_file_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;

  s_log = string("pick_file:") +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TITLE)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_FILTER)) +
          string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_DEFAULT));
  value_set_str(&v, "file");

  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, pick_file) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_PICK_FILE,
                             navigator_handler_create(pick_file_on_request));
  ASSERT_EQ(navigator_pick_file("hello", "image/png", TRUE, &str), RET_OK);
  ASSERT_EQ(s_log, string("pick_file:helloimage/png awtk"));
  ASSERT_EQ(string(str.str), string("file"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

static ret_t navigator_on_close(navigator_handler_t* handler, navigator_request_t* req) {
  value_t v;
  s_log =
      string("close:") + string(tk_object_get_prop_str(TK_OBJECT(req), NAVIGATOR_ARG_TARGET));

  if (tk_object_get_prop_bool(TK_OBJECT(req), NAVIGATOR_ARG_FORCE, FALSE)) {
    s_log += " force=true";
  } else {
    s_log += " force=false";
  }

  value_set_bool(&v, TRUE);
  navigator_request_on_result(req, &v);

  return RET_OK;
}

TEST(Navigator, close) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_CLOSE,
                             navigator_handler_create(navigator_on_close));
  ASSERT_EQ(navigator_close("hello"), RET_OK);
  ASSERT_EQ(s_log, string("close:hello force=true"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}

TEST(Navigator, request_close) {
  str_t str;
  navigator_t* old = navigator();
  navigator_t* nav = navigator_create();
  navigator_set(nav);

  str_init(&str, 0);
  str_set(&str, " awtk");
  navigator_register_handler(nav, NAVIGATOR_REQ_CLOSE,
                             navigator_handler_create(navigator_on_close));
  ASSERT_EQ(navigator_request_close("hello"), RET_OK);
  ASSERT_EQ(s_log, string("close:hello force=false"));

  tk_object_unref(TK_OBJECT(nav));
  navigator_set(old);
}
