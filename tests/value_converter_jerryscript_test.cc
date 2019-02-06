#include "mvvm/jerryscript/model_jerryscript.h"
#include "mvvm/jerryscript/value_converter_jerryscript.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ValueConverterJerryScript, basic) {
  const char* code =
      "var ValueConverters = {}; \
        ValueConverters.dummy = {\
          toView:function(v) {return v+1;}, \
          toModel:function(v) {return v-1} \
        }";
  value_t from;
  value_t to;
  value_converter_jerryscript_init();
  model_t* model = model_jerryscript_create("test", code, strlen(code));

  value_set_int(&from, 100);
  value_converter_t* c = value_converter_create("dummy");
  ASSERT_NE(c, VALUE_CONVERTER(NULL));

  ASSERT_EQ(value_converter_to_view(c, &from, &to), RET_OK);
  ASSERT_EQ(value_int(&from) + 1, value_int(&to));

  value_set_int(&from, 100);
  ASSERT_EQ(value_converter_to_model(c, &from, &to), RET_OK);
  ASSERT_EQ(value_int(&from) - 1, value_int(&to));

  value_converter_jerryscript_deinit();
  object_unref(OBJECT(model));
}

TEST(ValueConverterJerryScript, not_exist) {
  const char* code = "var ValueConverters = {};";

  value_converter_jerryscript_init();
  model_t* model = model_jerryscript_create("test", code, strlen(code));

  value_converter_t* c = value_converter_create("dummy");
  ASSERT_EQ(c, VALUE_CONVERTER(NULL));

  value_converter_jerryscript_deinit();
  object_unref(OBJECT(model));
}
