## ui_test 测试用例

### 用法

* 先启动被测程序

示例

```
./bin/demo1
```

* 运行测试程序

示例

```
../awtk/bin/ui_test tests/ui_test_data/slider_pointer_stress.ini
```

> 工作目录为 awtk-mvvm 根目录

### 1.slider_key_stress.ini		

* 功能 左右按键压力测试
* 适用于 
  * demo1/jsdemo1 
  * demo2/jsdemo2 
  * demo5/jsdemo5 
  * demo6/jsdemo6 

### 2.slider_pointer_stress.ini

* 功能 点击 slider 压力测试
* 适用于 
  * demo1/jsdemo1 
  * demo2/jsdemo2 
  * demo5/jsdemo5 
  * demo6/jsdemo6 

### 3.slider_pointer_apply_stress.ini

* 功能 点击 slider 和 apply 按钮 压力测试
* 适用于 
  * demo4/jsdemo4

### 4.rand_click_stress.ini

* 功能 随机点击压力测试
* 适用于 任意程序

### 5.demo8_stess.ini

* 功能 demo8 基本测试
* 适用于 demo8/jsdemo8

### 6.slider_key_0_100.ini
* 功能 演示带条件的 stop
* 适用于 有 slider 的界面

### 7.slider_key_0_50.ini
* 功能 演示带条件的 goto
* 适用于 有 slider 的界面

### 8.demo9_stess.ini

* 功能 demo9 基本测试
* 适用于 demo9/jsdemo9

### 9.demo15_stess.ini

* 功能 demo15 基本测试
* 适用于 demo15/jsdemo15

### 10.demo16_stess.ini

* 功能 demo16 基本测试
* 适用于 demo16/jsdemo16

