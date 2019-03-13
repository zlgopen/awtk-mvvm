#include "tkc/utils.h"
#include "mvvm/base/model_dummy.h"
#include "mvvm/base/model_array.h"
#include "mvvm/base/view_model_array.h"
#include "gtest/gtest.h"

TEST(ModelArray, basic) {
  value_t v;
  uint32_t i = 0;
  model_t* model = model_array_create(NULL);
  view_model_t* vm = view_model_array_create(model);

  for (i = 0; i < 100; i++) {
    char str[32];
    model_t* submodel = model_dummy_create(NULL);

    tk_snprintf(str, sizeof(str) - 1, "test%u", i);
    object_set_prop_int(OBJECT(submodel), "a", i);
    object_set_prop_int(OBJECT(submodel), "b", i * 2);
    object_set_prop_str(OBJECT(submodel), "c", str);
    model_array_add(model, submodel);

    ASSERT_EQ(view_model_eval(vm, "$item.a + $item.b", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 3 * i);

    ASSERT_EQ(view_model_eval(vm, "item.a", &v), RET_OK);
    ASSERT_EQ(value_int(&v), i);

    ASSERT_EQ(view_model_eval(vm, "item.c", &v), RET_OK);
    ASSERT_STREQ(value_str(&v), str);

    ASSERT_EQ(object_get_prop_int(OBJECT(vm), MODEL_PROP_ITEMS, -1), i + 1);
    ASSERT_EQ(object_get_prop_int(OBJECT(vm), MODEL_PROP_CURSOR, -1), i);
    ASSERT_EQ(object_get_prop_int(OBJECT(model), MODEL_PROP_ITEMS, -1), i + 1);

    view_model_array_inc_cursor(vm);
  }

  object_unref(OBJECT(vm));
}
