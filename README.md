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
git clone https://github.com/jerryscript-project/jerryscript.git 3rd/jerryscript
scons
```

> 编译linux-fb版本，请修改SConstruct

```
#for pc
#sys.path.insert(0, '../awtk/')

#for linux-fb
sys.path.insert(0, '../awtk-linux-fb/')
```

* 运行demos

```
./bin/demo1
```

> 本文以Linux/MacOS为例，Windows可能会微妙差异，请酌情处理。

## 四、文档
* [介绍](docs/8.intro.md)
* [ViewModel实现方法](docs/9.view_model.md)
* [数据绑定](docs/10.data_binding.md)
* [命令绑定](docs/11.command_binding.md)
* [窗口导航器](docs/12.navigate.md)
* [JS实现ViewModel](docs/13.js_view_model.md)
* [高级用法](docs/14.advance_usages.md)

## 五、示例

|  C DEMO  | JS DEMO  | 说明 |
| -------- | -------  | --------------------------------- |
| demo1    | jsdemo1  | 数据绑定基本用法                     |
| demo2    | jsdemo2  | trigger参数的用法                   |
| demo3    | jsdemo3  | converter参数的用法                 |
| demo4    | jsdemo4  | 显式更新(Trigger=Explicit)的用法     |
| demo5    | jsdemo5  | validator参数的用法                 |
| demo6    | jsdemo6  | 命令绑定的基本用法                    |
| demo7    | jsdemo7  | 以计算器为例的综合用法                |
| demo8    | jsdemo8  | 以shape编辑为例的综合用法             |
| demo9    | jsdemo9  | 多窗口的基本用法                     |
| demo10   | jsdemo10 | 定时器的基本用法                     |
| demo11   | jsdemo11 | 一个视图多个ViewModel的用法          |
| demo12   | jsdemo12 | 多窗口之间参数传递                   |
| demo13   | jsdemo13 | 列表视图的基本用法                   |
| demo14   | jsdemo14 | 快捷键的基本用法                     |
| demo15   | （no）   | 自定义导航器处理插件，以及MVVM窗口和非MVVM窗口的交互 |
| (no)     | jsdemo15 | combox 数据联动                     |
| (no)     | jsdemo16 | 动态界面                            |
