# 在 AWTK-WEB 中使用 MVVM

AWTK-WEB APP 的初始化流程和普通 AWTK APP 不同，对于基于 MVVM 的 APP，如果要在 WEB 上运行，需要自己初始化 MVVM 模块。

## 声明 MVVM 初始化和反初始化函数

```
static ret_t mvvm_app_init(void);
static ret_t mvvm_app_deinit(void);
```

## 调用 MVVM 初始化和反初始化函数

```c
ret_t application_init(void) {
#ifdef AWTK_WEB
  mvvm_app_init();
#endif/*AWTK_WEB*/

  view_model_factory_register("temperature", temperature_view_model_create);

  return navigator_to("temperature1");
}

ret_t application_exit(void) {
  log_debug("application_exit\n");
#ifdef AWTK_WEB
  mvvm_app_deinit();
#endif/*AWTK_WEB*/

  return RET_OK;
}
```

完整示例请参考：demos/demo1/application.c
