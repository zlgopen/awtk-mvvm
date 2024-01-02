#include "mvvm/mvvm.h"
#include "mvvm/view_models/view_model_file.h"
#include "gtest/gtest.h"
#include "tkc/path.h"

static void test_conf(const char* name, const char* args) {    
    navigator_request_t* req = navigator_request_create(args, NULL);
    view_model_t* vm = view_model_factory_create_model(name, req);
    ASSERT_EQ(vm != NULL, true);
    tk_object_t* obj = TK_OBJECT(vm);
    
    ASSERT_EQ(tk_object_set_prop_str(obj, "network.device", "eth2"), RET_OK);
    ASSERT_EQ(tk_object_set_prop_str(obj, "network.ip", "192.168.1.2"), RET_OK);
    
    ASSERT_STREQ(tk_object_get_prop_str(obj, "network.device"), "eth2");
    ASSERT_STREQ(tk_object_get_prop_str(obj, "network.ip"), "192.168.1.2");

    ASSERT_EQ(tk_object_set_prop_str(obj, "network.device", "eth0"), RET_OK);
    ASSERT_EQ(tk_object_set_prop_str(obj, "network.ip", "192.168.1.1"), RET_OK);
    
    ASSERT_STREQ(tk_object_get_prop_str(obj, "network.device"), "eth0");
    ASSERT_STREQ(tk_object_get_prop_str(obj, "network.ip"), "192.168.1.1");

    TK_OBJECT_UNREF(vm);
}

static void test_conf_array(const char* name, const char* args) {    
    navigator_request_t* req = navigator_request_create(args, NULL);
    view_model_t* vm = view_model_factory_create_model(name, req);
    ASSERT_EQ(vm != NULL, true);
    tk_object_t* obj = TK_OBJECT(vm);
    
    ASSERT_EQ(tk_object_set_prop_str(obj, "[0].device", "eth2"), RET_OK);
    ASSERT_EQ(tk_object_set_prop_str(obj, "[0].ip", "192.168.1.2"), RET_OK);
    
    ASSERT_STREQ(tk_object_get_prop_str(obj, "[0].device"), "eth2");
    ASSERT_STREQ(tk_object_get_prop_str(obj, "[0].ip"), "192.168.1.2");

    ASSERT_EQ(tk_object_set_prop_str(obj, "[0].device", "eth0"), RET_OK);
    ASSERT_EQ(tk_object_set_prop_str(obj, "[0].ip", "192.168.1.1"), RET_OK);
    
    ASSERT_STREQ(tk_object_get_prop_str(obj, "[0].device"), "eth0");
    ASSERT_STREQ(tk_object_get_prop_str(obj, "[0].ip"), "192.168.1.1");

    TK_OBJECT_UNREF(vm);
}

TEST(ViewModelConf, ini1) {
  test_conf("ini(url=${app_dir}/demos/demo_conf/demo.ini)", "aaa=bbb");
}

TEST(ViewModelConf, ini2) {
  test_conf("ini()", "target?url=${app_dir}/demos/demo_conf/demo.ini");
}

TEST(ViewModelConf, json1) {
  test_conf("json(url=${app_dir}/demos/demo_conf/demo.json)", "aaa=bbb");
}

TEST(ViewModelConf, json2) {
  test_conf("json()", "target?url=${app_dir}/demos/demo_conf/demo.json");
}

TEST(ViewModelConf, ubjson1) {
  test_conf("ubjson(url=${app_dir}/demos/demo_conf/demo.ubjson)", "aaa=bbb");
}

TEST(ViewModelConf, ubjson2) {
  test_conf("ubjson()", "target?url=${app_dir}/demos/demo_conf/demo.ubjson");
}


TEST(ViewModelConf, xml1) {
  test_conf("xml(url=${app_dir}/demos/demo_conf/demo.xml)", "aaa=bbb");
}

TEST(ViewModelConf, xml2) {
  test_conf("xml()", "target?url=${app_dir}/demos/demo_conf/demo.xml");
}

TEST(ViewModelConf, json_array1) {
  test_conf_array("json_array(url=${app_dir}/demos/demo_conf/demo_array.json, prefix=networks)", "aaa=bbb");
}

TEST(ViewModelConf, json_array2) {
  test_conf_array("json_array()", "target?url=${app_dir}/demos/demo_conf/demo_array.json&prefix=networks");
}

TEST(ViewModelConf, xml_array1) {
  test_conf_array("xml_array(url=${app_dir}/demos/demo_conf/demo_array.xml, prefix=networks)", "aaa=bbb");
}

TEST(ViewModelConf, xml_array2) {
  test_conf_array("xml_array()", "target?url=${app_dir}/demos/demo_conf/demo_array.xml&prefix=networks");
}

TEST(ViewModelConf, csv1) {
  test_conf_array("csv(url=${app_dir}/demos/demo_conf/demo.csv, col_names='device,ip')", "aaa=bbb");
}

TEST(ViewModelConf, csv2) {
  test_conf_array("csv()", "target?url=${app_dir}/demos/demo_conf/demo.csv&col_names='device,ip'");
}
