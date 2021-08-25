# 本示例展示了将复杂属性放到property tag中的用法。

有的属性值比较复杂，包含一些特殊字符，这些字符如果出现在属性中，需要进行转义处理，此时可以放到独立的property tag中。如：

```
  <label x="center" y="middle:-40" w="50%" h="40">
    <property name="v-data:text"><![CDATA[ {($value < 50) ? "low" : "high"} ]]></property>
    <property name="v-data:style"><![CDATA[ {($value < 50) ? "green" : "default" }]]></property>
  </label>
```