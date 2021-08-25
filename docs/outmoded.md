# 过时用法

## 一、命令绑定 (command_binding)

### 1.1 内置命令

1. SendKey：请使用 fscript 代替本函数，但仍然保存对旧命令的兼容；
2. SetWidgetProp：请使用 fscript 代替本函数，但仍然保存对旧命令的兼容；
3. debug：请使用 fscript 代替本函数，但仍然保存对旧命令的兼容。

> 以上过时用法以及新用法示例详见 [命令绑定](11.command_binding.md)。

## 二、高级用法 (advance_usages)

1. 嵌入表达式：变量名以 "\$" 符号开头是过时用法，可删除 "\$" 符号，但仍然保存对旧用法的兼容；
2. 列表渲染：v-for-items 是过时用法，请使用 v-for 代替，但仍然保存对旧用法的兼容。

> 以上过时用法以及新用法示例详见 [高级用法](14.advance_usages.md)。
