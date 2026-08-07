## object\_js\_factory\_t
### 概述
object jerryscript工厂。
----------------------------------
### 函数
<p id="object_js_factory_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#object_js_factory_t_object_js_factory">object\_js\_factory</a> | 获取缺省的object jerryscript工厂对象。 |
| <a href="#object_js_factory_t_object_js_factory_create">object\_js\_factory\_create</a> | 创建object jerryscript工厂对象。 |
| <a href="#object_js_factory_t_object_js_factory_create_object">object\_js\_factory\_create\_object</a> | 创建指定类型的object jerryscript对象。 |
| <a href="#object_js_factory_t_object_js_factory_destroy">object\_js\_factory\_destroy</a> | 析构并释放object jerryscript工厂对象。 |
| <a href="#object_js_factory_t_object_js_factory_register">object\_js\_factory\_register</a> | 注册object jerryscript创建函数。 |
| <a href="#object_js_factory_t_object_js_factory_set">object\_js\_factory\_set</a> | 设置缺省的object jerryscript工厂对象。 |
#### object\_js\_factory 函数
-----------------------

* 函数功能：

> <p id="object_js_factory_t_object_js_factory">获取缺省的object jerryscript工厂对象。

* 函数原型：

```
object_js_factory_t* object_js_factory ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_js\_factory\_t* | 返回object jerryscript工厂对象。 |
#### object\_js\_factory\_create 函数
-----------------------

* 函数功能：

> <p id="object_js_factory_t_object_js_factory_create">创建object jerryscript工厂对象。

* 函数原型：

```
object_js_factory_t* object_js_factory_create ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_js\_factory\_t* | 返回object jerryscript工厂对象。 |
#### object\_js\_factory\_create\_object 函数
-----------------------

* 函数功能：

> <p id="object_js_factory_t_object_js_factory_create_object">创建指定类型的object jerryscript对象。

* 函数原型：

```
object_js_t* object_js_factory_create_object (object_js_factory_t* factory, jsvalue_t jsobj, bool_t free_handle);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | object\_js\_t* | 返回object对象。 |
| factory | object\_js\_factory\_t* | object jerryscript工厂对象。 |
| jsobj | jsvalue\_t | jerryscript对象。 |
| free\_handle | bool\_t | object销毁的同时释放jerryscript对象。 |
#### object\_js\_factory\_destroy 函数
-----------------------

* 函数功能：

> <p id="object_js_factory_t_object_js_factory_destroy">析构并释放object jerryscript工厂对象。

* 函数原型：

```
ret_t object_js_factory_destroy (object_js_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | object\_js\_factory\_t* | object jerryscript工厂对象。 |
#### object\_js\_factory\_register 函数
-----------------------

* 函数功能：

> <p id="object_js_factory_t_object_js_factory_register">注册object jerryscript创建函数。

* 函数原型：

```
ret_t object_js_factory_register (object_js_factory_t* factory, jsobj_type_t type, object_js_create_t create);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | object\_js\_factory\_t* | reader工厂对象。 |
| type | jsobj\_type\_t | jerryscript对象的类型。 |
| create | object\_js\_create\_t | object jerryscript创建函数。 |
#### object\_js\_factory\_set 函数
-----------------------

* 函数功能：

> <p id="object_js_factory_t_object_js_factory_set">设置缺省的object jerryscript工厂对象。

* 函数原型：

```
ret_t object_js_factory_set (object_js_factory_t* factory);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| factory | object\_js\_factory\_t* | object jerryscript工厂对象。 |
