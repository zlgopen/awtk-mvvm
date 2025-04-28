## xml mvvm 属性解析工具

该工具主要用于将xml文件中的MVVM属性提取出来，以属性可能存在路径的方式存储到指定文件中。

```bash
Usage: ./bin/mvvm_prop_gen.exe input output [src_filename]
```

参数：

* input：需要提取的文件，可以是单个ui文件，也可以是包含ui文件的文件夹。为文件夹且没有指定src_filename参数时，会递归处理子文件夹。
* output：输出文件或文件夹路径，input为当俄ui文件时，output也必须为文件路径；input为文件夹时，output也必须为已经存在的文件夹路径。
* src_filename：指定ui文件的配置。当input为文件夹时有效。如果指定，则只处理配置中指定的ui文件。

### src_filename示例：

```json
{
  "sources": [
    "home_page.xml"
    "page1.xml",
    "foo/bar.xml"
  ]
}
```

### 实际使用参数处理结果：

| 输入   | 输出   | 处理结果                               |
| ------ | ------ | -------------------------------------- |
| 单文件 |        | 将输入文件已使用属性解析并打印到控制台 |
| 单文件 | 单文件 | 将输入文件已使用属性解析存入到输出文件 |
| 文件夹 | 文件夹 | 以1:1的方式解析生成对应输出文件        |

### 导出文件说明

```xml
<window v-model="user_message">
  <input v-data:text="{username}"/>
  ...
</window>
```

```json
{
  "username": [                  /* 变量名称 */
    {
      "path": "user_message",    /* 作用域路径 */
      "is_array": false,         /* 是否为数组变量 */
    }
  ]
  ...
}
```
