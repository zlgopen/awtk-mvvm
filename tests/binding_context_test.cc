#ifndef AWTK_NOGUI
#include "awtk.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_dummy.h"
#include "mvvm/base/view_model_array_dummy.h"
#include "mvvm/awtk/binding_context_awtk.h"
#include "gtest/gtest.h"
#include "test_obj.h"

static view_model_t* s_temp_view_model;
static view_model_t* s_humidity_view_model;
static view_model_t* s_persons_view_model;

#define STR_V_MODEL_TEMP "temp"
#define STR_V_MODEL_PERSONS "persons"
#define STR_V_MODEL_HUMIDITY "humidity"

static view_model_t* test_temp_view_model_get(navigator_request_t* req) {
  object_ref(OBJECT(s_temp_view_model));

  return s_temp_view_model;
}

static view_model_t* test_humidity_view_model_get(navigator_request_t* req) {
  object_ref(OBJECT(s_humidity_view_model));

  return s_humidity_view_model;
}

static view_model_t* test_persons_view_model_get(navigator_request_t* req) {
  object_ref(OBJECT(s_persons_view_model));

  return s_persons_view_model;
}

static ret_t persons_gen(view_model_t* view_model, uint32_t n) {
  uint32_t i = 0;
  view_model_array_dummy_clear(view_model);

  for (i = 0; i < n; i++) {
    view_model_t* subview_model = view_model_dummy_create(NULL);
    object_set_prop_int(OBJECT(subview_model), "a", i);
    object_set_prop_int(OBJECT(subview_model), "b", i + 1);
    object_set_prop_int(OBJECT(subview_model), "c", i + 2);
    view_model_array_dummy_add(view_model, subview_model);
    object_unref(OBJECT(subview_model));
  }

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

  object_unref(OBJECT(s_temp_view_model));
  s_temp_view_model = NULL;

  object_unref(OBJECT(s_humidity_view_model));
  s_humidity_view_model = NULL;

  object_unref(OBJECT(s_persons_view_model));
  s_persons_view_model = NULL;
  idle_manager_remove_all(idle_manager());

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
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  widget_set_prop_str(slider, "v-data:value", "{i32}");
  bind_for_window(win);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), value_int(&v));

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, multi_view_model) {
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* temp_slider = slider_create(win, 0, 0, 128, 30);
  widget_t* humidity_slider = slider_create(win, 0, 70, 128, 30);

  test_view_model_init();

  widget_set_prop_str(temp_slider, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  widget_set_prop_str(temp_slider, "v-data:value", "{i32}");

  widget_set_prop_str(humidity_slider, WIDGET_PROP_V_MODEL, STR_V_MODEL_HUMIDITY);
  widget_set_prop_str(humidity_slider, "v-data:value", "{i32}");

  bind_for_window(win);

  widget_set_value(temp_slider, 99);
  widget_set_value(humidity_slider, 66);

  ASSERT_EQ(object_get_prop_int(OBJECT(s_temp_view_model), "i32", 0), 99);
  ASSERT_EQ(object_get_prop_int(OBJECT(s_humidity_view_model), "i32", 0), 66);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_once) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=Once}");
  bind_for_window(win);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_one_way) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=OneWay}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  value_set_int(&v, 88);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);
  idle_dispatch();
  ASSERT_EQ(widget_get_value(slider), 88);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_changed) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changed}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_changing) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Changing}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 33);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, data_explicit) {
  value_t v;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  slider_set_value_internal(slider, 33, EVT_VALUE_CHANGING, TRUE);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, command_update_to_view_model) {
  value_t v;
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* slider = slider_create(win, 0, 0, 128, 30);
  test_view_model_init();

  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  value_set_int(&v, 66);
  object_set_prop(OBJECT(s_temp_view_model), "i32", &v);

  widget_set_prop_str(slider, "v-data:value", "{i32, Mode=TwoWay, Trigger=Explicit}");
  widget_set_prop_str(slider, "v-on:pointer_down", "{save, Args=2, UpdateModel=True}");
  bind_for_window(win);
  ASSERT_EQ(widget_get_value(slider), 66);

  widget_set_value(slider, 99);
  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 66);

  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 0);

  e.e = event_init(EVT_POINTER_DOWN, slider);
  e.x = 30;
  e.y = 30;
  widget_dispatch(slider, &(e.e));

  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "i32", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 99);

  ASSERT_EQ(object_get_prop(OBJECT(s_temp_view_model), "save_count", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 1);

  widget_destroy(win);
  test_view_model_deinit();
}

TEST(BindingContextAwtk, command_close_window) {
  pointer_event_t e;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  window_manager_open_window(window_manager(), win);

  widget_t* button = button_create(win, 0, 0, 128, 30);
  test_view_model_init();

  idle_dispatch();
  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_TEMP);
  widget_set_prop_str(button, "v-on:pointer_down", "{save, Args=2, CloseWindow=True}");
  bind_for_window(win);

  e.e = event_init(EVT_POINTER_DOWN, button);
  e.x = 30;
  e.y = 30;
  widget_dispatch(button, &(e.e));

  ASSERT_EQ(win->parent, (widget_t*)NULL);
  idle_dispatch();
  test_view_model_deinit();
}

TEST(BindingContextAwtk, array) {
  uint32_t i = 0;
  widget_t* win = window_create(NULL, 0, 0, 400, 300);
  widget_t* list_view = list_view_create(win, 0, 0, 128, 300);
  widget_t* list_item = list_item_create(list_view, 0, 0, 128, 30);

  widget_t* a = slider_create(list_item, 0, 0, 0, 0);
  widget_t* b = slider_create(list_item, 0, 0, 0, 0);
  widget_t* c = slider_create(list_item, 0, 0, 0, 0);
  widget_t* d = slider_create(list_item, 0, 0, 0, 0);

  widget_set_name(a, "a");
  widget_set_name(b, "b");
  widget_set_name(c, "c");
  widget_set_name(d, "d");
  slider_set_max(a, 50000);
  slider_set_max(b, 50000);
  slider_set_max(c, 50000);
  slider_set_max(d, 50000);

  widget_set_prop_str(a, "v-data:value", "{item.a}");
  widget_set_prop_str(b, "v-data:value", "{item.b}");
  widget_set_prop_str(c, "v-data:value", "{item.c}");
  widget_set_prop_str(d, "v-data:value", "{item.a + item.b - item.c + 1}");

  test_view_model_init();

  widget_set_prop_bool(list_view, WIDGET_PROP_V_FOR_ITEMS, TRUE);
  widget_set_prop_str(win, WIDGET_PROP_V_MODEL, STR_V_MODEL_PERSONS);

  bind_for_window(win);

  ASSERT_EQ(view_model_array_dummy_size(s_persons_view_model), 10);
  ASSERT_EQ(list_view->children->size, view_model_array_dummy_size(s_persons_view_model));

  for (i = 0; i < view_model_array_dummy_size(s_persons_view_model); i++) {
    list_item = widget_get_child(list_view, i);
    log_debug("i=%d\n", i);
    a = widget_child(list_item, "a");
    b = widget_child(list_item, "b");
    c = widget_child(list_item, "c");
    d = widget_child(list_item, "d");

    ASSERT_EQ(widget_get_value(a), i);
    ASSERT_EQ(widget_get_value(b), i + 1);
    ASSERT_EQ(widget_get_value(c), i + 2);
    ASSERT_EQ(widget_get_value(d), i);
  }

  view_model_array_dummy_clear(s_persons_view_model);
  ASSERT_EQ(view_model_array_dummy_size(s_persons_view_model), 0);
  view_model_array_notify_items_changed(VIEW_MODEL(s_persons_view_model));
  idle_dispatch();
  ASSERT_EQ(list_view->children->size, view_model_array_dummy_size(s_persons_view_model));

  persons_gen(s_persons_view_model, 10000);
  view_model_array_notify_items_changed(VIEW_MODEL(s_persons_view_model));
  idle_dispatch();
  for (i = 0; i < view_model_array_dummy_size(s_persons_view_model); i++) {
    list_item = widget_get_child(list_view, i);
    log_debug("i=%d\n", i);
    a = widget_child(list_item, "a");
    b = widget_child(list_item, "b");
    c = widget_child(list_item, "c");
    d = widget_child(list_item, "d");

    ASSERT_EQ(widget_get_value(a), i);
    ASSERT_EQ(widget_get_value(b), i + 1);
    ASSERT_EQ(widget_get_value(c), i + 2);
    ASSERT_EQ(widget_get_value(d), i);
  }

  widget_destroy(win);
  test_view_model_deinit();

  idle_dispatch();
}
#endif /*AWTK_NOGUI*/


