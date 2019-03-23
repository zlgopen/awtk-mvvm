#include "mvvm/jerryscript/view_model_jerryscript.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ValueValidatorJerryScript, basic) {
  const char* code =
      "var ValueValidators = {}; \
        ValueValidators.jsdummy = {\
          isValid:function(v) {return v < 10;} \
        }";
  str_t str;
  value_t value;
  view_model_t* view_model = view_model_jerryscript_create("test", code, strlen(code), NULL);

  str_init(&str, 0);
  value_validator_t* c = value_validator_create("jsdummy");
  ASSERT_NE(c, VALUE_VALIDATOR(NULL));

  value_set_int(&value, 1);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), TRUE);

  value_set_int(&value, 5);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), TRUE);

  value_set_int(&value, 10);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);

  value_set_int(&value, 100);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);

  str_reset(&str);
  object_unref(OBJECT(c));
  object_unref(OBJECT(view_model));
}

TEST(ValueValidatorJerryScript, fix) {
  const char* code =
      "var ValueValidators = {}; \
        ValueValidators.jsdummy = {\
          fix:function(v) {return 10;} \
        }";
  value_t value;
  view_model_t* view_model = view_model_jerryscript_create("test", code, strlen(code), NULL);

  value_validator_t* c = value_validator_create("jsdummy");
  ASSERT_NE(c, VALUE_VALIDATOR(NULL));

  value_set_int(&value, 10);
  ASSERT_EQ(value_validator_fix(c, &value), RET_OK);
  ASSERT_EQ(value_int(&value), 10);

  value_set_int(&value, 100);
  ASSERT_EQ(value_validator_fix(c, &value), RET_OK);
  ASSERT_EQ(value_int(&value), 10);

  value_set_int(&value, 1);
  ASSERT_EQ(value_validator_fix(c, &value), RET_OK);
  ASSERT_EQ(value_int(&value), 10);

  object_unref(OBJECT(c));
  object_unref(OBJECT(view_model));
}

TEST(ValueValidatorJerryScript, message) {
  const char* code =
      "var ValueValidators = {}; \
        ValueValidators.jsdummy = {\
          isValid:function(v) {\
            if(v < 10) return {result:true, message:'ok'}; \
            else return {result:false, message:'too large'};} \
        }";
  str_t str;
  value_t value;
  view_model_t* view_model = view_model_jerryscript_create("test", code, strlen(code), NULL);

  str_init(&str, 0);
  value_validator_t* c = value_validator_create("jsdummy");
  ASSERT_NE(c, VALUE_VALIDATOR(NULL));

  value_set_int(&value, 1);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), TRUE);
  ASSERT_EQ(string(str.str), string("ok"));
  value_set_int(&value, 5);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), TRUE);
  ASSERT_EQ(string(str.str), string("ok"));

  value_set_int(&value, 10);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);
  ASSERT_EQ(string(str.str), string("too large"));

  value_set_int(&value, 100);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);
  ASSERT_EQ(string(str.str), string("too large"));

  str_reset(&str);
  object_unref(OBJECT(c));
  object_unref(OBJECT(view_model));
}

TEST(ValueValidatorJerryScript, not_exist) {
  const char* code = "var ValueValidators = {};";

  view_model_t* view_model = view_model_jerryscript_create("test", code, strlen(code), NULL);

  value_validator_t* c = value_validator_create("notexist");
  ASSERT_EQ(c, VALUE_VALIDATOR(NULL));

  object_unref(OBJECT(view_model));
}

TEST(ValueValidatorJerryScript, no_isValid) {
  const char* code =
      "var ValueValidators = {}; \
        ValueValidators.jsdummy = {\
          isValid:1 \
        }";
  str_t str;
  value_t value;
  view_model_t* view_model = view_model_jerryscript_create("test", code, strlen(code), NULL);
  value_validator_t* c = value_validator_create("notexist");

  str_init(&str, 0);
  value_set_int(&value, 1);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);

  str_reset(&str);
  object_unref(OBJECT(c));
  object_unref(OBJECT(view_model));
}
