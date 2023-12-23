# awtk-mvvm

## 一、MVVM 
MVVM(Model-View-ViewModel) 是分离用户界面和业务逻辑的经典模式。

分离用户界面和业务逻辑的好处：

  * 让用户界面可以独立变化。
  * 方便为业务逻辑编写单元测试程序。
  * 方便 UI 设计师和程序员的分工和合作。

MVVM 相比 MVP 和 MVC 的优势：

  * 使用更简单。
  * 分离更彻底。
  * 声明式的编程更容易维护。

MVVM 的核心思想有两个：

* 通过数据绑定实现 View 与 Model 之间的数据交换。
* 通过命令绑定实现 View 事件与 Model 行为之间的关联。

## 二、AWTK-MVVM

AWTK-MVVM 是一套为 [AWTK](https://github.com/zlgopen/awtk) 用 C 语言开发，并支持各种脚本语言的 MVVM 框架，实现了数据绑定、命令绑定和窗口导航等基本功能，使用 AWTK-MVVM 开发应用程序，无需学习 [AWTK](https://github.com/zlgopen/awtk) 本身的 API，只需学习绑定规则和 Model 的实现方式即可。与其它 MVVM 框架相比，其特点有：

* 代码小。
* 性能高。
* 内存开销小。
* 隔离更彻底。
* 可移植到其它 GUI。
* 支持多种编程语言（目前支持 C/JS)。

## 三、使用方法

> 本文以 Linux/MacOS 为例，Windows 可能会微妙差异，请酌情处理。

#### 3.1 获取 awtk 并编译

```cmd
git clone https://github.com/zlgopen/awtk.git
cd awtk
scons
```

#### 3.2 获取 awtk-mvvm 并编译

>  awtk 和 awtk-mvvm 须处于同一级目录

* 获取 awtk-mvvm

```cmd
git clone https://github.com/zlgopen/awtk-mvvm.git
cd awtk-mvvm
git clone https://github.com/jerryscript-project/jerryscript.git 3rd/jerryscript/jerryscript
cd 3rd/jerryscript/jerryscript
git reset --hard 3737a28eafd580a2bee2794e4f5edd0c0471a0c6
```

> 当前适配的 jerryscript 的 SHA-1 为 3737a28eafd580a2bee2794e4f5edd0c0471a0c6；如果更新 jerryscript 为最新的代码，则可能会有兼容问题。

* 生成资源

```cmd
python ./scripts/update_res.py all
```

* 编译 PC 版本

```cmd
scons
```

> 完整的命令行参数可通过如下命令查看：

```cmd
scons help
```

* 编译 LINUX FB 版本（请先编译 awtk-linux-fb）

```cmd
scons LINUX_FB=true
```

* 运行 demos

```
./bin/demo1
```

* 注意事项

(1) 如果不需要 JS，请修改 SConstruct，注释掉下面这行代码，然后重新编译：

```python
os.environ['WITH_JS'] = 'true'
```

(3) 如果启用了 WITH_JS 和 WITH_JS_SNAPSHOT, 那么更新 JS 后，需重新编译生成 mvvm_factory_gen 工具，之后使用该工具重新生成 tools/mvvm_factory_gen/mvvm_factory.js 的快照，再重新编译 mvvm。
