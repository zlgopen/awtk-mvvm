/**
 * File:   xml_mvvm_prop.c
 * Author: AWTK Develop Team
 * Brief:  xml parser for mvvm prop
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2023-05-22 GaoY <gaoyuan@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "common/utils.h"
#include "xml_mvvm_prop.h"
#include "xml/xml_parser.h"

typedef struct _xml_mvvm_prop_builder_t {
  XmlBuilder builder;

  mvvm_prop_gen_t* mvvm_prop_gen;
} xml_mvvm_prop_builder_t;

static const char* xml_mvvm_prop_data_format(const char* prop, uint32_t prop_len) {
  char* ptr = (char*)prop;
  if (ptr[prop_len - 1] == '}') ptr[prop_len - 1] = '\0';
  if (ptr[0] == '{') ptr += 1;
  return ptr;
}

static void xml_mvvm_prop_builder_on_start(XmlBuilder* builder, const char* tag,
                                           const char** attrs) {
  uint32_t i = 0;
  mvvm_prop_gen_exec gen_exec = NULL;
  xml_mvvm_prop_builder_t* b = (xml_mvvm_prop_builder_t*)builder;
  return_if_fail(b != NULL && builder != NULL && tag != NULL && attrs != NULL);

  gen_exec = mvvm_prop_gen_translate_keyword("scope_start");
  if (gen_exec != NULL) {
    gen_exec(b->mvvm_prop_gen, tag);
  }

  while (attrs[i] != NULL && attrs[i + 1] != NULL) {
    gen_exec = mvvm_prop_gen_translate_keyword(attrs[i]);
    if (gen_exec != NULL) {
      const char* prop = NULL;
      prop = xml_mvvm_prop_data_format((const char*)attrs[i + 1], strlen(attrs[i + 1]));
      gen_exec(b->mvvm_prop_gen, tag, prop);
    }
    i += 2;
  }

  return;
}

static void xml_mvvm_prop_builder_on_end(XmlBuilder* builder, const char* tag) {
  mvvm_prop_gen_exec gen_exec = NULL;
  xml_mvvm_prop_builder_t* b = (xml_mvvm_prop_builder_t*)builder;
  return_if_fail(b != NULL && builder != NULL);

  gen_exec = mvvm_prop_gen_translate_keyword("scope_end");
  if (gen_exec != NULL) {
    gen_exec(b->mvvm_prop_gen, tag);
  }

  return;
}

static void xml_mvvm_prop_builder_on_error(XmlBuilder* builder, int line, int col,
                                           const char* message) {
  xml_mvvm_prop_builder_t* b = (xml_mvvm_prop_builder_t*)builder;
  return_if_fail(b != NULL && builder != NULL && message != NULL);

  log_debug("%d:%d %s.", line, col, message);

  return;
}

static xml_mvvm_prop_builder_t* builder_init(xml_mvvm_prop_builder_t* b) {
  return_value_if_fail(b != NULL, NULL);

  b->mvvm_prop_gen = mvvm_prop_gen_create();

  b->builder.on_pi = NULL;
  b->builder.on_text = NULL;
  b->builder.on_comment = NULL;
  b->builder.on_start = xml_mvvm_prop_builder_on_start;
  b->builder.on_end = xml_mvvm_prop_builder_on_end;
  b->builder.on_error = xml_mvvm_prop_builder_on_error;

  return b;
}

static void builder_deinit(xml_mvvm_prop_builder_t* b) {
  return_if_fail(b != NULL);

  if (b->mvvm_prop_gen != NULL) {
    mvvm_prop_gen_destory(b->mvvm_prop_gen);
    b->mvvm_prop_gen = NULL;
  }

  return;
}

mvvm_prop_result_t** xml_mvvm_prop_xml_to_array(const char* xml, uint32_t* result_size) {
  XmlParser* parser = NULL;
  xml_mvvm_prop_builder_t builder;
  mvvm_prop_result_t** results = NULL;
  return_value_if_fail(xml != NULL && result_size != NULL, NULL);

  builder_init(&builder);
  parser = xml_parser_create();
  xml_parser_set_trim_text(parser, TRUE);
  xml_parser_set_builder(parser, &builder.builder);
  xml_parser_parse(parser, xml, strlen(xml));
  results = mvvm_prop_gen_result_create(builder.mvvm_prop_gen, result_size);
  xml_parser_destroy(parser);
  builder_deinit(&builder);

  return results;
}

mvvm_prop_result_t** xml_mvvm_prop_file_to_array(const char* in_filename, uint32_t* result_size) {
  uint32_t length = 0;
  const char* xml = NULL;
  mvvm_prop_result_t** results = NULL;
  goto_error_if_fail(in_filename != NULL);

  xml = file_read(in_filename, &length);
  goto_error_if_fail(xml != NULL);
  results = xml_mvvm_prop_xml_to_array(xml, result_size);
  goto_error_if_fail(results != NULL);
  TKMEM_FREE(xml);
  return results;
error:
  if (xml != NULL) {
    TKMEM_FREE(xml);
  }
  return NULL;
}

ret_t xml_mvvm_prop_file_to_packfile(const char* in_filename, const char* out_filename) {
  str_t json;
  ret_t ret = RET_OK;
  fs_file_t* fp = NULL;
  uint32_t result_size = 0;
  mvvm_prop_result_t** result = NULL;
  return_value_if_fail(in_filename != NULL && out_filename != NULL, RET_BAD_PARAMS);

  fp = fs_open_file(os_fs(), out_filename, "wb+");
  return_value_if_fail(fp != NULL, RET_FAIL);

  str_init(&json, 1024);
  result = xml_mvvm_prop_file_to_array(in_filename, &result_size);
  if (result != NULL) {
    mvvm_prop_gen_save_result(result, result_size, &json);
    if (fs_file_write(fp, json.str, json.size) != json.size) {
      ret = RET_FAIL;
      log_debug("write reuslt into file failed!\n");
    }
  }
  str_reset(&json);
  fs_file_close(fp);
  mvvm_prop_gen_result_destory(result, result_size);

  return ret;
}

mvvm_prop_result_t** xml_mvvm_prop_packfile_to_array(const char* in_filename, uint32_t* result_size) {
  str_t json;
  uint32_t length = 0;
  const char* str = NULL;
  mvvm_prop_result_t** results = NULL;
  return_value_if_fail(in_filename != NULL && result_size != NULL, NULL);

  str = file_read(in_filename, &length);
  return_value_if_fail(str != NULL, NULL);

  str_init(&json, length);
  str_set_with_len(&json, str, length);
  results = mvvm_prop_gen_proto_result(&json, result_size);

  TKMEM_FREE(str);
  str_reset(&json);
  return results;
}
