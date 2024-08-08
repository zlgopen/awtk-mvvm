#include "gtest/gtest.h"
#include "tkc/object_default.h"
#include "tkc/tokenizer.h"
#include "tkc/log.h"
#include "mvvm/base/view_model_factory.h"
#include "mvvm/base/view_model_compositor.h"
#include "humidity_view_model.h"
#include "temperature_view_model.h"

static view_model_t* test_view_model_compositor_create(const char* vmodel,
                                                       navigator_request_t* req) {
  tokenizer_t t;
  view_model_t* compositor = view_model_compositor_create(req);
  return_value_if_fail(compositor != NULL, NULL);

  tokenizer_init(&t, vmodel, strlen(vmodel), "+");
  while (tokenizer_has_more(&t)) {
    const char* type1 = tokenizer_next(&t);
    view_model_t* vm = view_model_factory_create_model(type1, req);
    if (vm != NULL) {
      if (TK_OBJECT(vm)->name == NULL) {
        tk_object_set_name(TK_OBJECT(vm), type1);
      }
      if (view_model_compositor_add(compositor, vm) != RET_OK) {
        log_warn("view_model_compositor_add failed\n");
        TK_OBJECT_UNREF(vm);
      }
    } else {
      log_warn("create %s view_model failed\n", type1);
    }
  }
  tokenizer_deinit(&t);
  return compositor;
}

TEST(ViewModelCompositor, basic) {
  value_t v;
  navigator_request_t req;
  memset(&req, 0x00, sizeof(req));
  view_model_factory_register("temp", temperature_view_model_create);
  view_model_factory_register("humi", humidity_view_model_create);
  view_model_t* vm = test_view_model_compositor_create("temp+humi", &req);
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(vm);
  ASSERT_EQ(compositor->view_models.size, 2);

  ASSERT_EQ(view_model_get_prop(vm, "temp", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 123);
  ASSERT_EQ(view_model_set_prop(vm, "temp", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "humi", &v), RET_OK);
  ASSERT_EQ(view_model_set_prop(vm, "humi", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "not_exist", &v), RET_NOT_FOUND);
  ASSERT_EQ(view_model_set_prop(vm, "not_exist", &v), RET_NOT_FOUND);

  ASSERT_EQ(view_model_can_exec(vm, "apply_humi", NULL), TRUE);
  ASSERT_EQ(view_model_can_exec(vm, "apply_temp", NULL), TRUE);

  ASSERT_EQ(view_model_exec(vm, "apply_humi", NULL), RET_OK);
  ASSERT_EQ(view_model_exec(vm, "apply_temp", NULL), RET_OK);

  ASSERT_EQ(view_model_can_exec(vm, "not_exist", NULL), FALSE);
  ASSERT_NE(view_model_exec(vm, "not_exist", NULL), RET_OK);

  TK_OBJECT_UNREF(vm);
}

TEST(ViewModelCompositor, prefix) {
  value_t v;
  navigator_request_t req;
  memset(&req, 0x00, sizeof(req));
  view_model_factory_register("a", temperature_view_model_create);
  view_model_factory_register("b", humidity_view_model_create);
  view_model_t* vm = test_view_model_compositor_create("a+b", &req);
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(vm);
  ASSERT_EQ(compositor->view_models.size, 2);

  ASSERT_EQ(view_model_get_prop(vm, "a.temp", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 123);
  ASSERT_EQ(view_model_set_prop(vm, "a.temp", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "b.humi", &v), RET_OK);
  ASSERT_EQ(view_model_set_prop(vm, "b.humi", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "not_exist", &v), RET_NOT_FOUND);
  ASSERT_EQ(view_model_set_prop(vm, "not_exist", &v), RET_NOT_FOUND);

  ASSERT_EQ(view_model_can_exec(vm, "b.apply_humi", NULL), TRUE);
  ASSERT_EQ(view_model_can_exec(vm, "a.apply_temp", NULL), TRUE);

  ASSERT_EQ(view_model_exec(vm, "b.apply_humi", NULL), RET_OK);
  ASSERT_EQ(view_model_exec(vm, "a.apply_temp", NULL), RET_OK);

  ASSERT_EQ(view_model_can_exec(vm, "not_exist", NULL), FALSE);
  ASSERT_NE(view_model_exec(vm, "not_exist", NULL), RET_OK);

  TK_OBJECT_UNREF(vm);
}

view_model_t* my_humidity_view_model_create(navigator_request_t* req) {
  view_model_t* vm = humidity_view_model_create(req);
  tk_object_set_name(TK_OBJECT(vm), "vm1");

  return vm;
}

view_model_t* my_temperature_view_model_create(navigator_request_t* req) {
  view_model_t* vm = temperature_view_model_create(req);
  tk_object_set_name(TK_OBJECT(vm), "vm2");
  
  return vm;
}

TEST(ViewModelCompositor, named) {
  value_t v;
  navigator_request_t req;
  memset(&req, 0x00, sizeof(req));
  view_model_factory_register("temp", my_temperature_view_model_create);
  view_model_factory_register("humi", my_humidity_view_model_create);

  view_model_t* vm = test_view_model_compositor_create("temp+humi", &req);
  view_model_compositor_t* compositor = VIEW_MODEL_COMPOSITOR(vm);
  ASSERT_EQ(compositor->view_models.size, 2);

  ASSERT_EQ(view_model_get_prop(vm, "vm2.temp", &v), RET_OK);
  ASSERT_EQ(value_int(&v), 123);
  ASSERT_EQ(view_model_set_prop(vm, "vm2.temp", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "vm1.humi", &v), RET_OK);
  ASSERT_EQ(view_model_set_prop(vm, "vm1.humi", &v), RET_OK);

  ASSERT_EQ(view_model_get_prop(vm, "not_exist", &v), RET_NOT_FOUND);
  ASSERT_EQ(view_model_set_prop(vm, "not_exist", &v), RET_NOT_FOUND);

  ASSERT_EQ(view_model_can_exec(vm, "vm1.apply_humi", NULL), TRUE);
  ASSERT_EQ(view_model_can_exec(vm, "vm2.apply_temp", NULL), TRUE);

  ASSERT_EQ(view_model_exec(vm, "vm1.apply_humi", NULL), RET_OK);
  ASSERT_EQ(view_model_exec(vm, "vm2.apply_temp", NULL), RET_OK);

  ASSERT_EQ(view_model_can_exec(vm, "not_exist", NULL), FALSE);
  ASSERT_NE(view_model_exec(vm, "not_exist", NULL), RET_OK);

  TK_OBJECT_UNREF(vm);
}
