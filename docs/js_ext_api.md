# jerryscript 扩展函数

## 1.调试函数

### 1.1.print 打印调试信息

* 原型 

```
print(message)
```

* 示例

```
print('hello');
```

### 1.2 assert 断言

* 原型 

```
assert(bool_value)
```

* 示例

```
assert(true);
```

## 2.文件

### 2.1.file_exist 文件是否存在

* 原型 

```
file_exist(filename) -> 存在返回 true，否则返回 false
```

* 示例

```
assert(file_exist('test.json'));
```

### 2.2.file_remove 删除文件

* 原型 

```
file_remove(filename) -> 成功返回 true，否则返回 false
```

* 示例

```
assert(file_remove('test.json'));
```

### 2.3. text_file_write 写文本文件

* 原型 

```
text_file_write(filename, content) -> 成功返回 true，否则返回 false
```

* 示例

```
assert(text_file_write('test.json', '{count:123}'));
```

### 2.4. text_file_read 读文本文件

* 原型 

```
text_file_read(filename) -> 成功返回文件内容，失败返回 null。
```

* 示例

```
assert(text_file_read('test.json') == '{count:123}');
```

