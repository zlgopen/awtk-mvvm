## xml mvvm 属性解析工具
该工具主要用于将xml文件中的MVVM属性提取出来，以属性可能存在路径的方式存储到指定文件中。
```
Usage: ./bin/mvvm_prop_gen.exe input output

input：文件输入，通常情况下是 .xml单文件 和 整个文件夹
> 存在递归逻辑，会遍历整个文件夹下所有的 .xml文件，注意文件夹层级。

output：文件输出，将可能存在的路径打包存储到指定文件中或者指定文件夹下。

目前支持如下两种可选形式：
单文件(input) + 单文件(output)
文件夹(input) + 文件夹(output)
> 其他情况皆为报错, 详细信息见下表。
```

#### 实际使用参数处理结果：
| 输入   | 输出   | 处理结果                               |
| ------ | ------ | -------------------------------------- |
| 单文件 | 单文件 | 将输入文件已使用属性解析存入到输出文件 |
| 文件夹 | 文件夹 | 以1:1的方式解析生成对应输出文件        |

#### 目前未支持属性
```
1、v-on
2、fscript {bind_data, bind_data = prop > 50 ? "true" : "false" }
3、fscript {index==selected_index}
4、no error return
   <view v-model="temp" v-data="value">
   <view v-data="value" v-model="temp">
5、...
```

