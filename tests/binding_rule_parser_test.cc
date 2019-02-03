#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/data_binding.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(CommandBindingParser, close_window) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, CloseWindow}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, TRUE);
  ASSERT_EQ(string(cmd->command), string("Save"));
  ASSERT_EQ(string(cmd->event), string("click"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, close_window_true) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, CloseWindow=True}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, TRUE);
  ASSERT_EQ(string(cmd->event), string("click"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, close_window_false) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, CloseWindow=False}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, FALSE);
  ASSERT_EQ(string(cmd->event), string("click"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, update_model) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, UpdateModel}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, TRUE);
  ASSERT_EQ(string(cmd->event), string("click"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, update_model_true) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, UpdateModel=True}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, TRUE);
  ASSERT_EQ(string(cmd->event), string("click"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, update_model_false) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, UpdateModel=False}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("click"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, event_args) {
  binding_rule_t* rule = binding_rule_parse("v-on:keydown:ctrl_a", "{Save, UpdateModel=False}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("keydown"));
  ASSERT_EQ(string(cmd->event_args), string("ctrl_a"));

  object_unref(OBJECT(rule));
}

TEST(CommandBindingParser, ievent_args) {
  binding_rule_t* rule = binding_rule_parse("v-on:keydown:ctrl_a", "{Save, updateModel=false}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("keydown"));
  ASSERT_EQ(string(cmd->event_args), string("ctrl_a"));

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, basic) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, mode) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Mode=TwoWay}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(data->mode, BINDING_TWO_WAY);

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, imode) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, mode=twoWay}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(data->mode, BINDING_TWO_WAY);

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, trigger) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Trigger=Explicit}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(data->trigger, UPDATE_WHEN_EXPLICIT);

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, itrigger) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, trigger=explicit}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(data->trigger, UPDATE_WHEN_EXPLICIT);

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, converter) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Converter=converter}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(string(data->converter), string("converter"));

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, iconverter) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, converter=converter}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->converter), string("converter"));

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, validator) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Validator=validator}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(string(data->validator), string("validator"));

  object_unref(OBJECT(rule));
}

TEST(DataBindingParser, ivalidator) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, validator=validator}");
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->validator), string("validator"));

  object_unref(OBJECT(rule));
}
