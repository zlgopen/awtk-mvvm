#include "gtest/gtest.h"
#include "tkc/object_default.h"
#include "mvvm/awtk/widget_hardware.h"
#include "mvvm/hardware/device_factory.h"

static object_t* dummy_device_create(const char* args) {
  return object_default_create();
}

TEST(WidgetHardware, basic) {
  widget_t* w = NULL;
  device_factory_init();

  ASSERT_EQ(device_factory_register("dummy", dummy_device_create), RET_OK);

  w = widget_hardware_create(NULL, 0, 0, 0, 0, "dummy", NULL);
  ASSERT_EQ(w != NULL, TRUE);
  ASSERT_STREQ(widget_get_type(w), "dummy");

  widget_destroy(w);
  device_factory_deinit();
}
