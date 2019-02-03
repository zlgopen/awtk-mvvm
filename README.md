# awtk-mvvm

[MVVM(Model-View-ViewModel)介绍](docs/MVVM.md)

AWTK-MVVM是一套为[AWTK](https://github.com/zlgopen/awtk)用C语言开发并支持各种脚本语言的MVVM框架，实现了数据绑定、命令绑定和窗口导航等基本功能，使用AWTK-MVVM开发应用程序，无需学习[AWTK](https://github.com/zlgopen/awtk)本身的API，只需学习绑定规则和Model的实现方式即可。

我们在实现MVVM时，把AWTK相关的代码进行了隔离，而且AWTK相关的代码不足500行。所以它非常容易移植到其它GUI，从理论上讲，使用AWTK-MVVM开发的应用程序可以做得GUI无关，把GUI从AWTK换成emwin不需要修改应用程序。

使用方法：

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

