#include "mvvm/base/command_binding.h"
#include "gtest/gtest.h"
#include "base/keys.h"

#include <string>
using std::string;

TEST(CommandBinding, basic) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  tk_object_t* o = TK_OBJECT(rule);
  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_COMMAND, "Name"), RET_OK);
  ASSERT_EQ(string(rule->command), string("Name"));

  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_ARGS, "text"), RET_OK);
  ASSERT_EQ(string(rule->args), string("text"));

  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_EVENT, "event"), RET_OK);
  ASSERT_EQ(string(rule->event), string("event"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBinding, key_filter) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  tk_object_t* o = TK_OBJECT(rule);

  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_KEY_FILTER, "ctrl_a"), RET_OK);
  ASSERT_EQ(rule->filter.ctrl, 1u);
  ASSERT_EQ(rule->filter.key, TK_KEY_a);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBinding, key_filter1) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  tk_object_t* o = TK_OBJECT(rule);

  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_KEY_FILTER, "ctrl_shift_LEFT"), RET_OK);
  ASSERT_EQ(rule->filter.ctrl, 1u);
  ASSERT_EQ(rule->filter.shift, 1u);
  ASSERT_EQ(rule->filter.lctrl, 1u);
  ASSERT_EQ(rule->filter.lshift, 1u);
  ASSERT_EQ(rule->filter.rctrl, 1u);
  ASSERT_EQ(rule->filter.rshift, 1u);
  ASSERT_EQ(rule->filter.key, TK_KEY_LEFT);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBinding, close_window) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  tk_object_t* o = TK_OBJECT(rule);

  ASSERT_EQ(rule->close_window, FALSE);
  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_CLOSE_WINDOW, "TRUE"), RET_OK);
  ASSERT_EQ(rule->close_window, TRUE);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBinding, update_model) {
  command_binding_t* rule = (command_binding_t*)command_binding_create();
  tk_object_t* o = TK_OBJECT(rule);

  ASSERT_EQ(rule->update_model, FALSE);
  ASSERT_EQ(tk_object_set_prop_str(o, COMMAND_BINDING_UPDATE_VIEW_MODEL, "TRUE"), RET_OK);
  ASSERT_EQ(rule->update_model, TRUE);

  tk_object_unref(TK_OBJECT(rule));
}
