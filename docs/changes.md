# 最新动态
* 2020/11/7
  * 修改binding\_context\_on\_rebind没有清除updating\_view标志的问题。

* 2020/10/27
  * 更新编译脚本
  * 修正view_model_compositor销毁时重复unref内部的view model的问题
  * 修正由于父binding_context先销毁导致子binding_context重置parent时异常的问题

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
