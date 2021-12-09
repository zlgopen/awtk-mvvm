#include "mvvm/base/data_binding.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(DataBinding, basic) {
  data_binding_t* rule = (data_binding_t*)data_binding_create();
  tk_object_t* o = TK_OBJECT(rule);
  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_PATH, "Name"), RET_OK);
  ASSERT_EQ(string(rule->path), string("Name"));

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_PROP, "text"), RET_OK);
  ASSERT_EQ(string(rule->prop), string("text"));

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_CONVERTER, "conv"), RET_OK);
  ASSERT_EQ(string(rule->converter), string("conv"));

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_VALIDATOR, "valid"), RET_OK);
  ASSERT_EQ(string(rule->validator), string("valid"));

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBinding, mode) {
  data_binding_t* rule = (data_binding_t*)data_binding_create();
  tk_object_t* o = TK_OBJECT(rule);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_MODE, "Once"), RET_OK);
  ASSERT_EQ(rule->mode, BINDING_ONCE);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_MODE, "OneWay"), RET_OK);
  ASSERT_EQ(rule->mode, BINDING_ONE_WAY);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_MODE, "TwoWay"), RET_OK);
  ASSERT_EQ(rule->mode, BINDING_TWO_WAY);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_MODE, "OneWayToModel"), RET_OK);
  ASSERT_EQ(rule->mode, BINDING_ONE_WAY_TO_VIEW_MODEL);

  tk_object_unref(TK_OBJECT(rule));
}

TEST(DataBinding, trigger) {
  data_binding_t* rule = (data_binding_t*)data_binding_create();
  tk_object_t* o = TK_OBJECT(rule);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_TRIGGER, "Changed"), RET_OK);
  ASSERT_EQ(rule->trigger, UPDATE_WHEN_CHANGED);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_TRIGGER, "Changing"), RET_OK);
  ASSERT_EQ(rule->trigger, UPDATE_WHEN_CHANGING);

  ASSERT_EQ(tk_object_set_prop_str(o, DATA_BINDING_TRIGGER, "Explicit"), RET_OK);
  ASSERT_EQ(rule->trigger, UPDATE_WHEN_EXPLICIT);

  tk_object_unref(TK_OBJECT(rule));
}
