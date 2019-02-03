# 本示例展示了MVVM validator的用法。

界面上有两个label控件和一个slider控件，三者都绑定到**温度(temp)**变量上，当slider拖动时，其修改模型temperature对象的value属性，temperature对象的value属性自动更新到label上。其中一个label显示温度，另外一个label显示温度是否正确的信息。

```
  <label text="1" x="0" y="middle" w="40%" h="40" v-data:text="{temp}"/>
  <label x="right" y="middle" w="40%" h="40" v-data:text="{error.of.temp}"/>
  <slider x="center" y="middle:40" w="80%" h="20" v-data:value="{temp, validator=water_temp}"/>
```

* data:text="{error.of.temp}" 的意思是把控件的text属性与模型的temp变量的错误信息关联起来。





