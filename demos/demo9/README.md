# 本示例展示了MVVM 多窗口的基本用法。

MVVM应用程序中，通过navigator\_to函数实现窗口之间的导航。

每个窗口都需要调用NAVIGATOR\_ADD\_HANDLER将自己注册的到navigator中。

NAVIGATOR\_ADD\_HANDLER需要两个参数：

* name 窗口资源(XML)的名称。
* model\_create model的创建函数。

在本例子中注册了三个窗口：

```
  NAVIGATOR_ADD_HANDLER("demo9_main", NULL);
  NAVIGATOR_ADD_HANDLER("humidity", humidity_create);
  NAVIGATOR_ADD_HANDLER("temperature9", temperature_create);
```

在UI的XML文件中，可通过navigate命令打开窗口：

```
  <button text="Temperature" x="center" y="middle:-40" w="40%" h="40" 
    v-on:click="{navigate, args=temperature9}"/>
  <button text="Humidity" x="center" y="middle:40" w="40%" h="40" 
    v-on:click="{navigate, args=humidity}"/>
```

在UI的XML文件中，可通过nothing命令+CloseWindow参数关闭当前窗口：

```
 <button text="Close" x="center" y="bottom:40" w="40%" h="40" v-on:click="{nothing, CloseWindow=True}"/>
```



