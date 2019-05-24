## 第10章 数据绑定

通过规则在视图和模型之间建立联系，当用户在视图上修改数据时，视图上的数据自动同步到模型中，当模型中的数据有变化时，自动更新到视图上去，这就是数据绑定。

### 10.1 建立视图与视图模型的联系

在进行数据绑定之前，先要建立视图与视图模型的联系。在AWTK中，我们用XML文件来描述用户界面，这个XML文件就是视图。在上一章中，我们实现了一个温度控制器的视图模型temperature，现在我们来写一个视图，在这个视图上有：

* 一个label控件用来显示当前的温度。
* 一个slider控件用来调节当前的温度。

这个文件的内容如下(具体细节请参考AWTK界面描述文件和布局参数)：

```
<window >
  <label x="center" y="middle" w="50%" h="40" text="0.000000"/>
  <slider x="center" y="middle:40" w="80%" h="20" value="0"/>
</window>
```

> 等AWStudio发布后，可以直接通过拖拽生成。

程序运行起来后，界面是这样的：

![view](images/temperature_view.png)

为了将视图与视图模型关联起来，我们为控件引入一个属性"v-model"来指定视图模型的名称，该属性可以应用到任何一个控件上。通常我们在窗口上指定，这样，整个窗口就与该视图模型关联起来了。

比如，下面的例子我们将视图与名为"temperature"的视图模型关联起来：

```
<window v-model="temperature">
...
</window>
```

视图模型的名称是怎么来的呢？在上一章中，我们注册视图模型时，有这样一段代码：

```
view_model_factory_register("temperature", temperature_view_model_create);
```

这段代码的意义就在于：需要名为"temperature"的视图模型时，调用函数temperature\_view\_model\_create去创建。

### 10.2 数据绑定的基本用法

数据绑定规则也是一个控件属性：

属性的名称由两部分组成，两者之间用英文冒号分隔。

* v-data 表示该属性是一个数据绑定规则。
* 第二部分是控件属性的名称，表示对该控件的哪个属性绑定进行绑定。

属性的值放在'{'和'}'之间，里面是视图模型中数据的名称。

如：

```
v-data:text="{value}" 表示将控件的"text"属性与视图模型的"value"属性关联起来。
```

```
v-data:value="{value}" 表示将控件的"value"属性与视图模型的"value"属性关联起来。
```

温度控制器的完整示例：

```
<window v-model="temperature">
  <label x="center" y="middle" w="50%" h="40" v-data:text="{value}"/>
  <slider x="center" y="middle:40" w="80%" h="20" v-data:value="{value}"/>
</window>
```

这样，label的文本和slider的值就与视图模型中的温度关联起来了。但用户拖动slider时，会自动修改模型中的温度，同时温度由会自动更新到label上。

在数据绑定规则中，还可以指定一些高级参数，这些参数之间用英文逗号分隔，后面我们将详细介绍这些参数的用法。

### 10.3 何时将数据更新到模型

