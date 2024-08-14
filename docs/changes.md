# 最新动态

* 2024/08/14
  * 增加UI自动测试数据

* 2024/08/09
  * 完善app_conf/conf的数组子对象的编辑/新增和详情。

* 2024/08/08
  * jerryscript 增加了几个扩展函数[JS 扩展函数](docs/js_ext_api.md)

* 2024/06/20
  * 增加支持 awtk-web 的文档和示例。

* 2024/05/09
  * 修正README.md。

* 2024/04/08
  * 创建view model时如果type为NULL则尝试从通用模型中创建，以便XML中不指定v-model属性时可以使用通用模型。

* 2024/03/26
  * 创建view model时req带上parent view model。

* 2024/03/22
  * 修复控件异步销毁时没有注销该控件与binding context之间的事件导致的崩溃问题。（感谢兆坤提供补丁）

* 2024/03/20
  * 修复控件异步销毁时清除绑定规则却没有注销相关事件导致的崩溃问题。（感谢兆坤提供补丁）

* 2024/02/04
  * view model compositor 支持通过前缀访问子vm的属性。

* 2024/02/21
  * 完善 MVVM 绑定的 can exec。（感谢兆坤提供补丁）
  * command binding 添加 is_continue 属性，主要解决控件同时绑定value_changed 和 value_changed_by_ui命令时，有个会不执行的问题。（感谢兆坤提供补丁）

* 2024/01/26
  * 完善items biding（感谢志鹏提供补丁）

* 2024/01/15
  * 修正 MVVM 的编译参数增加类型定义。（感谢智明提供补丁）

* 2024/01/12
  * MVVM 的编译参数增加类型定义。（感谢智明提供补丁）

* 2024/01/07
  * dummy view model支持设置初始值。

* 2024/01/01
  * view\_model\_conf 支持从request中获取参数。
  * view\_model\_file 支持从request中获取参数。

* 2023/12/30
  * 修复view\_model\_object\_wrapper/view\_model\_array\_object\_wrapper 事件分发的问题。

* 2023/12/29
  * 修复selected\_index可能大于size的问题。

* 2023/12/28
  * navigate命令的参数，target后可以直接跟参数?... 如：{navigate, Args=coffee_win?prefix=coffee}

* 2023/12/27
  * 完善 binding\_context\_awtk\_notify\_items\_changed，去掉优化，否则在同时触发items change和props changes时，props的界面没有更新。


* 2023/12/24
  * 增加view\_model\_conf
  * 完善binding\_context\_awtk\_update\_data

* 2023/12/23
  * 增加view\_model\_file

* 2023/12/14
  * 修正一个有绑定规则的控件从父控件移除后先销毁父控件再销毁自身会导致访问野指针的问题。（感谢智明提供补丁）

* 2023/12/12
  * 完善 binding\_context\_update\_error\_of，修复错误信息无法清除的问题。

* 2023/12/10
  * 完善 view\_model\_array\_preprocess\_prop，支持通过selected.来访问当前选择的项目。
  * 修复单元测试。
  * 增加单元测试。
  * 修复table view绑定时大量无用log

* 2023/12/09
  * 完善view\_model\_factory，支持指定view\_model的参数。比如: db(table=scores, filter='chinese > 90');

* 2023/12/05
  * 完善view\_model\_array\_object\_wrapper，支持detail命令。

* 2023/12/01
  * 修改命令绑定支持诸如鼠标+Ctrl的快捷方式。（感谢兆坤提供补丁）

* 2023/11/28
  * 增加demo42。

* 2023/11/20
  * 修改部分返回值错误。

* 2023/09/05
  * 修正绑定规则中的 fscript 表达式初始化时由于规则对象未绑定 binding\_context 导致表达式解析时打印告警的问题。（感谢兆坤提供补丁）

* 2023/08/28
  * 完善编译脚本。（感谢智明提供补丁）

* 2023/08/23
  * 修正注释。（感谢兆坤提供补丁）

* 2023/07/19
  * improve binding\_context\_awtk for global key.

* 2023/07/12
  * 导出 navigator\_close 到 fscript。（感谢兆坤提供补丁）

* 2023/06/26
  * 修改  mvvm\_prop\_gen 解析绑定时先处理 v-model 属性。（感谢高源提供补丁）

* 2023/06/20
  * 解决 mvvm\_prop\_gen 导出文件为空的问题。（感谢高源提供补丁）
  * 完善 mvvm\_prop\_gen 的 readme。（感谢高源提供补丁）

* 2023/06/19
  * 添加 mvvm\_prop\_gen，用于将 UI 文件 MVVM 属性绑定中用到的变量提取出来。（感谢高源提供补丁）

* 2023/05/31
  * 完善调试信息。（感谢兆坤提供补丁）

* 2023/05/28
  * 完善代码生成工具。框架需要的是view\_model而不是model，所以使用emitter\_forward\_retarget代替emitter\_forward。

* 2023/04/04
  * 添加 ui\_loader\_mvvm\_load\_widget\_with\_parent 和 完善 ui\_loader\_mvvm\_load_\widget，支持 applet，以及更新部分注释。（感谢兆坤提供补丁）
  * 修改注册 app\_conf 到 JS 层时，如果 app\_conf 为空不打印告警。
  * 添加 navigator\_set\_screen\_saver\_time 以及相关 demo。（感谢泽武提供补丁）
  * 修改注释。

* 2023/03/06
  * 完善README.md。（感谢雨欣提供补丁）

* 2023/01/05
  * 完善 jsvalue\_from\_value、jsvalue\_to\_value，支持 bitmap 类型。

* 2022/12/14
  * 修复编译问题。（感谢兆坤提供补丁）

* 2022/12/02
  * 修改在 JS 代码中可以通过 appConf 访问 C 语言的 app_conf 对象。

* 2022/12/01
  * 修复在 Fscript 中移除数组的元素后数组长度没更新的问题。（感谢雨欣提供补丁）

* 2022/11/07
  * 完善 JS 数组的适配，以便在 Fscript 中使用。（感谢雨欣提供补丁）

* 2021/09/09
  * 修正 args 为 Object 类型时 JS 层 tk\_object\_t 的 canExec 和 exec 没有正确传参的问题。

* 2021/09/04
  * 修改 binding\_context 解绑后不刷新界面，避免使用无效的 JS 模型数据导致异常。

* 2021/09/02
  * 修正条件渲染时如果所有条件都不满足没有销毁上一次符合条件的控件的问题。
  * 修正 API 注释。（感谢福明提供补丁）

* 2021/09/01
  * 修改 ui build 的时候不触发 value\_changed\_by\_ui 事件。（感谢兆坤提供补丁）

* 2022/08/28
  * 增加 mvvm\_awtk\_is\_quited，用于判断MVVM是否已经退出，已经退出则不再更新界面。

* 2021/08/18
  * 修正 navigator\_count\_view\_models 返回值有误的问题。（感谢福明提供补丁）

* 2021/08/11
  * jerry-all-in.c 统一 include 源代码的方式。

* 2021/08/09
  * 修正由于 binding rule 未及时清理导致列表项发生变化时打印过多log的问题。

* 2021/08/05
  * 导出tk_object_t对象的forwardEvent、unforwardEvent到JS。

* 2021/07/18
  * 完善view\_model\_dummy/view\_model\_array\_dummy。
  * ui\_loader\_mvvm 支持加载xml文件。

* 2021/04/18
  * 修复 demo20 串口通信异常的问题。（感谢智明提供补丁）

* 2021/03/23
  * 完善 mingw 的编译问题。（感谢智明提供补丁）

* 2021/03/04
  * 修复绑定数据后 value\_changed\_by\_ui 事件无法触发的问题。

* 2021/03/03
  * 修复 view\_model\_array\_object\_wrapper 包装的对象的项目数量变化后界面没有同步更新的问题。

* 2021/02/23
  * 导出 tk\_command\_arguments\_to\_object()到 fscript。（感谢雨欣提供补丁）
  * 集成 Iot.js。（感谢智明提供补丁）
  * 消除未使用的变量或函数的warning。（感谢智明提供补丁）

* 2021/02/14
  * 修复在 idle 销毁 binding\_context 前定时器触发 idle\_update\_to\_view 导致打印告警的问题。（感谢兆坤提供补丁）

* 2021/02/11
  * 修正 data\_binding 内存泄漏的问题。（感谢兆坤提供补丁）

* 2021/02/10
  * 修正 fscript\_eval 或 fscript\_exec 后没有 reset 返回的结果导致内存泄漏的问题。（感谢兆坤提供补丁）
  * 完善数据转换器，可以通过 ConverterArgs 传递参数给转换器。（感谢智明提供补丁）

* 2021/01/25
  * 修正连续多个条件渲染时计算控件序号有误的问题。

* 2021/01/21
  * 修改错误的注释。（感谢兆坤提供补丁）

* 2021/01/05
  * 确保释放jerryscript前关闭全部窗口，避免jerryscript出现断言

* 2021/12/28
  * 修复在VS中编译出现snprintf重定义的问题。

* 2021/12/27
  * 延后控件数据绑定的时机，实现控件事件触发后先进行数据同步（比如控件发生value\_changed事件之后先触发数据同步，再触发命令绑定）。（感谢智明提供补丁）

* 2021/12/09
  * 修改object_t为tk\_object\_t。

* 2021/11/24
  * 完善 binding\_context，修正子控件绑定view model时由于时序有误导致父的view model没有销毁的问题。

* 2021/11/23
  * 完善 binding\_context，修正内存泄漏以及在条件渲染控件的子控件中定义v-model时没有正确解绑和触发WILL\_UNMOUNT事件的问题。
  * 修正 WITH_JS 非 true 时无法正常编译的问题。

* 2021/11/16
  * 增加 navigator\_get\_locale、navigator\_set\_locale，用于获取和设置界面的语言。（感谢雨欣提供补丁）
  * 增加 navigator\_get\_theme、navigator\_set\_theme，用于获取和设置界面的主题。（感谢雨欣提供补丁）
  * 增加 demo40、jsdemo40，演示界面主题和语言的切换。（感谢雨欣提供补丁）
  * 完善文档。

* 2021/11/14
  * 修改最新jerryscript文件改名的问题。

* 2021/10/28
  * 消除 4013 warning

* 2021/10/14
  * 修正Windows x86下编译失败的问题

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
  * 新增注册tk_object_t 对象为 jerryscript 的全局对象的接口。
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
