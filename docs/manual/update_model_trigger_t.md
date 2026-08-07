## update\_model\_trigger\_t
### 概述
更新模型的时机。
### 常量
<p id="update_model_trigger_t_consts">

| 名称 | 说明 | 
| -------- | ------- | 
| UPDATE\_WHEN\_CHANGED | 编辑完成时才触发，即回车会或失去焦点时更新数据到模型。 |
| UPDATE\_WHEN\_CHANGING | 编辑时实时触发，在编辑时实时更新数据到模型。 |
| EUPDATE\_WHEN\_EXPLICIT | 需要显式触发。一般在命令绑定规则中指定的UpdateModel=True，执行命令前自动调用UpdateModel。 |
