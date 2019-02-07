# 本示例展示了MVVM显式更新的用法。

## 一、视图

界面上有一个label控件和一个slider控件，两者都绑定到**温度(temp)**变量上，当slider拖动时，由于指定了Trigger=Explicit，它并不会更新temperature对象的value，只有在点击"Apply"按钮时，才更新temperature对象的value。

```
  <label text="1" x="center" y="middle" w="50%" h="40" v-data:text="{temp}"/>
  <slider value="1" x="center" y="middle:40" w="80%" h="20" v-data:value="{temp, Trigger=Explicit}"/>
  <button name="apply" text="Apply" x="center" y="bottom:10" w="80%" h="30" 
    v-on:click="{nothing, UpdateModel=True}"/>
```

* v-data:text="{temp}" 的意思是把控件的text属性与模型的temp变量关联起来。

* v-data:value="{temp, Trigger=Explicit}" 的意思是把控件的value属性与模型的temp变量关联起来，但需要显式调用UpdateModel才会更新模型。

* v-on:click="{nothing, UpdateModel=True}" 这是一个命令绑定，nothing表示该命令什么也不做，但会执行UpdateModel把界面上的数据同步到模型。

## 二、模型

模型temperature里仅一个temp属性表示温度。

```
var temperature = { 
  temp : 20
}
```


