#include "mvvm/base/view_model_array_object_wrapper.h"
#include "tkc/object_default.h"
#include "tkc/object_array.h"
#include "gtest/gtest.h"

static tk_object_t* create_array_object() {
    value_t v;
    tk_object_t* o1 = object_default_create();
    tk_object_t* o2 = object_default_create();
    tk_object_t* o3 = object_default_create();
    tk_object_t* arr = object_array_create();

    tk_object_set_prop_str(o1, "name", "o1");
    tk_object_set_prop_str(o2, "name", "o2");
    tk_object_set_prop_str(o3, "name", "o3");
    tk_object_set_prop_int(o1, "int", 1);
    tk_object_set_prop_int(o2, "int", 2);
    tk_object_set_prop_int(o3, "int", 3);

    value_set_object(&v, o1);
    object_array_push(arr, &v);

    value_set_object(&v, o2);
    object_array_push(arr, &v);

    value_set_object(&v, o3);
    object_array_push(arr, &v);

    TK_OBJECT_UNREF(o1);
    TK_OBJECT_UNREF(o2);
    TK_OBJECT_UNREF(o3);

    return arr;
}

TEST(ViewModelObjectArrayWrapper, basic1) {
    value_t v;
    tk_object_t* o = create_array_object();
    view_model_t* vm = view_model_array_object_wrapper_create(o);

    ASSERT_EQ(view_model_get_prop(vm, "0.name", &v), RET_OK);
    ASSERT_STREQ(value_str(&v), "o1");
    
    ASSERT_EQ(view_model_get_prop(vm, "1.name", &v), RET_OK);
    ASSERT_STREQ(value_str(&v), "o2");
    
    ASSERT_EQ(view_model_get_prop(vm, "2.name", &v), RET_OK);
    ASSERT_STREQ(value_str(&v), "o3");

    ASSERT_EQ(view_model_get_prop(vm, TK_OBJECT_PROP_SIZE, &v), RET_OK);
    ASSERT_EQ(value_int(&v), 3);

    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}

TEST(ViewModelObjectArrayWrapper, basic2) {
    value_t v;
    tk_object_t* o = create_array_object();
    view_model_t* vm = view_model_array_object_wrapper_create(o);

    value_set_int(&v, 100);
    ASSERT_EQ(view_model_set_prop(vm, "0.int", &v), RET_OK);
    ASSERT_EQ(tk_object_get_prop_int32(o, "0.int", 0), 100);
    
    value_set_int(&v, 200);
    ASSERT_EQ(view_model_set_prop(vm, "1.int", &v), RET_OK);
    ASSERT_EQ(tk_object_get_prop_int32(o, "1.int", 0), 200);

    value_set_int(&v, 300);
    ASSERT_EQ(view_model_set_prop(vm, "1.int", &v), RET_OK);
    ASSERT_EQ(tk_object_get_prop_int32(o, "1.int", 0), 300);
    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}

TEST(ViewModelObjectArrayWrapper, selected) {
    value_t v;
    tk_object_t* o = create_array_object();
    view_model_t* vm = view_model_array_object_wrapper_create(o);

    value_set_int(&v, 0);
    ASSERT_EQ(view_model_set_prop(vm, VIEW_MODEL_PROP_SELECTED_INDEX, &v), RET_OK);
    ASSERT_EQ(view_model_get_prop(vm, "selected.int", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 1);
    value_set_int(&v, 111);
    ASSERT_EQ(view_model_set_prop(vm, "selected.int", &v), RET_OK);
    ASSERT_EQ(tk_object_get_prop_int32(o, "0.int", 0), 111);
    
    value_set_int(&v, 1);
    ASSERT_EQ(view_model_set_prop(vm, VIEW_MODEL_PROP_SELECTED_INDEX, &v), RET_OK);
    ASSERT_EQ(view_model_get_prop(vm, "selected.int", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 2);
    value_set_int(&v, 222);
    ASSERT_EQ(view_model_set_prop(vm, "selected.int", &v), RET_OK);
    ASSERT_EQ(tk_object_get_prop_int32(o, "1.int", 0), 222);
    
    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}