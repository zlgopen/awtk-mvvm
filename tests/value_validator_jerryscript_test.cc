#include "mvvm/base/view_model_factory.h"
#include "mvvm/jerryscript/jerry_script_helper.h"
#include "mvvm/jerryscript/value_validator_jerryscript.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ValueValidatorJerryScript, basic) {
  const char* code =
      "ValueValidator('jsdummy', {\
         isValid: function(v) {return v < 10;} \
       });";
  str_t str;
  value_t value;
  value_validator_t* c;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  c = value_validator_create("jsdummy");
  ASSERT_NE(c, VALUE_VALIDATOR(NULL));

  str_init(&str, 0);

  value_set_int(&value, 1);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), TRUE);

  value_set_int(&value, 5);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), TRUE);

  value_set_int(&value, 10);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);

  value_set_int(&value, 100);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);

  str_reset(&str);
  tk_object_unref(TK_OBJECT(c));
}

TEST(ValueValidatorJerryScript, fix) {
  const char* code =
      "ValueValidator('jsdummy', {\
         fix: function(v) {return 10;} \
       });";
  value_t value;
  value_validator_t* c;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  c = value_validator_create("jsdummy");
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

  tk_object_unref(TK_OBJECT(c));
}

TEST(ValueValidatorJerryScript, message) {
  const char* code =
      "ValueValidator('jsdummy', {\
          isValid:function(v) {\
            if(v < 10) return {result:true, message:'ok'}; \
            else return {result:false, message:'too large'};} \
       });";
  str_t str;
  value_t value;
  value_validator_t* c;

  jerry_script_eval_buff(code, strlen(code), "test", TRUE);
  c = value_validator_create("jsdummy");
  ASSERT_NE(c, VALUE_VALIDATOR(NULL));

  str_init(&str, 0);

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
  tk_object_unref(TK_OBJECT(c));
}

TEST(ValueValidatorJerryScript, not_exist) {
  value_validator_t* c = value_validator_create("notexist");
  ASSERT_EQ(c, VALUE_VALIDATOR(NULL));
}

TEST(ValueValidatorJerryScript, no_isValid) {
  str_t str;
  value_t value;
  value_validator_t* c = value_validator_create("notexist");

  str_init(&str, 0);
  value_set_int(&value, 1);
  ASSERT_EQ(value_validator_is_valid(c, &value, &str), FALSE);

  str_reset(&str);
  tk_object_unref(TK_OBJECT(c));
}
