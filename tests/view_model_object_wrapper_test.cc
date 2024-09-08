#include "mvvm/base/view_model_object_wrapper.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

#include <string>
using std::string;

TEST(ViewModelObjectWrapper, basic) {
    value_t v;
    tk_object_t* o = object_default_create();
    view_model_t* vm = view_model_object_wrapper_create(o);

    tk_object_set_prop_bool(o, "bool", true);
    tk_object_set_prop_int(o, "int", 1);
    tk_object_set_prop_double(o, "double", 1.0);
    tk_object_set_prop_str(o, "string", "string");

    ASSERT_EQ(view_model_get_prop(vm, "bool", &v), RET_OK);
    ASSERT_EQ(value_bool(&v), true);

    ASSERT_EQ(view_model_get_prop(vm, "int", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 1);

    ASSERT_EQ(view_model_get_prop(vm, "double", &v), RET_OK);
    ASSERT_EQ(value_double(&v), 1.0);

    ASSERT_EQ(view_model_get_prop(vm, "string", &v), RET_OK);
    ASSERT_EQ(strcmp(value_str(&v), "string"), 0);

    tk_object_set_prop_int(TK_OBJECT(vm), "int", 10);
    ASSERT_EQ(tk_object_get_prop(o, "int", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 10);

    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}

TEST(ViewModelObjectWrapper, prefix) {
    value_t v;
    tk_object_t* o = object_default_create();
    view_model_t* vm = view_model_object_wrapper_create_ex(o, "prefix");

    tk_object_set_prop_bool(o, "prefix.bool", true);
    tk_object_set_prop_int(o, "prefix.int", 1);
    tk_object_set_prop_double(o, "prefix.double", 1.0);
    tk_object_set_prop_str(o, "prefix.string", "string");

    ASSERT_EQ(view_model_get_prop(vm, "bool", &v), RET_OK);
    ASSERT_EQ(value_bool(&v), true);

    ASSERT_EQ(view_model_get_prop(vm, "int", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 1);

    ASSERT_EQ(view_model_get_prop(vm, "double", &v), RET_OK);
    ASSERT_EQ(value_double(&v), 1.0);

    ASSERT_EQ(view_model_get_prop(vm, "string", &v), RET_OK);
    ASSERT_EQ(strcmp(value_str(&v), "string"), 0);

    tk_object_set_prop_int(TK_OBJECT(vm), "int", 10);
    ASSERT_EQ(tk_object_get_prop(o, "prefix.int", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 10);

    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}

static ret_t on_prop_changed(void* ctx, event_t* e) {
    str_t* str = (str_t*)ctx;
    char buff[32] = {0};
    prop_change_event_t* evt = (prop_change_event_t*)e;

    str_append(str, evt->name);
    str_append(str, "=");
    str_append(str, value_str_ex((value_t*)(evt->value), buff, sizeof(buff)));
    str_append(str, ";");

    return RET_OK;
}

TEST(ViewModelObjectWrapper, prop_changed) {
    str_t str;
    tk_object_t* o = object_default_create();
    view_model_t* vm = view_model_object_wrapper_create_ex(o, "prefix");

    str_init(&str, 100);
    emitter_on(EMITTER(vm), EVT_PROP_CHANGED, on_prop_changed, &str);
    tk_object_set_prop_bool(o, "prefix.bool", true);
    ASSERT_STREQ(str.str, "prefix.bool=true;");
    str_reset(&str);

    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}

static ret_t on_cmd_exec(void* ctx, event_t* e) {
    str_t* str = (str_t*)ctx;
    cmd_exec_event_t* evt = (cmd_exec_event_t*)e;

    str_append(str, evt->name);
    str_append(str, "=");
    str_append(str, evt->args);
    str_append(str, ";");

    return RET_OK;
}

TEST(ViewModelObjectWrapper, cmd_exec) {
    str_t str;
    tk_object_t* o = object_default_create();
    view_model_t* vm = view_model_object_wrapper_create_ex(o, "prefix");

    str_init(&str, 100);
    emitter_on(EMITTER(vm), EVT_CMD_CAN_EXEC, on_cmd_exec, &str);
    emitter_on(EMITTER(vm), EVT_CMD_WILL_EXEC, on_cmd_exec, &str);
    emitter_on(EMITTER(vm), EVT_CMD_EXECED, on_cmd_exec, &str);

    view_model_can_exec(vm, "cmd1", "args1");
    ASSERT_STREQ(str.str, "cmd1=args1;");
    
    
    str_clear(&str);
    view_model_exec(vm, "cmd2", "args2");
    ASSERT_STREQ(str.str, "cmd2=args2;cmd2=args2;");
    str_reset(&str);


    TK_OBJECT_UNREF(o);
    TK_OBJECT_UNREF(vm);
}
