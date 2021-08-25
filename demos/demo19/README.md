# 本示例展示了通过属性控制动画

widget 的"exec"属性可以用来执行特定的函数，其值由函数名和参数两部分组成，两者用英文冒号分隔。

目前主要用于动画的控制，支持的函数有：

* start_animator 开始动画
* stop_animator 停止动画
* pause_animator 暂停动画
* destroy_animator 销毁动画

比如下面的属性表示开始 rotation 动画：

```
exec="start_animator:rotation"
```

在 MVVM 中，一般是根据模型中数据来控制动画。比如在本例中，温度低于 3 或高于 6 时开始动画，其它暂停动画，可以这样写：

```
{(value < 3 || value > 6) ? "start_animator:rotation" : "pause_animator:rotation"}
```

由于 XML 的属性中不允许出现<>"等字符，我们需要把属性独立出来：

```
    <property name="v-data:exec"><![CDATA[ {(value < 3 || value > 6) ? "start_animator:rotation" : "pause_animator:rotation"} ]]></property>
```    