#include "tkc/utils.h"
#include "mvvm/base/model_array.h"
#include "mvvm/base/view_model_array.h"
#include "gtest/gtest.h"

TEST(ViewModelArray, basic) {
  model_t* model = model_array_create(NULL);
  view_model_t* vm = view_model_array_create(model);
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(vm);

  ASSERT_STREQ(view_model_preprocess_expr(vm, "item.a * item.b"), "$item_a * $item_b");
  ASSERT_STREQ(view_model_preprocess_expr(vm, "$item.a * $item.b"), "$item_a * $item_b");

  vm_array->cursor = 1;
  ASSERT_STREQ(view_model_preprocess_prop(vm, "item_a"), "[1].a");
  ASSERT_STREQ(view_model_preprocess_prop(vm, "item_b"), "[1].b");

  vm_array->cursor = 10;
  ASSERT_STREQ(view_model_preprocess_prop(vm, "item_a"), "[10].a");
  ASSERT_STREQ(view_model_preprocess_prop(vm, "item_b"), "[10].b");

  object_unref(OBJECT(vm));
}
