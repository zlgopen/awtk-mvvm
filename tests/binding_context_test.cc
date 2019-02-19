#include "mvvm/base/model_factory.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "widgets/window.h"
#include "widgets/slider.h"
#include "widgets/button.h"
#include "base/idle.h"
#include "gtest/gtest.h"
#include "test_obj.inc"

static model_t* s_temp_model;
static model_t* s_humidity_model;

#define STR_V_MODEL_TEMP "temp"
#define STR_V_MODEL_HUMIDITY "humidity"

static model_t* test_temp_model_get(navigator_request_t* req) {
  object_ref(OBJECT(s_temp_model));

  return s_temp_model;
}

static model_t* test_humidity_model_get(navigator_request_t* req) {
  object_ref(OBJECT(s_humidity_model));

  return s_humidity_model;
}

static void test_model_init(void) {
  s_temp_model = test_obj_create_model();
  model_factory_register(STR_V_MODEL_TEMP, test_temp_model_get);

  s_humidity_model = test_obj_create_model();
  model_factory_register(STR_V_MODEL_HUMIDITY, test_humidity_model_get);

  return;
}

static ret_t test_model_deinit(void) {
  model_factory_unregister(STR_V_MODEL_TEMP);
  model_factory_unregister(STR_V_MODEL_HUMIDITY);

  object_unref(OBJECT(s_temp_model));
  s_temp_model = NULL;

  object_unref(OBJECT(s_humidity_model));
  s_humidity_model = NULL;

  return RET_OK;
}

static ret_t bind_for_window(widget_t* win) {
  navigator_request_t* req = navigator_request_create("test", NULL);

  binding_context_bind_for_window(win, req);
  object_unref(OBJECT(req));

  return RET_OK;
}

TEST(BindingContextAwtk, data_two_way) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  widget_set_prop_str(slider, "v-data:value", "{i32}");
  bind_for_window(win);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), value_int(&v));

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, multi_model) {
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* temp_slider = slider_create(win, 0, 0, 128, 30);
  widget_t* humidity_slider = slider_create(win, 0, 70, 128, 30);

  test_model_init();

  widget_set_prop_str(temp_slider, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  widget_set_prop_str(temp_slider, "v-data:value", "{i32}");

  widget_set_prop_str(humidity_slider, WIDGET_PROP_V_MODEL, STR_V_MODEL_HUMIDITY);
  widget_set_prop_str(humidity_slider, "v-data:value", "{i32}");

  bind_for_window(win);

  widget_set_value(temp_slider, 99);
  widget_set_value(humidity_slider, 66);

  ASSERT_EQ(object_get_prop_int(OBJECT(s_temp_model), "i32", 0), 99);
  ASSERT_EQ(object_get_prop_int(OBJECT(s_humidity_model), "i32", 0), 66);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, data_once) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=Once}");
  bind_for_window(win);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, data_one_way) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=OneWay}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  value_set_int(&v, 88);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 88);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, data_changed) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changed}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, data_changing) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changing}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 33);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, data_explicit) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, command_update_to_model) {
  value_t v;
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  widget_set_prop_str(slider, "v-on:pointer_down", "{save, Args=2, UpdateModel=True}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  e.e = event_init(EVT_POINTER_DOWN, slider);
  e.x = 30;
  e.y = 30;
  widget_dispatch(slider, &(e.e));

  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  ASSERT_EQ(object_get_prop(OBJECT(s_temp_model), "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 2);

  widget_destroy(win);
  test_model_deinit();
}

TEST(BindingContextAwtk, command_close_window) {
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* button = button_create(win, 0, 0, 128, 30);
  test_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  widget_set_prop_str(button, "v-on:pointer_down", "{save, Args=2, CloseWindow=True}");
  bind_for_window(win);

  e.e = event_init(EVT_POINTER_DOWN, button);
  e.x = 30;
  e.y = 30;
  widget_dispatch(button, &(e.e));

  ASSERT_EQ(win->parent, (widget_t*)NULL);
  idle_dispatch();
  test_model_deinit();
}
