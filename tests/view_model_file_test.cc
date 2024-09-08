#include "mvvm/mvvm.h"
#include "mvvm/view_models/view_model_file.h"
#include "gtest/gtest.h"
#include "tkc/path.h"

TEST(ViewModelFile, basic) {
    char filename[256];
    navigator_request_t* req = navigator_request_create("name=aaa", NULL);
    view_model_t* vm = view_model_factory_create_model("file(path=${app_dir}/tests/test_data/hello.txt, auto_load=true)", req);
    ASSERT_EQ(vm != NULL, true);
    tk_object_t* obj = TK_OBJECT(vm);

    ASSERT_EQ(tk_object_get_prop_int(obj, "size", 0), 13);
    ASSERT_EQ(tk_str_start_with(tk_object_get_prop_str(obj, "content"), "Hello World!"), true);
    ASSERT_EQ(tk_object_get_prop_bool(obj, "auto_load", FALSE), true);
    ASSERT_EQ(tk_object_get_prop_bool(obj, "is_dirty", TRUE), false);

    path_expand_vars("${app_dir}/tests/test_data/hello.txt", filename, sizeof(filename)-1);
    ASSERT_STREQ(tk_object_get_prop_str(obj, "filename"), filename);

    TK_OBJECT_UNREF(vm);
}
