﻿#include "mvvm/base/binding_rule_parser.h"
#include "mvvm/base/command_binding.h"
#include "mvvm/base/data_binding.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(CommandBindingParser, close_window) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, CloseWindow}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, TRUE);
  ASSERT_EQ(cmd->auto_disable, TRUE);
  ASSERT_EQ(string(cmd->command), string("Save"));
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, auto_disable) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, AutoDisable=false}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->auto_disable, FALSE);
  ASSERT_EQ(string(cmd->command), string("Save"));
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, args) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{add_char, args=1}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(string(cmd->command), string("add_char"));
  ASSERT_EQ(string(cmd->args), string("1"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, args_expr) {
  binding_rule_t* rule = binding_rule_parse(
      "v-on:click", "{fscript, args=set(a, 123);set(b, \"abc\");set(c, a+b)}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(string(cmd->command), string("fscript"));
  ASSERT_EQ(string(cmd->args), string("set(a, 123);set(b, \"abc\");set(c, a+b)"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, args1) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{add_char, args==}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(string(cmd->command), string("add_char"));
  ASSERT_EQ(string(cmd->args), string("="));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, args3) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{add_char, args==+-*/==}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(string(cmd->command), string("add_char"));
  ASSERT_EQ(string(cmd->args), string("=+-*/=="));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, close_window_true) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, CloseWindow=True}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, TRUE);
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, close_window_false) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, CloseWindow=False}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, FALSE);
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, update_model) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, UpdateModel}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, TRUE);
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, update_model_true) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, UpdateModel=True}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, TRUE);
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, update_model_false) {
  binding_rule_t* rule = binding_rule_parse("v-on:click", "{Save, UpdateModel=False}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("click"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, key_filter) {
  binding_rule_t* rule =
      binding_rule_parse("v-on:keydown:ctrl_a", "{Save, UpdateModel=False}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("keydown"));
  ASSERT_EQ(string(cmd->key_filter), string("ctrl_a"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(CommandBindingParser, ikey_filter) {
  binding_rule_t* rule =
      binding_rule_parse("v-on:keydown:ctrl_a", "{Save, updateModel=false}", TRUE);
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("keydown"));
  ASSERT_EQ(string(cmd->key_filter), string("ctrl_a"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, basic) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, mode) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Mode=TwoWay}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(data->mode, BINDING_TWO_WAY);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, imode) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, mode=twoWay}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(data->mode, BINDING_TWO_WAY);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, trigger) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Trigger=Explicit}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(data->trigger, UPDATE_WHEN_EXPLICIT);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, itrigger) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, trigger=explicit}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(data->trigger, UPDATE_WHEN_EXPLICIT);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, converter) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Converter=converter}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(string(data->converter), string("converter"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, iconverter) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, converter=converter}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->converter), string("converter"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, validator) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, Validator=validator}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("Name"));
  ASSERT_EQ(string(data->prop), string("text"));
  ASSERT_EQ(string(data->validator), string("validator"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, ivalidator) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{Name, validator=validator}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->validator), string("validator"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, expr) {
  binding_rule_t* rule = binding_rule_parse("v-data:text", "{$type==1}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_EQ(string(data->path), string("$type==1"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBindingParser, style) {
  binding_rule_t* rule = binding_rule_parse("v-data:style:normal:text_color", "{$type==1}", TRUE);
  data_binding_t* data = (data_binding_t*)rule;

  ASSERT_STREQ((data->prop), ("style:normal:text_color"));

  tk_object_unref(TK_OBJECT(rule));
}
