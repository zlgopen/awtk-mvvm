#include "gtest/gtest.h"
#include "awtk.h"
#include "tkc/object_default.h"
#include "mvvm/base/custom_binder.h"

static ret_t custom_binder_dummy(binding_context_t* ctx, binding_rule_t* rule) {
  return RET_OK;
}

TEST(CustomBinder, basic) {
  ASSERT_EQ(custom_binder_exist("dummy"), FALSE);
  ASSERT_EQ(custom_binder_register("dummy", custom_binder_dummy), RET_OK);
  ASSERT_EQ(custom_binder_exist("dummy"), TRUE);

  ASSERT_EQ(custom_binder_unregister("dummy"), RET_OK);
  ASSERT_EQ(custom_binder_exist("dummy"), FALSE);
}
