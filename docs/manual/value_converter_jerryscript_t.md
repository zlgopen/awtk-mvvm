## value\_converter\_jerryscript\_t
### 概述
![image](images/value_converter_jerryscript_t_0.png)

将jerryscript包装成值转换对象。

JS的全局对象ValueConverters，记录了所有的ValueConverter。
----------------------------------
### 函数
<p id="value_converter_jerryscript_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#value_converter_jerryscript_t_value_converter_jerryscript_deinit">value\_converter\_jerryscript\_deinit</a> | ~初始化jerryscript value converter。 |
| <a href="#value_converter_jerryscript_t_value_converter_jerryscript_init">value\_converter\_jerryscript\_init</a> | 初始化jerryscript value converter，注册相应的工厂函数。 |
#### value\_converter\_jerryscript\_deinit 函数
-----------------------

* 函数功能：

> <p id="value_converter_jerryscript_t_value_converter_jerryscript_deinit">~初始化jerryscript value converter。

* 函数原型：

```
ret_t value_converter_jerryscript_deinit ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
#### value\_converter\_jerryscript\_init 函数
-----------------------

* 函数功能：

> <p id="value_converter_jerryscript_t_value_converter_jerryscript_init">初始化jerryscript value converter，注册相应的工厂函数。

* 函数原型：

```
ret_t value_converter_jerryscript_init ();
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
