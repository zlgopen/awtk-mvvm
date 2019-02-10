# awtk-mvvm

## 一、MVVM 
MVVM(Model-View-ViewModel)是分离用户界面和业务逻辑的经典模式。

分离用户界面和业务逻辑的好处：

  * 让用户界面可以独立变化。
  * 方便为业务逻辑编写单元测试程序。
  * 方便UI设计师和程序员的分工和合作。

MVVM相比MVP和MVC的优势：

  * 使用更简单。
  * 分离更彻底。
  * 声明式的编程更容易维护。

MVVM的核心思想有两个：

* 通过数据绑定实现View与Model之间的数据交换。
* 通过命令绑定实现View事件与Model行为之间的关联。

## 二、AWTK-MVVM

AWTK-MVVM是一套为[AWTK](https://github.com/zlgopen/awtk)用C语言开发，并支持各种脚本语言的MVVM框架，实现了数据绑定、命令绑定和窗口导航等基本功能，使用AWTK-MVVM开发应用程序，无需学习[AWTK](https://github.com/zlgopen/awtk)本身的API，只需学习绑定规则和Model的实现方式即可。与其它MVVM框架相比，其特点有：

* 代码小。
* 性能高。
* 内存开销小。
* 隔离更彻底。
* 可移植到其它GUI。
* 支持多种编程语言(目前支持C/JS)。

> 目前还不成熟，API可能会有较大变化，请不要在产品中使用。

## 三、使用方法

* 获取awtk并编译。

```
git clone https://github.com/zlgopen/awtk.git
cd awtk
scons
```

* 获取awtk-mvvm并编译(awtk和awtk-mvvm处于用一级目录)

```
git clone https://github.com/zlgopen/awtk-mvvm.git
cd awtk-mvvm
scons
```

* 运行demos

```
./bin/demo1
```

> 本文以Linux/MacOS为例，Windows可能会微妙差异，请酌情处理。

