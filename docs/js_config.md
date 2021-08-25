# JS 相关配置

## 一、支持 jerryscript

AWTK-MVVM 默认支持 jerryscript，即 SConstruct 中存在以下代码：

```
os.environ['WITH_JS'] = 'true'
```

通常情况，我们并不需要 jerryscript 支持，可以禁用 jerryscript。修改 SConstruct，注释掉上面这行代码。

## 二、使用快照资源

为提高初始化效率，支持 jerryscript 时，AWTK-MVVM 默认使用快照资源 mvvm_factory.snapshot，即 SConstruct 中存在以下代码，若想禁用快照，将其注释掉即可：

```
os.environ['WITH_JS_SNAPSHOT'] = 'true'
```

快照是以 "空间换时间" 的方式提高初始化效率，禁用快照可以减少内存的消耗，但增加程序初始化的时间，用户可根据实际情况选择是否使用。

> 所谓的快照资源指使用 jerryscript 接口预编译 awtk-mvvm/tools/mvvm_factory_gen/mvvm_factory.js 后生成的资源。

## 三、jerryscript 内存池

jerryscript 可使用栈或堆的形式定义内存池，AWTK-MVVM 默认使用堆的方式分配内存空间给 jerryscript，即 SConstruct 中存在以下代码，若想使用栈的形式分配内存，将其注释掉即可：

```
os.environ['WITH_JS_EXTERNAL_CONTEXT'] = 'true'
```

1. 使用堆的形式分配内存时，可通过宏 JERRY_GLOBAL_HEAP_SIZE 设置 jerryscript 内存池大小，其单位为 KB。比如，设置内存池大小为 2 M：

```
    APP_CFLAGS += ' -DJERRY_EXTERNAL_CONTEXT=1 -DJERRY_GLOBAL_HEAP_SIZE=2048 '
```

2. 使用栈的形式分配内存时，可通过宏 CONFIG_MEM_HEAP_AREA_SIZE 设置 jerryscript 内存池大小，其单位为字节。比如，设置内存池大小为 2 M：

```
    APP_CFLAGS += ' -DCONFIG_MEM_HEAP_AREA_SIZE=2097152 '
```
