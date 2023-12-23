# 文件模型

名称：file

功能：用于读写文件内容，浏览（打开/保存）文件。

## 内置属性

| 属性 | 类型 | 说明 |
| :-----| ---- | :---- |
| filename | string | 文件名 |
| content | string | 文件内容 |
| size | number | 文件大小 |
| auto_load | boolean | 是否自动加载文件内容 |
| is_dirty | boolean | 文件内容是否被修改 |

## 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|
| browse | open(xxxx)/save(xxx) | 浏览文件 |

* browse 命令参数说明

> open(filter, title) 打开文件

示例

```xml
{browse, Args=open(filter='.txt.c.cpp.md', title='Text File')}
```
> saveas(filter, title) 保存文件

示例

```xml
{browse, Args=saveas(filter='.txt.c.cpp.md', title='Text File')}
```