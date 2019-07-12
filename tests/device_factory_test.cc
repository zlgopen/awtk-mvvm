#include "gtest/gtest.h"
#include "tkc/object_default.h"
#include "mvvm/hardware/device_factory.h"

static object_t* dummy_device_create(const char* args) {
  return object_default_create();
}

TEST(DeviceFactory, basic) {
  object_t* device = NULL;
  device_factory_init();

  ASSERT_NE(device_factory_register(NULL, dummy_device_create), RET_OK);
  ASSERT_NE(device_factory_register("dummy", NULL), RET_OK);

  ASSERT_EQ(device_factory_has("dummy"), FALSE);
  ASSERT_EQ(device_factory_register("dummy", dummy_device_create), RET_OK);
  ASSERT_EQ(device_factory_has("dummy"), TRUE);
  ASSERT_EQ(device_factory_unregister("dummy"), RET_OK);
  ASSERT_EQ(device_factory_has("dummy"), FALSE);
  ASSERT_EQ(device_factory_register("dummy", dummy_device_create), RET_OK);

  device = device_factory_create_device("dummy", "default");
  ASSERT_EQ(device != NULL, TRUE);

  object_unref(device);

  device_factory_deinit();
}
