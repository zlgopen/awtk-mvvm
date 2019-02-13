# javascript模型的创建方法

在创建javascript模型时，按下列顺序进行查找：

* 1. 与文件名同名的全局变量。

> 如，文件名为test\_model.js，则查找名为test\_model的全局变量。

* 2. 与文件名驼峰格式同名的全局变量。

> 如，文件名为test\_model.js，则查找名为testModel的全局变量。

* 3. 与文件名同名的创建函数。

> 如，文件名为test\_model.js，则查找名为create\_test\_model的全局函数，并调用此函数创建模型的实例。

* 4. 与文件名驼峰格式的同名的创建函数。

> 如，文件名为test\_model.js，则查找名为createTestModel的全局函数，并调用此函数创建模型的实例。