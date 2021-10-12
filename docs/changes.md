# 最新动态

* 2021/10/12
  * 修正缺少__func__宏导致编译出错的问题（感谢陈谭提供补丁）

* 2021/10/11
  * fscript 添加 view model 相关的API；
  * 统一 C/JS/fscript 的 view model 相关的 API 的名称。
  * 完善 mvvm_app.inc

* 2021/09/29
  * 修改 object_js_base_is_listener_registered 有误导致死循环的问题。
  * 完善 demo。
  * 完善文档。
  * 适配 jerryscript（SHA-1: 8b3eb931670a563bd7bf52ed344263d9409f60b6）

* 2021/09/16
  * 修改 view model 不分割属性或者方法的路径，改由 model 自行处理。（感谢雨欣提供补丁）
  * 修正条件渲染时绑定规则更新不及时导致打印访问无效变量的告警的问题。（感谢雨欣提供补丁）
  * 修正条件渲染的表达式执行时没有去除尾部的“}”导致打印告警的问题。（感谢雨欣提供补丁）
  * 完善 view model 的消息传递。

* 2021/09/15
  * 新增 Application() 接口用于在 JS 代码中指定程序的生命周期函数。

* 2021/09/07
  * 新增 jsobj_unregister_global 接口以便注销全局的 JS 对象。

* 2021/08/25
  * 新增 ui_loader_mvvm，MVVM 绑定改为在 UI 数据加载时完成，减少控件上不必要的绑定属性的存储，以及方便实现条件渲染和列表渲染。
  * 新增条件渲染（v-if、v-elif、v-else）。
  * 新增列表渲染（v-for）。
  * 统一 command_binding 的 Args 字段的格式。
  * 完善 binding_context（主要是修改部分接口以便 ui_loader_mvvm 和 binding_rule 调用，细化 widget 和 ViewModel 的绑定逻辑）。
  * 修改 custom_binder 以适应新的 binding_context。
  * 新增 view_model_notify_items_changed 函数，用于触发 items 变化事件时可以指定发生变化的数组。
  * 完善 navigator（主要是重构 navigator_request_t，修改 navigator_request_t 的字符串形式的 Args 的格式与 command_binding 的 Args 字段一致，以及新增 ViewModel 相关的请求处理器）。
  * 重构 JS 形式的 ViewModel、ValueValidator、ValueConverter。
  * 新增注册 object_t 对象为 jerryscript 的全局对象的接口。
  * 完善 CPP Adapter。（感谢雨欣提供补丁）
  * 整理 Demo，将功能相同的 C、CPP、JS 的 Demo 改为使用同一个 ui，同时添加新的 Demo。（感谢雨欣提供补丁）
  * 导出 jsobj 的相关函数，以便自定义控件实现 custom_binder 时可以调用。（感谢雨欣提供补丁）
  * 完善 runJsMVVM。
  * 完善编译脚本。
  * 适配 jerryscript（SHA-1: 3bcd48f72d4af01d1304b754ef19fe1a02c96049）
  * 完善文档。（感谢雨欣提供补丁）
  * 修正由于 ui_builder_default 接口变化导致编译失败的问题 

* 2021/05/26
  * 修复去掉need_relayout_children的问题。

* 2021/05/18
  * 修复data_binding的内存泄漏问题。（感谢周聪提供补丁）
  * 修复view_model_compositor_on_unmount没有正确on_unmount的问题。（感谢周聪提供补丁）

* 2021/04/15
  * 增加demo34。演示模型触发界面更新。

* 2021/04/11
  * 快捷键格式修改为用下划线连接，如ctrl_s。（感谢福明提供补丁）

* 2021/03/25
  * 升级jerryscript到最新版本。

* 2021/03/01
  * 升级jerryscript到最新版本。

* 2021/01/23
  * 将tk\_expr\_eval移到到tkc。

* 2021/01/20
  * 在更新界面时，先处理命令绑定，再处理数据绑定，以便可通过enable的数据绑定，来启用/禁用控件。
  * 修复data\_binding\_object\_get\_prop使用自定义fscript函数崩溃的问题。

* 2021/01/12
  * 完善文档。

* 2021/01/10
  * 将部分fscript扩展函数移到awt/src/fscript\_ext/fscript\_widget.c中，请参考[fscript_widget](https://github.com/zlgopen/awtk/blob/master/docs/fscript_widget.md)


* 2020/12/16
  * 增加navigate\_request\_close函数，用于请求关闭指定窗口。

* 2020/12/05
  * navigate 的 target 支持参数，具体请[窗口导航](12.navigate.md )文档。
  * 增加navigate\_close函数，用于关闭指定窗口。
  * 增加 demo33，演示 navigate 参数。

* 2020/12/04
  * 数据绑定支持 ToModel 和 ToView 两个参数，用 fscript 实现数据格式双向转换的功能。请参考 [数据格式转换](10.data_binding.md)
  * 增加 fscript 命令，用来执行一段 fscript 脚本，并增加了一些函数。请参考 [高级用法](14.advance_usages.md)
  
> 请参考相应文档和 demo28。

* 2020/11/28
  * 更新关于表达式的文档。
  * 测试各个 demos, 修复 view\_model\_array\_object\_wrapper.c 和 view\_model\_object\_wrapper.c 中的问题。
  
* 2020/11/27
  * 表达式引擎升级为 fscript，请参考 [fscript 脚本引擎](https://github.com/zlgopen/awtk/blob/master/docs/fscript.md) 获取更多信息。

* 2020/11/26
  * 完善 demo32
  * 增加 window\_close/window\_open 事件。
  * 增加 navigator\_replace 打开新窗口并关闭当前窗口。
  * 修正 view\_model\_app\_conf 无法访问诸如“temp.[0].value”等属性的问题。

* 2020/11/18
  * 普通 view model 支持 path 形式访问属性及命令（感谢朝泽提供补丁）。
  * 修改 view\_model\_array 访问如"[0]"或"[0"的属性时返回 RET\_BAD\_PARAMS（感谢朝泽提供补丁）。
  * 单元测试崩溃的问题：没有初始化 jerryscript 和导出 jerry\_release\_value()，以及 view\_model\_compositor\_create 没有正确创建的原因（感谢朝泽提供补丁）。
  * 修正 Windows 下由于 include 的搜索顺序导致 include 非期望的文件从而引起编译失败的问题
  * 添加展示一个 view model 多个 model 的用法的 demo

* 2020/11/7
  * 修改 binding\_context\_on\_rebind 没有清除 updating\_view 标志的问题。
  * view\_model\_dummy\_can\_exec 返回 FALSE。

* 2020/10/27
  * 更新编译脚本
  * 修正 view_model_compositor 销毁时重复 unref 内部的 view model 的问题
  * 修正由于父 binding_context 先销毁导致子 binding_context 重置 parent 时异常的问题

* 2020/10/18
  * 支持全局按键事件，用于实现全局的快捷键。请参考 design/default/ui/demo9_main.xml

* 2020/09/26
  * 在 binding\_context\_awtk\_destroy 中，断开与 view model 的连接，避免 view model 被重用时，事件继续分发到被销毁的 context 上。

* 2020/09/25
  * 支持嵌套 view model。
  * 增加 demo31。

* 2020/09/18
  * 增加 value\_changed\_by\_ui 事件（感谢朝泽提供补丁）。
  * 增加 debug 命令。

* 2020/09/15
  * 支持长按键事件。

* 2020/09/13
  * 完善输入事情处理。如果命令执行成功，返回 RET_STOP 停止后续事件处理。

* 2020/09/12
  * 增加 SetWidgetProp 和 SendKey 内置命令。
  * 增加 demo30

* 2020/08/10
  * 修复 mvvm 的默认数据和 label 的绑定数据相同的时候，一开始会导致 label 不赋值，导致 label 不显示文字，在绑定后加上控件强制设置数据的操作来解决该问题（感谢智明提供补丁）。

* 2020/08/08
  * 修改绑定 style 的问题。

* 2020/08/06
  * 完善 view model 创建，复合 view model 支持 sub view model。

* 2020/07/24
  * 支持 table view 实现自定义的绑定。

* 2020/07/22
  * 完善 view model array，命令支持自定义命令参数，如果没有指定参数，以当前的 cursor 为参数。
  * 完善 binding\_context\_on\_rebind，在 rebind 时不再需要 update view。

* 2020/06/30
  * 增加 view_model_compositor
  * 增加 demo27 
  * 更新文档。

* 2020/06/26
  * 基本实现无代码编辑和保存 app conf。

* 2020/06/15
  * 完善 view\_model\_object\_wrapper/view\_model\_object\_array\_wrapper
  * 增加 demo24

* 2020/06/12
  * 增加 view\_model\_array\_object\_wrapper

* 2020/06/11
  * rule 的 patch 支持多级，如 font.name
  * 增加 demo22

* 2020/06/10
  * 支持 sub view model
  * 完善 view\_model\_object\_wrapper 支持 sub view model
  * 增加 app\_conf 的例子。
  * 增加 demo21

* 2020/05/03
  * javascript 支持加载模块（参考 temperature_ex.js)。
  * 完善 mvvm，修改 jsdemo15 的问题。

* 2019/12/03
  * 修改 view model array 的 CURSOR 不应该触发 prop changed 事件的问题

* 2019/12/02
  * 修改 validator fix value 之后界面没有及时更新的问题

* 2019/11/26
  * 提供 navigator\_info/navigator\_warn/navigator\_confirm 的缺省实现。

* 2019/11/23
  * 支持 NFD 打开文件对话框。

* 2019/11/10
  * 修改 view model 设置 cursor 属性时触发 CHANGED 事件的 BUG。

* 2019/11/07
  * 增加 demo19 演示控制动画的方法。

* 2019/11/02
  * 实现 navigator\_toast
  * 增加 navigator\_back
  * 增加 navigator\_back\_to\_home

* 2019/10/23
  * 修改 edit/combobox 在 listview 中的问题。
  * 增加键盘的资源。

* 2019/08/30
  * 增加 C++适配器。

* 2019/06/16
  * 重构
  * 增加文档。

* 2019/05/28
  * 增加按键事件触发命令。
  
* 2019/05/18
  * 代码产生器

* 2019/05/17
  * 介绍文档。

* 2019/03/25

* 2019/03/25
  * ListView 基本功能可用。

* 2019/03/05
  * 每个 view model 用独立的 context。

* 2019/02/25
  * 重构 scons 脚本。

* 2019/02/23
  * 重构
  * 增加 jsdemo12

* 2019/02/21
  * 重构
  * 增加 demo12

* 2019/02/20
  * 增加 demo11

* 2019/02/19
  * 重构
  * 支持一个窗口对应多个 model。 

* 2019/02/14
  * 重构
  * model 的创建函数增加 navigator\_request\_t 参数。

* 2019/02/13
  * 增加 timer。
  * 增加 demo10/jsdemo10。
  * javascript model 支持多实例。
  * 增加文档。
  * 更新示例。

* 2019/02/12
  * 重构

* 2019/02/11
  * 重构
  * 完善文档。

* 2019/02/10
  * 重构
  * 完善文档。

* 2019/02/09
  * 重构
  * 增加 jsdemo9
  * js 支持传递参数并返回值。

* 2019/02/08
  * 重构
  * 增加 navigator\_handler\_awtk
  * 增加 model 事件
  * 增加 demo7/demo9
  * 增加 model\_factory

* 2019/02/07
  * 重构
  * 增加 value\_validator\_jerryscript

* 2019/02/06
  * 重构
  * 增加测试
  * 支持 js 通知 c 属性有变化。
  * 增加 value\_converter\_jerryscript

* 2019/02/05
  * 增加 model\_jerryscript

* 2019/02/04
  * 完善基本框架。
  * 增加 demo7/demo8

* 2019/02/03
  * 完善基本框架。
  * 增加 demo5/demo6
  * 增加文档。

* 2019/02/02
  * 完善基本框架。
  * 增加 demo1/demo2/demo3/demo4

* 2019/02/01
  * 增加测试。
  * 合并 mvvm 分支。
  * 将 mvvm 代码移动到独立的项目：awtk-mvvm

* 2019/01/31
  * 增加 binding context awtk

* 2019/01/30
  * 增加 binding context
  
* 2019/01/29
  * 增加 navigator

* 2019/01/28
  * 增加 navigator\_request
  * 增加 navigator\_handler

* 2019/01/27
  * 增加 view\_model

* 2019/01/26
  * 修改字符串翻译的问题。issue 83/86
  * 增加 value converter factory
  * 增加 value validator factory
  * add expr eval

* 2019/01/25
  * 增加 model\_delegate。

* 2019/01/24
  * 增加 value\_validator/value\_validator\_delegate

* 2019/01/23
  * 增加 value\_converter/value\_converter\_delegate

* 2019/01/22
  * 增加 binding rule parse

* 2019/01/21
  * 增加 data\_binding/command\_binding.
  * 增加测试程序。

* 2019/01/20
  * 完善 object，增加一些包装函数。
  * 增加 binding\_rule/binding\_rule\_data
