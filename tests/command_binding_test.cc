#include "mvvm/base/command_binding.h"
#include "gtest/gtest.h"
#include "base/keys.h"

#include <string>
using std::string;

TEST(CommandBinding, basic) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  object_t* o = OBJECT(rule);
  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_COMMAND, "Name"), RET_OK);
  ASSERT_EQ(string(rule->command), string("Name"));

  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_ARGS, "text"), RET_OK);
  ASSERT_EQ(string(rule->args), string("text"));

  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_EVENT, "event"), RET_OK);
  ASSERT_EQ(string(rule->event), string("event"));

  object_unref(OBJECT(rule));
}

TEST(CommandBinding, key_filter) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  object_t* o = OBJECT(rule);

  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_KEY_FILTER, "ctrl_a"), RET_OK);
  ASSERT_EQ(rule->filter.ctrl, TRUE);
  ASSERT_EQ(rule->filter.key, TK_KEY_a);

  object_unref(OBJECT(rule));
}

TEST(CommandBinding, key_filter1) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  object_t* o = OBJECT(rule);

  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_KEY_FILTER, "ctrl_shift_LEFT"), RET_OK);
  ASSERT_EQ(rule->filter.ctrl, TRUE);
  ASSERT_EQ(rule->filter.shift, TRUE);
  ASSERT_EQ(rule->filter.lctrl, TRUE);
  ASSERT_EQ(rule->filter.lshift, TRUE);
  ASSERT_EQ(rule->filter.rctrl, TRUE);
  ASSERT_EQ(rule->filter.rshift, TRUE);
  ASSERT_EQ(rule->filter.key, TK_KEY_LEFT);

  object_unref(OBJECT(rule));
}

TEST(CommandBinding, close_window) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  object_t* o = OBJECT(rule);

  ASSERT_EQ(rule->close_window, FALSE);
  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_CLOSE_WINDOW, "TRUE"), RET_OK);
  ASSERT_EQ(rule->close_window, TRUE);

  object_unref(OBJECT(rule));
}

TEST(CommandBinding, update_model) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  object_t* o = OBJECT(rule);

  ASSERT_EQ(rule->update_model, FALSE);
  ASSERT_EQ(object_set_prop_str(o, COMMAND_BINDING_UPDATE_VIEW_MODEL, "TRUE"), RET_OK);
  ASSERT_EQ(rule->update_model, TRUE);

  object_unref(OBJECT(rule));
}
