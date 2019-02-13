# 本示例展示了MVVM中timer用法。

界面上有一个label控件，它绑定到**温度(temp)**变量上，定时器每秒钟更新模型的value属性，temperature对象的value属性自动更新到label上。

```
  <label text="1" x="center" y="middle" w="50%" h="40" v-data:text="{temp}"/>
```

* v-data:text="{temp}" 的意思是把控件的text属性与模型的temp变量关联起来。




