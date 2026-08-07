## custom\_binder\_t
### 概述
自定义绑定器。
----------------------------------
### 函数
<p id="custom_binder_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#custom_binder_t_custom_binder_bind">custom\_binder\_bind</a> | 自定义绑定。 |
| <a href="#custom_binder_t_custom_binder_deinit">custom\_binder\_deinit</a> | ~初始化自定义绑定器。 |
| <a href="#custom_binder_t_custom_binder_exist">custom\_binder\_exist</a> | 检查自定义绑定器是否存在。 |
| <a href="#custom_binder_t_custom_binder_init">custom\_binder\_init</a> | 初始化自定义绑定器。 |
| <a href="#custom_binder_t_custom_binder_register">custom\_binder\_register</a> | 注册模型的自定义绑定器。 |
| <a href="#custom_binder_t_custom_binder_unregister">custom\_binder\_unregister</a> | 注销自定义绑定器。 |
#### custom\_binder\_bind 函数
-----------------------

* 函数功能：

> <p id="custom_binder_t_custom_binder_bind">自定义绑定。

* 函数原型：

```
ret_t custom_binder_bind (const char* type, void* widget, binding_context_t* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 模型的类型。 |
| widget | void* | 控件。 |
| ctx | binding\_context\_t* | ctx对象。 |
#### custom\_binder\_deinit 函数
-----------------------

* 函数功能：

> <p id="custom_binder_t_custom_binder_deinit">~初始化自定义绑定器。

* 函数原型：

```
ret_t custom_binder_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### custom\_binder\_exist 函数
-----------------------

* 函数功能：

> <p id="custom_binder_t_custom_binder_exist">检查自定义绑定器是否存在。

* 函数原型：

```
bool_t custom_binder_exist (const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| type | const char* | 控件的类型。 |
#### custom\_binder\_init 函数
-----------------------

* 函数功能：

> <p id="custom_binder_t_custom_binder_init">初始化自定义绑定器。

* 函数原型：

```
ret_t custom_binder_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### custom\_binder\_register 函数
-----------------------

* 函数功能：

> <p id="custom_binder_t_custom_binder_register">注册模型的自定义绑定器。

* 函数原型：

```
ret_t custom_binder_register (const char* type, custom_bind_t bind);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 控件的类型。 |
| bind | custom\_bind\_t | 创建函数。 |
#### custom\_binder\_unregister 函数
-----------------------

* 函数功能：

> <p id="custom_binder_t_custom_binder_unregister">注销自定义绑定器。

* 函数原型：

```
ret_t custom_binder_unregister (const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 控件的类型。 |
