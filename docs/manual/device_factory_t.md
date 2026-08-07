## device\_factory\_t
### 概述
设备工厂。
----------------------------------
### 函数
<p id="device_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#device_factory_t_device_factory_create_device">device\_factory\_create\_device</a> | 创建设备。 |
| <a href="#device_factory_t_device_factory_deinit">device\_factory\_deinit</a> | ~初始化。 |
| <a href="#device_factory_t_device_factory_has">device\_factory\_has</a> | 检查是否存在。 |
| <a href="#device_factory_t_device_factory_init">device\_factory\_init</a> | 初始化。 |
| <a href="#device_factory_t_device_factory_register">device\_factory\_register</a> | 注册。 |
| <a href="#device_factory_t_device_factory_unregister">device\_factory\_unregister</a> | 注销。 |
#### device\_factory\_create\_device 函数
-----------------------

* 函数功能：

> <p id="device_factory_t_device_factory_create_device">创建设备。

* 函数原型：

```
object_t device_factory_create_device (const char* type, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_t | 返回object对象。 |
| type | const char* | 类型。 |
| args | const char* | 参数。 |
#### device\_factory\_deinit 函数
-----------------------

* 函数功能：

> <p id="device_factory_t_device_factory_deinit">~初始化。

* 函数原型：

```
ret_t device_factory_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### device\_factory\_has 函数
-----------------------

* 函数功能：

> <p id="device_factory_t_device_factory_has">检查是否存在。

* 函数原型：

```
bool_t device_factory_has (const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示FALSE表示不存在。 |
| type | const char* | 类型。 |
#### device\_factory\_init 函数
-----------------------

* 函数功能：

> <p id="device_factory_t_device_factory_init">初始化。

* 函数原型：

```
ret_t device_factory_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### device\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="device_factory_t_device_factory_register">注册。

* 函数原型：

```
ret_t device_factory_register (const char* type, device_object_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 类型。 |
| create | device\_object\_create\_t | 创建回调函数指针。 |
#### device\_factory\_unregister 函数
-----------------------

* 函数功能：

> <p id="device_factory_t_device_factory_unregister">注销。

* 函数原型：

```
ret_t device_factory_unregister (const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 类型。 |
