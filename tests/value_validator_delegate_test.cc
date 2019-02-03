#include "tkc/utils.h"
#include "mvvm/base/value_validator_delegate.h"
#include "gtest/gtest.h"

#include <string>

using std::string;

static bool_t is_valid_age(const value_t* value, str_t* msg) {
  int32_t age = value_int(value);

  if (age > 0 && age < 150) {
    return TRUE;
  } else {
    str_set(msg, "error");
    return FALSE;
  }
}

static ret_t fix_age(value_t* value) {
  int32_t age = value_int(value);

  if (age < 0) {
    value_set_int(value, 0);
  } else if (age > 150) {
    value_set_int(value, 150);
  }

  return RET_OK;
}

TEST(ValueValidatorDelegate, basic) {
  value_t v;
  str_t str;
  value_validator_t* c = value_validator_delegate_create(is_valid_age, fix_age);

  str_init(&str, 0);
  value_set_int(&v, 1234);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), FALSE);
  ASSERT_EQ(string(str.str), "error");

  value_set_int(&v, 123);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), TRUE);

  str_reset(&str);
  object_unref(OBJECT(c));
}

TEST(ValueValidatorDelegate, fix) {
  value_t v;
  value_validator_t* c = value_validator_delegate_create(is_valid_age, fix_age);

  value_set_int(&v, -10);
  ASSERT_EQ(value_validator_fix(c, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);
  
  value_set_int(&v, 1000);
  ASSERT_EQ(value_validator_fix(c, &v), RET_OK);
  ASSERT_EQ(value_int(&v), 150);

  object_unref(OBJECT(c));
}

static void* create_dummy_value_validator(void) {
  return value_validator_delegate_create(is_valid_age, fix_age);
}

TEST(ValueValidatorDelegate, factory) {
  value_t v;
  str_t str;
  value_validator_init();
  ASSERT_EQ(value_validator_register("dummy", create_dummy_value_validator), RET_OK);
  value_validator_t* c = value_validator_create("dummy");

  ASSERT_NE(c, (value_validator_t*)NULL);

  str_init(&str, 0);
  value_set_int(&v, 1234);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), FALSE);
  ASSERT_EQ(string(str.str), "error");

  value_set_int(&v, 123);
  ASSERT_EQ(value_validator_is_valid(c, &v, &str), TRUE);

  object_unref(OBJECT(c));
  value_validator_deinit();
}
