# 本示例展示了MVVM 用委托的方式简化model的实现。

### 一、界面

界面上有一个label控件、一个slider控件和一个button控件，前两者都绑定到**温度(temp)**变量上，当slider拖动时，其修改模型temperature对象的value属性，temperature对象的value属性自动更新到label上。在temperature对象的value属性值有变化时，button才启用，点击button执行save命令，button重新被禁用。

```
  <label text="1" x="center" y="middle:-40" w="80%" h="40" v-data:text="{temp}"/>
  <slider x="center" y="middle" w="90%" h="20" v-data:value="{temp}"/>
  <button text="Save" x="center" y="middle:40" w="40%" h="40" v-on:click="{save}"/>
```

* v-data:text="{temp}" 的意思是把控件的text属性与模型的temp变量关联起来。

* v-data:value="{temp}" 的意思是把控件的value属性与模型的temp变量关联起来。

* v-on:click="{save}" 的意思是click事件触发时，执行save命令。

### 二、模型

模型采用model\_delegate简化model的实现，不再需要实现set\_prop、get\_prop、can\_exec和exec等方法。

```
model_t* temperature_create(void) {
  temperature_t* t = TKMEM_ZALLOC(temperature_t);
  model_t* model = model_delegate_create(t, default_destroy);
 
  MODEL_SIMPLE_PROP(model, "temp", VALUE_TYPE_DOUBLE, &(t->value));
  MODEL_COMMAND(model, "save", temperature_save, temperature_can_save);

  return model;
}
```



