# 本示例展示了MVVM最简单的用法。

界面上有一个label控件和一个slider控件，两者都绑定到**温度(temp)**变量上，当slider拖动时，其修改模型temperature对象的value属性，temperature对象的value属性自动更新到label上。

```
  <label text="1" x="center" y="middle" w="50%" h="40" v-data:text="{temp}"/>
  <slider value="1" x="center" y="middle:40" w="80%" h="20" v-data:value="{temp}"/>
```

* v-data:text="{temp}" 的意思是把控件的text属性与模型的temp变量关联起来。

* v-data:value="{temp}" 的意思是把控件的value属性与模型的temp变量关联起来。



