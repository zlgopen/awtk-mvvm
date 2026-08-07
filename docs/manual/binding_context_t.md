## binding\_context\_t
### 概述
主要负责绑定规则的管理。
----------------------------------
### 函数
<p id="binding_context_t_methods">

| 函数名称 | 说明 | 
| -------- | ------------ | 
| <a href="#binding_context_t_binding_context_bind_command">binding\_context\_bind\_command</a> | 绑定命令到上下文。 |
| <a href="#binding_context_t_binding_context_bind_condition">binding\_context\_bind\_condition</a> | 绑定条件渲染到上下文。 |
| <a href="#binding_context_t_binding_context_bind_data">binding\_context\_bind\_data</a> | 绑定数据到上下文。 |
| <a href="#binding_context_t_binding_context_bind_items">binding\_context\_bind\_items</a> | 绑定列表渲染到上下文。 |
| <a href="#binding_context_t_binding_context_calc_widget_index_of_rule">binding\_context\_calc\_widget\_index\_of\_rule</a> | 获取指定所绑定的控件在控件树上的索引（列表渲染规则返回所渲染的第0个元素的索引）。 |
| <a href="#binding_context_t_binding_context_can_exec">binding\_context\_can\_exec</a> | 检查内置命令是否可以执行。 |
| <a href="#binding_context_t_binding_context_clear_bindings">binding\_context\_clear\_bindings</a> | 清除绑定规则。 |
| <a href="#binding_context_t_binding_context_clear_bindings_of_widget">binding\_context\_clear\_bindings\_of\_widget</a> | 清除指定控件的绑定规则。 |
| <a href="#binding_context_t_binding_context_destroy">binding\_context\_destroy</a> | 销毁binding context对象。 |
| <a href="#binding_context_t_binding_context_exec">binding\_context\_exec</a> | 执行内置命令。 |
| <a href="#binding_context_t_binding_context_get_items_cursor_of_rule">binding\_context\_get\_items\_cursor\_of\_rule</a> | 获取与指定绑定规则最近的一个列表渲染规则的当前数组cursor。 |
| <a href="#binding_context_t_binding_context_get_prop_by_rule">binding\_context\_get\_prop\_by\_rule</a> | 获取相对于指定绑定规则的数据的值。 |
| <a href="#binding_context_t_binding_context_get_root">binding\_context\_get\_root</a> | 获取根context。 |
| <a href="#binding_context_t_binding_context_init">binding\_context\_init</a> | 初始化。 |
| <a href="#binding_context_t_binding_context_notify_items_changed">binding\_context\_notify\_items\_changed</a> | 触发items改变事件。 |
| <a href="#binding_context_t_binding_context_notify_items_changed_sync">binding\_context\_notify\_items\_changed\_sync</a> | 触发items改变事件，并同步更新数据到视图。 |
| <a href="#binding_context_t_binding_context_resolve_path_by_rule">binding\_context\_resolve\_path\_by\_rule</a> | 解析相对于指定绑定规则的数据路径为ViewModel的绝对路径。 |
| <a href="#binding_context_t_binding_context_set_bound">binding\_context\_set\_bound</a> | 设置是否完成绑定。 |
| <a href="#binding_context_t_binding_context_set_parent">binding\_context\_set\_parent</a> | 设置父context。 |
| <a href="#binding_context_t_binding_context_set_prop_by_rule">binding\_context\_set\_prop\_by\_rule</a> | 设置相对于指定绑定规则的数据的值。 |
| <a href="#binding_context_t_binding_context_set_view_model">binding\_context\_set\_view\_model</a> | 设置绑定的ViewModel。 |
| <a href="#binding_context_t_binding_context_update_to_model">binding\_context\_update\_to\_model</a> | 更新数据到模型。 |
| <a href="#binding_context_t_binding_context_update_to_view">binding\_context\_update\_to\_view</a> | 更新数据到视图。 |
| <a href="#binding_context_t_binding_rule_parse">binding\_rule\_parse</a> | annotation ["global"] |
### 属性
<p id="binding_context_t_properties">

| 属性名称 | 类型 | 说明 | 
| -------- | ----- | ------------ | 
| <a href="#binding_context_t_bound">bound</a> | bool\_t | 已经完成绑定。 |
| <a href="#binding_context_t_command_bindings">command\_bindings</a> | darray\_t* | 命令绑定规则集合。 |
| <a href="#binding_context_t_data_bindings">data\_bindings</a> | darray\_t* | 数据绑定规则集合。 |
| <a href="#binding_context_t_dynamic_bindings">dynamic\_bindings</a> | darray\_t* | 动态绑定规则集合（包括条件渲染和列表渲染）。 |
| <a href="#binding_context_t_navigator_request">navigator\_request</a> | navigator\_request\_t* | 请求参数。 |
| <a href="#binding_context_t_parent">parent</a> | binding\_context\_t* | 父的binding_context对象。 |
| <a href="#binding_context_t_update_view_idle_id">update\_view\_idle\_id</a> | uint32\_t | 更新view的idle的id。 |
| <a href="#binding_context_t_updating_model">updating\_model</a> | bool\_t | 正在更新模型。 |
| <a href="#binding_context_t_updating_view">updating\_view</a> | bool\_t | 正在更新视图。 |
| <a href="#binding_context_t_updating_view_by_ui">updating\_view\_by\_ui</a> | bool\_t | 更新视图的事件由ui触发。 |
| <a href="#binding_context_t_view_model">view\_model</a> | view\_model\_t* | 绑定的ViewModel。 |
| <a href="#binding_context_t_widget">widget</a> | void* | 绑定的根控件。 |
#### binding\_context\_bind\_command 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_bind_command">绑定命令到上下文。

* 函数原型：

```
ret_t binding_context_bind_command (binding_context_t* ctx, binding_rule_t* rule);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 命令绑定规则。 |
#### binding\_context\_bind\_condition 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_bind_condition">绑定条件渲染到上下文。

* 函数原型：

```
ret_t binding_context_bind_condition (binding_context_t* ctx, binding_rule_t* rule);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 条件渲染规则。 |
#### binding\_context\_bind\_data 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_bind_data">绑定数据到上下文。

* 函数原型：

```
ret_t binding_context_bind_data (binding_context_t* ctx, binding_rule_t* rule);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 数据绑定规则。 |
#### binding\_context\_bind\_items 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_bind_items">绑定列表渲染到上下文。

* 函数原型：

```
ret_t binding_context_bind_items (binding_context_t* ctx, binding_rule_t* rule);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 列表渲染规则。 |
#### binding\_context\_calc\_widget\_index\_of\_rule 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_calc_widget_index_of_rule">获取指定所绑定的控件在控件树上的索引（列表渲染规则返回所渲染的第0个元素的索引）。

* 函数原型：

```
uint32_t binding_context_calc_widget_index_of_rule (binding_context_t* ctx, binding_rule_t* rule);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回索引。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 绑定规则。 |
#### binding\_context\_can\_exec 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_can_exec">检查内置命令是否可以执行。

* 函数原型：

```
bool_t binding_context_can_exec (binding_context_t* ctx, const char* cmd, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | bool\_t | 返回TRUE表示可以执行，否则表示不可以执行。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| cmd | const char* | 命令名。 |
| args | const char* | 命令参数。 |
#### binding\_context\_clear\_bindings 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_clear_bindings">清除绑定规则。

* 函数原型：

```
ret_t binding_context_clear_bindings (binding_context_t* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
#### binding\_context\_clear\_bindings\_of\_widget 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_clear_bindings_of_widget">清除指定控件的绑定规则。

* 函数原型：

```
ret_t binding_context_clear_bindings_of_widget (binding_context_t* ctx, void* widget);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| widget | void* | 控件对象。 |
#### binding\_context\_destroy 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_destroy">销毁binding context对象。

* 函数原型：

```
ret_t binding_context_destroy (binding_context_t* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
#### binding\_context\_exec 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_exec">执行内置命令。

* 函数原型：

```
ret_t binding_context_exec (binding_context_t* ctx, const char* cmd, const char* args);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| cmd | const char* | 命令名。 |
| args | const char* | 命令参数。 |
#### binding\_context\_get\_items\_cursor\_of\_rule 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_get_items_cursor_of_rule">获取与指定绑定规则最近的一个列表渲染规则的当前数组cursor。

* 函数原型：

```
uint32_t binding_context_get_items_cursor_of_rule (binding_context_t* ctx, binding_rule_t* rule);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | uint32\_t | 返回cursor。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 绑定规则。 |
#### binding\_context\_get\_prop\_by\_rule 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_get_prop_by_rule">获取相对于指定绑定规则的数据的值。

* 函数原型：

```
ret_t binding_context_get_prop_by_rule (binding_context_t* ctx, binding_rule_t* rule, const char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 绑定规则。 |
| name | const char* | 数据名称。 |
| v | value\_t* | 返回的值。 |
#### binding\_context\_get\_root 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_get_root">获取根context。

* 函数原型：

```
ret_t binding_context_get_root (binding_context_t* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回binding\_context对象。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
#### binding\_context\_init 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_init">初始化。

> 内部自动增加req和vm的引用计数。

* 函数原型：

```
ret_t binding_context_init (binding_context_t* ctx, navigator_request_t* req);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| req | navigator\_request\_t* | 请求参数对象。 |
#### binding\_context\_notify\_items\_changed 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_notify_items_changed">触发items改变事件。

* 函数原型：

```
ret_t binding_context_notify_items_changed (binding_context_t* ctx, tk_object_t* items);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| items | tk\_object\_t* | items对象。 |
#### binding\_context\_notify\_items\_changed\_sync 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_notify_items_changed_sync">触发items改变事件，并同步更新数据到视图。

* 函数原型：

```
ret_t binding_context_notify_items_changed_sync (binding_context_t* ctx, tk_object_t* items);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| items | tk\_object\_t* | items对象。 |
#### binding\_context\_resolve\_path\_by\_rule 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_resolve_path_by_rule">解析相对于指定绑定规则的数据路径为ViewModel的绝对路径。

* 函数原型：

```
const char* binding_context_resolve_path_by_rule (binding_context_t* ctx, binding_rule_t* rule, const char* path, bool_t* is_cursor);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | const char* | 为cursor时返回列表渲染规则的当前数组cursor，否则返回路径。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 绑定规则。 |
| path | const char* | 数据路径。 |
| is\_cursor | bool\_t* | 是否为cursor。 |
#### binding\_context\_set\_bound 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_set_bound">设置是否完成绑定。

* 函数原型：

```
ret_t binding_context_set_bound (binding_context_t* ctx, binding_context_t* parent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| parent | binding\_context\_t* | 父binding\_context对象。 |
#### binding\_context\_set\_parent 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_set_parent">设置父context。

* 函数原型：

```
ret_t binding_context_set_parent (binding_context_t* ctx, binding_context_t* parent);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| parent | binding\_context\_t* | 父binding\_context对象。 |
#### binding\_context\_set\_prop\_by\_rule 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_set_prop_by_rule">设置相对于指定绑定规则的数据的值。

* 函数原型：

```
ret_t binding_context_set_prop_by_rule (binding_context_t* ctx, binding_rule_t* rule, const char* name, value_t* v);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| rule | binding\_rule\_t* | 绑定规则。 |
| name | const char* | 数据名称。 |
| v | value\_t* | 值。 |
#### binding\_context\_set\_view\_model 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_set_view_model">设置绑定的ViewModel。

* 函数原型：

```
ret_t binding_context_set_view_model (binding_context_t* ctx, view_model_t* view_model);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
| view\_model | view\_model\_t* | 绑定的ViewModel对象。 |
#### binding\_context\_update\_to\_model 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_update_to_model">更新数据到模型。

* 函数原型：

```
ret_t binding_context_update_to_model (binding_context_t* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
#### binding\_context\_update\_to\_view 函数
-----------------------

* 函数功能：

> <p id="binding_context_t_binding_context_update_to_view">更新数据到视图。

* 函数原型：

```
ret_t binding_context_update_to_view (binding_context_t* ctx);
```

* 参数说明：

| 参数 | 类型 | 说明 |
| -------- | ----- | --------- |
| 返回值 | ret\_t | 返回RET\_OK表示成功，否则表示失败。 |
| ctx | binding\_context\_t* | binding\_context对象。 |
#### bound 属性
-----------------------
> <p id="binding_context_t_bound">已经完成绑定。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### command\_bindings 属性
-----------------------
> <p id="binding_context_t_command_bindings">命令绑定规则集合。

* 类型：darray\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### data\_bindings 属性
-----------------------
> <p id="binding_context_t_data_bindings">数据绑定规则集合。

* 类型：darray\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### dynamic\_bindings 属性
-----------------------
> <p id="binding_context_t_dynamic_bindings">动态绑定规则集合（包括条件渲染和列表渲染）。

* 类型：darray\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### navigator\_request 属性
-----------------------
> <p id="binding_context_t_navigator_request">请求参数。

* 类型：navigator\_request\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### parent 属性
-----------------------
> <p id="binding_context_t_parent">父的binding_context对象。

* 类型：binding\_context\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### update\_view\_idle\_id 属性
-----------------------
> <p id="binding_context_t_update_view_idle_id">更新view的idle的id。

* 类型：uint32\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### updating\_model 属性
-----------------------
> <p id="binding_context_t_updating_model">正在更新模型。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### updating\_view 属性
-----------------------
> <p id="binding_context_t_updating_view">正在更新视图。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### updating\_view\_by\_ui 属性
-----------------------
> <p id="binding_context_t_updating_view_by_ui">更新视图的事件由ui触发。

* 类型：bool\_t

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### view\_model 属性
-----------------------
> <p id="binding_context_t_view_model">绑定的ViewModel。

* 类型：view\_model\_t*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
#### widget 属性
-----------------------
> <p id="binding_context_t_widget">绑定的根控件。

* 类型：void*

| 特性 | 是否支持 |
| -------- | ----- |
| 可直接读取 | 是 |
| 可直接修改 | 否 |
