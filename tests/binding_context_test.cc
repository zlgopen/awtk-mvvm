#ifndef AWTK_NOGUI
#include "awtk.h"
#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_binary_writer.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_dummy.h"
#include "mvvm/base/view_model_array_dummy.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "mvvm/base/binding_rule_parser.h"
#include "gtest/gtest.h"
#include "test_obj.h"

static view_model_t* s_temp_view_model;
static view_model_t* s_humidity_view_model;
static view_model_t* s_persons_view_model;

#define STR_V_MODEL_TEMP "temp"
#define STR_V_MODEL_PERSONS "persons"
#define STR_V_MODEL_HUMIDITY "humidity"

static view_model_t* test_temp_view_model_get(navigator_request_t* req) {
  tk_object_ref(TK_OBJECT(s_temp_view_model));

  return s_temp_view_model;
}

static view_model_t* test_humidity_view_model_get(navigator_request_t* req) {
  tk_object_ref(TK_OBJECT(s_humidity_view_model));

  return s_humidity_view_model;
}

static view_model_t* test_persons_view_model_get(navigator_request_t* req) {
  tk_object_ref(TK_OBJECT(s_persons_view_model));

  return s_persons_view_model;
}

static ret_t persons_gen(view_model_t* view_model, uint32_t n) {
  uint32_t i = 0;
  navigator_request_t* req = navigator_request_create("test", NULL);
  view_model_array_dummy_clear(view_model);

  for (i = 0; i < n; i++) {
    view_model_t* subview_model = view_model_dummy_create(req);
    tk_object_set_prop_int(TK_OBJECT(subview_model), "a", i);
    tk_object_set_prop_int(TK_OBJECT(subview_model), "b", i + 1);
    tk_object_set_prop_int(TK_OBJECT(subview_model), "c", i + 2);
    view_model_array_dummy_add(view_model, subview_model);
    tk_object_unref(TK_OBJECT(subview_model));
  }
  TK_OBJECT_UNREF(req);

  return RET_OK;
}

static view_model_t* persons_create_view_model(void) {
  view_model_t* view_model = view_model_array_dummy_create(NULL);

  persons_gen(view_model, 10);

  return view_model;
}

static void test_view_model_init(void) {
  s_temp_view_model = test_obj_view_model_create(NULL);
  view_model_factory_register(STR_V_MODEL_TEMP, test_temp_view_model_get);

  s_humidity_view_model = test_obj_view_model_create(NULL);
  view_model_factory_register(STR_V_MODEL_HUMIDITY, test_humidity_view_model_get);

  s_persons_view_model = persons_create_view_model();
  view_model_factory_register(STR_V_MODEL_PERSONS, test_persons_view_model_get);

  return;
}

static ret_t test_view_model_deinit(void) {
  view_model_factory_unregister(STR_V_MODEL_TEMP);
  view_model_factory_unregister(STR_V_MODEL_HUMIDITY);
  view_model_factory_unregister(STR_V_MODEL_PERSONS);

  tk_object_unref(TK_OBJECT(s_temp_view_model));
  s_temp_view_model = NULL;

  tk_object_unref(TK_OBJECT(s_humidity_view_model));
  s_humidity_view_model = NULL;

  tk_object_unref(TK_OBJECT(s_persons_view_model));
  s_persons_view_model = NULL;
  idle_manager_remove_all(idle_manager());

  return RET_OK;
}

static binding_context_t* binding_context_create(binding_context_t* parent, const char* vmodel,
                                                 widget_t* widget) {
  navigator_request_t* req = navigator_request_create("test", NULL);
  binding_context_t* ctx = binding_context_awtk_create(parent, vmodel, req, widget);

  tk_object_unref(TK_OBJECT(req));

  return ctx;
}

static binding_rule_t* binding_rule_create(widget_t* widget, const char* name, const char* val) {
  binding_rule_t* rule = binding_rule_parse(name, val, widget->vt->inputable);

  rule->widget = widget;
  return rule;
}

TEST(BindingContextAwtk, data_two_way) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);

  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), value_int(&v));

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, multi_view_model) {
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* temp_slider = slider_create(win, 0, 0, 128, 30);
  widget_t* humidity_slider = slider_create(win, 0, 70, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, temp_slider);
  rule = binding_rule_create(temp_slider, "v-data:value", "{i32}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  ctx = binding_context_create(NULL, STR_V_MODEL_HUMIDITY, humidity_slider);
  rule = binding_rule_create(humidity_slider, "v-data:value", "{i32}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  widget_set_value(temp_slider, 99);
  widget_set_value(humidity_slider, 66);

  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(s_temp_view_model), "i32", 0), 99);
  ASSERT_EQ(tk_object_get_prop_int(TK_OBJECT(s_humidity_view_model), "i32", 0), 66);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_once) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32, Mode=Once}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  ASSERT_EQ(widget_get_value(slider), 66);
  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_one_way) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32, Mode=OneWay}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  value_set_int(&v, 88);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 88);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_changed) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changed}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_changing) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changing}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 33);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_explicit) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, command_update_to_view_model) {
  value_t v;
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  binding_context_t* ctx;
  binding_rule_t* rule;

  test_view_model_init();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_data(ctx, rule);
  rule = binding_rule_create(slider, "v-on:pointer_down", "{save, Args=2, UpdateModel=True}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_command(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  value_set_int(&v, 66);
  tk_object_set_prop(TK_OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  e.e = event_init(EVT_POINTER_DOWN, slider);
  e.x = 30;
  e.y = 30;
  widget_dispatch(slider, &(e.e));

  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  ASSERT_EQ(tk_object_get_prop(TK_OBJECT(s_temp_view_model), "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, command_close_window) {
  binding_context_t* ctx;
  binding_rule_t* rule;
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* button = button_create(win, 0, 0, 128, 30);

  test_view_model_init();
  window_manager_open_window(window_manager(), win);
  idle_dispatch();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(button, "v-on:pointer_down", "{save, Args=2, CloseWindow=True}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_command(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  e.e = event_init(EVT_POINTER_DOWN, button);
  e.x = 30;
  e.y = 30;
  widget_dispatch(button, &(e.e));

  ASSERT_EQ(win->parent, (widget_t*)NULL);
  idle_dispatch();
  test_view_model_deinit();
}

static ret_t widget_dispatch_pointer_down(widget_t* widget, int x, int y) {
  pointer_event_t e;
  e.e = event_init(EVT_POINTER_DOWN, widget);
  e.x = x;
  e.y = y;
  return widget_dispatch(widget, &(e.e));
}

TEST(BindingContextAwtk, fscript1) {
  binding_context_t* ctx;
  binding_rule_t* rule;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* button = button_create(win, 0, 0, 128, 30);

  test_view_model_init();
  window_manager_open_window(window_manager(), win);
  idle_dispatch();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(button, "v-on:pointer_down",
                             "{fscript, Args=widget_set(x, widget_get(x)+10)}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_command(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  widget_dispatch_pointer_down(button, 30, 30);
  ASSERT_EQ(button->x, 10);

  idle_dispatch();
  test_view_model_deinit();
}

TEST(BindingContextAwtk, fscript2) {
  binding_context_t* ctx;
  binding_rule_t* rule;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* button = button_create(win, 0, 0, 128, 30);

  test_view_model_init();
  window_manager_open_window(window_manager(), win);
  idle_dispatch();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(button, "v-on:pointer_down",
                             "{fscript, Args=widget_set(parent.x, widget_get(parent.x)+10)}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_command(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  widget_dispatch_pointer_down(button, 30, 30);
  ASSERT_EQ(win->x, 10);

  idle_dispatch();
  test_view_model_deinit();
}

TEST(BindingContextAwtk, fscript3) {
  binding_context_t* ctx;
  binding_rule_t* rule;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* button = button_create(win, 0, 0, 128, 30);

  test_view_model_init();
  window_manager_open_window(window_manager(), win);
  idle_dispatch();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(button, "v-on:pointer_down",
                             "{fscript, Args=widget_set(window.x, widget_get(window.x)+10)}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_command(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  widget_dispatch_pointer_down(button, 30, 30);
  ASSERT_EQ(win->x, 10);

  idle_dispatch();
  test_view_model_deinit();
}

TEST(BindingContextAwtk, fscript4) {
  binding_context_t* ctx;
  binding_rule_t* rule;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* button = button_create(win, 0, 0, 128, 30);

  test_view_model_init();
  widget_set_name(button, "button");
  window_manager_open_window(window_manager(), win);
  idle_dispatch();

  ctx = binding_context_create(NULL, STR_V_MODEL_TEMP, win);
  rule = binding_rule_create(
      button, "v-on:pointer_down",
      "{fscript, Args=widget_set(window.button.x, widget_get(window.button.x)+10)}");
  tk_object_set_prop_bool(TK_OBJECT(rule), BINDING_RULE_PROP_INITED, true);
  binding_context_bind_command(ctx, rule);
  binding_context_set_bound(ctx, TRUE);

  widget_dispatch_pointer_down(button, 30, 30);
  ASSERT_EQ(button->x, 10);

  idle_dispatch();
  test_view_model_deinit();
}

#endif /*AWTK_NOGUI*/
