#include "gtest/gtest.h"
#include "awtk.h"
#include "tkc/object_default.h"
#include "mvvm/base/custom_binder.h"

static ret_t custom_binder_dummy(void* widget, binding_context_t* binding_context) {
  return RET_OK;
}

TEST(CustomBinder, basic) {
  custom_binder_init();
  ASSERT_EQ(custom_binder_exist("dummy"), FALSE);
  ASSERT_EQ(custom_binder_register("dummy", custom_binder_dummy), RET_OK);
  ASSERT_EQ(custom_binder_exist("dummy"), TRUE);

  ASSERT_EQ(custom_binder_unregister("dummy"), RET_OK);
  ASSERT_EQ(custom_binder_exist("dummy"), FALSE);

  custom_binder_deinit();
}
