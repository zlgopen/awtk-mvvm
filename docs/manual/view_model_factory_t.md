## view\_model\_factory\_t
### 概述
模型工厂。
----------------------------------
### 函数
<p id="view_model_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#view_model_factory_t_view_model_factory_create_model">view\_model\_factory\_create\_model</a> | 创建模型。 |
| <a href="#view_model_factory_t_view_model_factory_create_model_generic">view\_model\_factory\_create\_model\_generic</a> | 创建通用模型。 |
| <a href="#view_model_factory_t_view_model_factory_deinit">view\_model\_factory\_deinit</a> | ~初始化模型工厂。 |
| <a href="#view_model_factory_t_view_model_factory_exist">view\_model\_factory\_exist</a> | 检查指定的模型是否存在。 |
| <a href="#view_model_factory_t_view_model_factory_init">view\_model\_factory\_init</a> | 初始化模型工厂。 |
| <a href="#view_model_factory_t_view_model_factory_register">view\_model\_factory\_register</a> | 注册模型的创建函数。 |
| <a href="#view_model_factory_t_view_model_factory_register_generic">view\_model\_factory\_register\_generic</a> | 注册模型的通用创建函数(主要用于脚本语言)。 |
| <a href="#view_model_factory_t_view_model_factory_unregister">view\_model\_factory\_unregister</a> | 注销模型的创建函数。 |
#### view\_model\_factory\_create\_model 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_create_model">创建模型。

* 函数原型：

```
view_model_t* view_model_factory_create_model (const char* type, navigator_request_t* req);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | view\_model\_t* | 返回view\_model对象。 |
| type | const char* | 模型的类型。 |
| req | navigator\_request\_t* | 请求参数。 |
#### view\_model\_factory\_create\_model\_generic 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_create_model_generic">创建通用模型。

* 函数原型：

```
view_model_t* view_model_factory_create_model_generic (const char* type, navigator_request_t* req);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | view\_model\_t* | 返回view\_model对象。 |
| type | const char* | 模型的类型。 |
| req | navigator\_request\_t* | 请求参数。 |
#### view\_model\_factory\_deinit 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_deinit">~初始化模型工厂。

* 函数原型：

```
ret_t view_model_factory_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### view\_model\_factory\_exist 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_exist">检查指定的模型是否存在。

* 函数原型：

```
bool_t view_model_factory_exist (const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示存在，否则表示不存在。 |
| type | const char* | 模型的类型。 |
#### view\_model\_factory\_init 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_init">初始化模型工厂。

* 函数原型：

```
ret_t view_model_factory_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### view\_model\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_register">注册模型的创建函数。

* 函数原型：

```
ret_t view_model_factory_register (const char* type, view_model_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 模型的类型。 |
| create | view\_model\_create\_t | 创建函数。 |
#### view\_model\_factory\_register\_generic 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_register_generic">注册模型的通用创建函数(主要用于脚本语言)。

* 函数原型：

```
ret_t view_model_factory_register_generic (view_model_generic_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| create | view\_model\_generic\_create\_t | 创建函数。 |
#### view\_model\_factory\_unregister 函数
-----------------------

* 函数功能：

> <p id="view_model_factory_t_view_model_factory_unregister">注销模型的创建函数。

* 函数原型：

```
ret_t view_model_factory_unregister (const char* type);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| type | const char* | 模型的类型。 |
