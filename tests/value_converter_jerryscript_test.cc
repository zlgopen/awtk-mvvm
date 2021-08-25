#include "mvvm/base/view_model_factory.h"
#include "mvvm/jerryscript/jerry_script_helper.h"
#include "mvvm/jerryscript/view_model_jerryscript.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ValueConverterJerryScript, basic) {
  const char* code =
      "ValueConverter('jsdummy', {\
         toView:function(v) {return v+1;},\
         toModel:function(v) {return v-1}\
       });";
  value_t from;
  value_t to;
  value_converter_t* c;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  c = value_converter_create("jsdummy");
  ASSERT_NE(c, VALUE_CONVERTER(NULL));

  value_set_int(&from, 100);
  ASSERT_EQ(value_converter_to_view(c, &from, &to), RET_OK);
  ASSERT_EQ(value_int(&from) + 1, value_int(&to));

  value_set_int(&from, 100);
  ASSERT_EQ(value_converter_to_model(c, &from, &to), RET_OK);
  ASSERT_EQ(value_int(&from) - 1, value_int(&to));

  object_unref(OBJECT(c));
}

TEST(ValueConverterJerryScript, not_exist) {
  value_converter_t* c = value_converter_create("notexist");
  ASSERT_EQ(c, VALUE_CONVERTER(NULL));
}
