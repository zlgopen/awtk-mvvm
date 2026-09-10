#include "mvvm/awtk/ui_loader_mvvm.h"
#include "base/window.h"
#include "base/dialog.h"
#include "base/self_layouter.h"
#include "tkc/path.h"
#include "ui_loader/ui_builder_default.h"
#include "gtest/gtest.h"

TEST(UILoaderMvvm, basic) {
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  ui_loader_t* loader = ui_loader_mvvm();
  ui_builder_t* builder = ui_builder_default_create("");
  const char* xml =
      "<dialog x=\"0\" y=\"0\" w=\"400\" h=\"300\">\
      <dialog_title style=\"default\" x=\"0\" y=\"0\" w=\"100%\" h=\"30\" text=\"Hello AWTK\" /> \
      <dialog_client style=\"border\" x=\"0\" y=\"bottom\" w=\"100%\" h=\"-30\">\
      <button x=\"0\" y=\"0\" w=\"80\" h=\"30\" name=\"ok\" text=\"ok\" />\
      <label x=\"128\" y=\"0\" w=\"80\" h=\"30\" name=\"cancel\" text=\"cancel\" />\
      </dialog_client>\
      </dialog>";

  ASSERT_EQ(ui_loader_load(loader, (const uint8_t*)xml, strlen(xml), builder), RET_OK);

  ASSERT_EQ(tk_str_eq(widget_get_type(builder->root), WIDGET_TYPE_DIALOG), true);
  ASSERT_EQ(widget_count_children(builder->root), 2);

  ok = widget_lookup(builder->root, "ok", TRUE);
  cancel = widget_lookup(builder->root, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(ok), WIDGET_TYPE_BUTTON), true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(cancel), WIDGET_TYPE_LABEL), true);

  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 0)->name, "ok"), 0);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(builder->root)->client, 1)->name, "cancel"), 0);

  widget_destroy(builder->root);
  ui_builder_destroy(builder);
}

TEST(UILoaderMvvm, component) {
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_t* dialog = dialog_create(NULL, 0, 0, 100, 100);
  ui_loader_t* loader = ui_loader_mvvm();
  ui_builder_t* builder = ui_builder_default_create("");
  const char* xml =
      "<dialog_title style=\"default\" x=\"0\" y=\"0\" w=\"100%\" h=\"30\" text=\"Hello AWTK\" /> \
      <dialog_client style=\"border\" x=\"0\" y=\"bottom\" w=\"100%\" h=\"-30\">\
      <button x=\"0\" y=\"0\" w=\"80\" h=\"30\" name=\"ok\" text=\"ok\" />\
      <label x=\"128\" y=\"0\" w=\"80\" h=\"30\" name=\"cancel\" text=\"cancel\" />\
      </dialog_client>";

  builder->widget = dialog;
  ASSERT_EQ(ui_loader_load(loader, (const uint8_t*)xml, strlen(xml), builder), RET_OK);
  ASSERT_EQ(widget_count_children(dialog), 2);

  ok = widget_lookup(dialog, "ok", TRUE);
  cancel = widget_lookup(dialog, "cancel", TRUE);

  ASSERT_EQ(ok != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(ok), WIDGET_TYPE_BUTTON), true);
  ASSERT_EQ(cancel != NULL, true);
  ASSERT_EQ(tk_str_eq(widget_get_type(cancel), WIDGET_TYPE_LABEL), true);

  ASSERT_EQ(strcmp(widget_get_child(DIALOG(dialog)->client, 0)->name, "ok"), 0);
  ASSERT_EQ(strcmp(widget_get_child(DIALOG(dialog)->client, 1)->name, "cancel"), 0);

  widget_destroy(dialog);
  ui_builder_destroy(builder);
}

TEST(UILoaderMvvm, component_with_condition) {
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_t* dialog = NULL;
  navigator_request_t* req = navigator_request_create(NULL, NULL);
  ui_loader_t* loader = ui_loader_mvvm();
  ui_builder_t* builder = ui_builder_default_create("");
  ui_loader_mvvm_builder_ctx_t ctx = {0};
  const char* xml0 = "<dialog x=\"0\" y=\"0\" w=\"400\" h=\"300\" v-model=\"\"/>";
  const char* xml1 =
      "<dialog_title v-if=\"{true}\" style=\"default\" x=\"0\" y=\"0\" w=\"100%\" h=\"30\" text=\"Hello AWTK\" /> \
      <dialog_client v-else=\"\" style=\"border\" x=\"0\" y=\"bottom\" w=\"100%\" h=\"-30\">\
      <button x=\"0\" y=\"0\" w=\"80\" h=\"30\" name=\"ok\" text=\"ok\" />\
      <label x=\"128\" y=\"0\" w=\"80\" h=\"30\" name=\"cancel\" text=\"cancel\" />\
      </dialog_client>";

  ctx.navigator_request = req;
  ui_builder_set_additional_context(builder, &ctx, NULL);
  ASSERT_EQ(ui_loader_load(loader, (const uint8_t*)xml0, strlen(xml0), builder), RET_OK);
  dialog = builder->root;
  ASSERT_EQ(tk_str_eq(widget_get_type(dialog), WIDGET_TYPE_DIALOG), true);
  ASSERT_EQ(widget_count_children(dialog), 0);

  builder->widget = dialog;
  ui_builder_set_additional_context(builder, NULL, NULL);
  ASSERT_EQ(ui_loader_load(loader, (const uint8_t*)xml1, strlen(xml1), builder), RET_OK);

  ASSERT_EQ(widget_count_children(dialog), 1);

  ok = widget_lookup(dialog, "ok", TRUE);
  cancel = widget_lookup(dialog, "cancel", TRUE);

  ASSERT_EQ(ok == NULL, true);
  ASSERT_EQ(cancel == NULL, true);

  ASSERT_EQ(wcs_cmp(widget_get_child(dialog, 0)->text.str, L"Hello AWTK"), 0);

  widget_destroy(dialog);
  ui_builder_destroy(builder);
  TK_OBJECT_UNREF(req);
}
