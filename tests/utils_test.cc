#include "mvvm/base/utils.h"
#include "gtest/gtest.h"

TEST(Utils, basic) {
  uint32_t index = 0;

  ASSERT_STREQ(tk_destruct_array_prop_name("[123].name", &index), "name");
  ASSERT_EQ(index, 123u);

  ASSERT_STREQ(tk_destruct_array_prop_name("[0].a", &index), "a");
  ASSERT_EQ(index, 0u);
}
