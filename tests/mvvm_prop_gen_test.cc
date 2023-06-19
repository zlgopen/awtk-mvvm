#include <string>
#include <iostream>
#include "tkc/fs.h"
#include "gtest/gtest.h"
#include "mvvm_prop_gen.h"
#include "xml_mvvm_prop.h"

using std::cout;
using std::endl;
using std::string;

static string res_to_str(mvvm_prop_result_t* mvvm_prop_result) {
  string prop;
  string result;

  if (mvvm_prop_result->prop_len > 0) {
    prop += mvvm_prop_result->prop;
  }

  while (mvvm_prop_result->group_head != NULL) {
    if (mvvm_prop_result->group_head->path_len > 0) {
      result += mvvm_prop_result->group_head->path;
      if (mvvm_prop_result->group_head->is_array) {
        result += ".[].";
      } else {
        result += ".";
      }
      result += prop;
    }
    result += " ";
    mvvm_prop_result->group_head = mvvm_prop_result->group_head->next;
  }

  /* dummy */
  if (mvvm_prop_result->group_size == 0) {
    result += prop;
  }
  return result;
}

TEST(mvvm_prop_gen, basic_use) {
  ret_t ret;
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;

  prop_array = xml_mvvm_prop_file_to_array("./design/default/ui/address.xml", &prop_size);
  ASSERT_EQ(prop_size > 0 && prop_array != NULL, TRUE);

  if (prop_array != NULL) {
    int i = 0;
    for (i = 0; i < prop_size; i++) {
      cout << res_to_str(prop_array[i]) << endl;

      ASSERT_EQ(prop_array[i]->group_size > 0, TRUE);
      ASSERT_EQ(strlen(prop_array[i]->prop), prop_array[i]->prop_len);
    }
  }
  ret = mvvm_prop_gen_result_destory(prop_array, prop_size);
  ASSERT_EQ(ret, RET_OK);
}

TEST(mvvm_prop_gen, basic_file) {
  ret_t ret;
  uint32_t i;
  uint32_t prop_size = 0;
  mvvm_prop_result_t** prop_array;

  ret = xml_mvvm_prop_file_to_packfile("./design/default/ui/app_conf_reload.xml", "./design/default/ui/basic_file_test.json");
  ASSERT_EQ(ret, RET_OK);

  prop_array = xml_mvvm_prop_packfile_to_array("./design/default/ui/basic_file_test.json", &prop_size);
  ASSERT_EQ(prop_size > 0 && prop_array != NULL, TRUE);
  if (prop_array != NULL) {
    int i = 0;
    for (i = 0; i < prop_size; i++) {
      cout << res_to_str(prop_array[i]) << endl;

      ASSERT_EQ(prop_array[i]->group_size > 0, TRUE);
      ASSERT_EQ(strlen(prop_array[i]->prop), prop_array[i]->prop_len);
    }
  }
  mvvm_prop_gen_result_destory(prop_array, prop_size);
  ret = file_remove("./design/default/ui/basic_file_test.json");
  ASSERT_EQ(ret, RET_OK);
}

TEST(mvvm_prop_gen, basic) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<window v-model=\"user_message\">"
                      "<input v-data:text=\"{username}\"/>"
                      "<input v-data:text=\"{password}\"/>"
                      "<input v-data:text=\"{phone}\"/>"
                    "</window>"
                    "<window v-model=\"score_table\">"
                      "<input v-data:text=\"{math}\"/>"
                      "<input v-data:text=\"{english}\"/>"
                    "</window>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 5);
  ASSERT_EQ(res_to_str(prop_array[0]), "user_message.username ");
  ASSERT_EQ(res_to_str(prop_array[1]), "user_message.password ");
  ASSERT_EQ(res_to_str(prop_array[2]), "user_message.phone ");
  ASSERT_EQ(res_to_str(prop_array[3]), "score_table.math ");
  ASSERT_EQ(res_to_str(prop_array[4]), "score_table.english ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, sub_view_model) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<window v-model=\"form\">"
                      "<form v-model=\"sub_view_model:user_message\">"
                        "<input v-data:text=\"{username}\"/>"
                        "<input v-data:text=\"{password}\"/>"
                        "<input v-data:text=\"{phone}\"/>"
                      "</form>"
                      "<form v-model=\"sub_view_model:score_table\">"
                        "<input v-data:text=\"{math}\"/>"
                        "<input v-data:text=\"{english}\"/>"
                      "</form>"
                    "</window>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 5);
  ASSERT_EQ(res_to_str(prop_array[0]), "form.user_message.username form.username ");
  ASSERT_EQ(res_to_str(prop_array[1]), "form.user_message.password form.password ");
  ASSERT_EQ(res_to_str(prop_array[2]), "form.user_message.phone form.phone ");
  ASSERT_EQ(res_to_str(prop_array[3]), "form.score_table.math form.math ");
  ASSERT_EQ(res_to_str(prop_array[4]), "form.score_table.english form.english ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, scope) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view v-model=\"weather\">"
                      "<label v-data:text=\"{today}\"/>"
                        "<view v-model=\"temperature\">"
                          "<label v-data:text=\"{temp_high}\"/>"
                          "<label v-data:text=\"{temp_low}\"/>"
                          "<view v-model=\"result\">"
                            "<label v-data:text=\"{is_true}\"/>"
                          "</view>"
                        "</view>"
                        "<view v-model=\"humidity\">"
                          "<label v-data:text=\"{hum_high}\"/>"
                          "<label v-data:text=\"{hum_low}\"/>"
                          "<view v-model=\"sub_view_model:result\">"
                            "<label v-data:text=\"{is_true}\"/>"
                          "</view>"
                        "</view>"
                      "<label v-data:text=\"{tomorrow}\"/>"
                        "<view v-model=\"sub_view_model:temperature\">"
                          "<label v-data:text=\"{temp_high}\"/>"
                          "<label v-data:text=\"{temp_low}\"/>"
                        "</view>"
                        "<view v-model=\"sub_view_model:humidity\">"
                          "<label v-data:text=\"{hum_high}\"/>"
                          "<label v-data:text=\"{hum_low}\"/>"
                        "</view>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 12);
  ASSERT_EQ(res_to_str(prop_array[0]), "weather.today ");
  ASSERT_EQ(res_to_str(prop_array[1]), "temperature.temp_high weather.temp_high ");
  ASSERT_EQ(res_to_str(prop_array[2]), "temperature.temp_low weather.temp_low ");
  ASSERT_EQ(res_to_str(prop_array[3]), "result.is_true temperature.is_true weather.is_true ");

  ASSERT_EQ(res_to_str(prop_array[4]), "humidity.hum_high weather.hum_high ");
  ASSERT_EQ(res_to_str(prop_array[5]), "humidity.hum_low weather.hum_low ");
  ASSERT_EQ(res_to_str(prop_array[6]), "humidity.result.is_true humidity.is_true weather.is_true ");

  ASSERT_EQ(res_to_str(prop_array[7]), "weather.tomorrow ");
  ASSERT_EQ(res_to_str(prop_array[8]), "weather.temperature.temp_high weather.temp_high ");
  ASSERT_EQ(res_to_str(prop_array[9]), "weather.temperature.temp_low weather.temp_low ");
  ASSERT_EQ(res_to_str(prop_array[10]), "weather.humidity.hum_high weather.hum_high ");
  ASSERT_EQ(res_to_str(prop_array[11]), "weather.humidity.hum_low weather.hum_low ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, dummy_view_model) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view>"
                      "<label v-data:text=\"{time}\"/>"
                      "<slider v-data:value=\"{value}\"/>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 2);
  ASSERT_EQ(res_to_str(prop_array[0]), "time");
  ASSERT_EQ(res_to_str(prop_array[1]), "value");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, multi) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<user_input v-data=\"data\" v-model=\"user_message\" v-data=\"{value}\">"
                      "<view v-data:text=\"{time}\" v-model=\"family_address\">"
                        "<label v-data:text=\"{address}\"/>"
                      "</view>"
                    "</user_input>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 4);
  ASSERT_EQ(res_to_str(prop_array[0]), "data");
  ASSERT_EQ(res_to_str(prop_array[1]), "user_message.value ");
  ASSERT_EQ(res_to_str(prop_array[2]), "user_message.time ");
  ASSERT_EQ(res_to_str(prop_array[3]), "family_address.address user_message.address ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, array) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view v-model=\"app_conf\">"
                      "<label v-data:text=\"{[0].color}\"/>"
                      "<view v-model=\"sub_view_model_array:font\">"
                        "<label v-data:text=\"{name}\"/>"
                        "<label v-data:text=\"{[0]}\"/>"
                      "</view>"
                      "<view v-model=\"sub_view_model:book\">"
                        "<label v-data:text=\"{page.[1]}\"/>"
                      "</view>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 4);
  ASSERT_EQ(res_to_str(prop_array[0]), "app_conf.[0].color ");
  ASSERT_EQ(res_to_str(prop_array[1]), "app_conf.font.name app_conf.name ");
  ASSERT_EQ(res_to_str(prop_array[2]), "app_conf.font.[0] ");
  ASSERT_EQ(res_to_str(prop_array[3]), "app_conf.book.page.[1] ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, v_for_empty) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view v-model=\"app_conf\">"
                      "<view v-model=\"sub_view_model_array:network\">"
                        "<page v-for-items=\"true\">"
                          "<view>"
                            "<edit v-data:value=\"{item.name}\"/>"
                            "<edit v-data:value=\"{item.ip}\"/>"
                            "<edit v-data:value=\"{item.mask}\"/>"
                            "<edit v-data:value=\"{item.gateway}\"/>"
                          "</view>"
                        "</page>"
                      "</view>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 4);
  ASSERT_EQ(res_to_str(prop_array[0]), "app_conf.network.[].name ");
  ASSERT_EQ(res_to_str(prop_array[1]), "app_conf.network.[].ip ");
  ASSERT_EQ(res_to_str(prop_array[2]), "app_conf.network.[].mask ");
  ASSERT_EQ(res_to_str(prop_array[3]), "app_conf.network.[].gateway ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);

  const char* xml1 = "<view v-model=\"app_conf\">"
                      "<page v-for=\"{}\">"
                        "<view>"
                          "<edit v-data:value=\"{item.name}\"/>"
                          "<edit v-data:value=\"{item.ip}\"/>"
                          "<edit v-data:value=\"{item.mask}\"/>"
                          "<edit v-data:value=\"{item.gateway}\"/>"
                        "</view>"
                      "</page>"
                     "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml1, &prop_size);
  ASSERT_EQ(prop_size, 4);
  ASSERT_EQ(res_to_str(prop_array[0]), "app_conf.[].name ");
  ASSERT_EQ(res_to_str(prop_array[1]), "app_conf.[].ip ");
  ASSERT_EQ(res_to_str(prop_array[2]), "app_conf.[].mask ");
  ASSERT_EQ(res_to_str(prop_array[3]), "app_conf.[].gateway ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, v_for_item_1) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view v-model=\"app_conf\">"
                      "<page v-for=\"{vm_name, Index=vm_index, Item=message}\">"
                        "<view>"
                          "<label v-data:text=\"{vm_index}\"/>"
                          "<edit v-data:value=\"{message.name}\"/>"
                          "<edit v-data:value=\"{message.ip}\"/>"
                          "<edit v-data:value=\"{message.mask}\"/>"
                          "<edit v-data:value=\"{message.gateway}\"/>"
                        "</view>"
                      "</page>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);
  ASSERT_EQ(prop_size, 5);
  ASSERT_EQ(res_to_str(prop_array[0]), "app_conf.vm_name ");
  ASSERT_EQ(res_to_str(prop_array[1]), "app_conf.vm_name.[].name ");
  ASSERT_EQ(res_to_str(prop_array[2]), "app_conf.vm_name.[].ip ");
  ASSERT_EQ(res_to_str(prop_array[3]), "app_conf.vm_name.[].mask ");
  ASSERT_EQ(res_to_str(prop_array[4]), "app_conf.vm_name.[].gateway ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);

  const char* xml1 = "<view v-model=\"app_conf\">"
                      "<page v-for=\"{vm_name, Index=vm_index}\">"
                        "<view>"
                          "<label v-data:text=\"{vm_index}\"/>"
                          "<edit v-data:value=\"{item.name}\"/>"
                          "<edit v-data:value=\"{item.ip}\"/>"
                          "<edit v-data:value=\"{item.mask}\"/>"
                          "<edit v-data:value=\"{item.gateway}\"/>"
                        "</view>"
                      "</page>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml1, &prop_size);
  ASSERT_EQ(prop_size, 5);
  ASSERT_EQ(res_to_str(prop_array[0]), "app_conf.vm_name ");
  ASSERT_EQ(res_to_str(prop_array[1]), "app_conf.vm_name.[].name ");
  ASSERT_EQ(res_to_str(prop_array[2]), "app_conf.vm_name.[].ip ");
  ASSERT_EQ(res_to_str(prop_array[3]), "app_conf.vm_name.[].mask ");
  ASSERT_EQ(res_to_str(prop_array[4]), "app_conf.vm_name.[].gateway ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);

  const char* xml2 = "<view v-model=\"app_conf\">"
                      "<page v-for=\"{vm_name}\">"
                        "<view>"
                          "<label v-data:text=\"{index}\"/>"
                          "<edit v-data:value=\"{item.name}\"/>"
                          "<edit v-data:value=\"{item.ip}\"/>"
                          "<edit v-data:value=\"{item.mask}\"/>"
                          "<edit v-data:value=\"{item.gateway}\"/>"
                          "<edit v-data:value=\">"
                        "</view>"
                      "</page>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml2, &prop_size);
  ASSERT_EQ(prop_size, 5);
  ASSERT_EQ(res_to_str(prop_array[0]), "app_conf.vm_name ");
  ASSERT_EQ(res_to_str(prop_array[1]), "app_conf.vm_name.[].name ");
  ASSERT_EQ(res_to_str(prop_array[2]), "app_conf.vm_name.[].ip ");
  ASSERT_EQ(res_to_str(prop_array[3]), "app_conf.vm_name.[].mask ");
  ASSERT_EQ(res_to_str(prop_array[4]), "app_conf.vm_name.[].gateway ");
  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

#if 0
TEST(mvvm_prop_gen, not_impl_for_bind_data) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view>"
                      "<label v-data:text=\"{phone == tel}\"/>"
                      "<slider v-data:value=\"{value = number > 50 ? number : 0}\"/>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);

  /* maybe if true */
  // ASSERT_EQ(prop_size, 4);
  // ASSERT_EQ(res_to_str(prop_array[0]), "phone ");
  // ASSERT_EQ(res_to_str(prop_array[1]), "tel ");
  // ASSERT_EQ(res_to_str(prop_array[2]), "value ");
  // ASSERT_EQ(res_to_str(prop_array[3]), "number ");

  /* now return */
  ASSERT_EQ(prop_size, 2);
  ASSERT_EQ(res_to_str(prop_array[0]), "phone == tel");
  ASSERT_EQ(res_to_str(prop_array[1]), "value = number > 50 ? number : 0");

  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, not_impl_for_fscript) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view>"
                      "<label v-data:text=\"{bool, bool = number > 50 ? 'true' : 'false'}\"/>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);

  /* maybe if true */
  // ASSERT_EQ(prop_size, 2);
  // ASSERT_EQ(res_to_str(prop_array[0]), "bool ");
  // ASSERT_EQ(res_to_str(prop_array[1]), "number ");

  /* now return */
  ASSERT_EQ(prop_size, 1);
  ASSERT_EQ(res_to_str(prop_array[0]), "bool");

  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

TEST(mvvm_prop_gen, not_impl_for_v_on) {
  uint32_t i;
  uint32_t prop_size;
  mvvm_prop_result_t** prop_array;
  const char* xml = "<view v-model=\"value\">"
                      "<button v-on:click=\"{bool == 'true' ? on : off}\"/>"
                    "</view>";
  prop_array = xml_mvvm_prop_xml_to_array(xml, &prop_size);

  /* maybe if true */
  // ASSERT_EQ(prop_size, 3);
  // ASSERT_EQ(res_to_str(prop_array[0]), "value.bool ");
  // ASSERT_EQ(res_to_str(prop_array[1]), "value.on ");
  // ASSERT_EQ(res_to_str(prop_array[2]), "value.off ");

  /* now return */
  ASSERT_EQ(prop_size, 0);

  mvvm_prop_gen_result_destory(prop_array, prop_size);
}

#endif