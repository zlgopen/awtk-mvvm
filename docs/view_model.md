## 用C语言实现ViewModel

用AWTK-MVVM开发应用程序时，开发者的主要工作是编写ViewModel和Model的代码。ViewModel本身是界面无关的，是可以自动测试的，所以简单的应用程序可以将ViewModel和Model合二为一，复杂的应用程序可以将两者分开。至于什么时候分开，什么时候合并，开发者可以自行决定。对于AWTK-MVVM框架来说，它只关心ViewModel，开发者按要求实现ViewModel接口即可。

用C语言实现ViewModel时，需要实现几个特定的接口函数。这主要是由C语言的特性决定的，C语言是静态编译的语言，也没有反射的功能，有些事情就比较麻烦了。为了弥补这些缺陷，我们需要实现下列几个接口函数：

1.没有办法通过属性的名称，从对象中获取属性的值。为此需要实现一个get_prop函数，给定一个属性的名称，返回该属性的值：

```
typedef ret_t (*object_get_prop_t)(object_t* obj, const char* name, value_t* v); 
```

2.没有办法通过属性的名称，去设置对象属性的值。为此需要实现一个set_prop函数，给定一个属性的名称和值，去设置该属性的值：

```
typedef ret_t (*object_set_prop_t)(object_t* obj, const char* name, const value_t* v); 
```

3.没有办法通过函数的名称，去调用对象的成员函数。为此需要实现exec函数，给定函数的名称，去执行对象的成员函数。还需要实现can_exec函数，检查该函数是否可用。

```
typedef bool_t (*object_can_exec_t)(object_t* obj, const char* name, const char* args);

typedef ret_t (*object_exec_t)(object_t* obj, const char* name, const char* args);
```

简单的说，实现ViewModel就是要实现以上这些函数，

### 1.手工编写ViewModel

这里，我们以一个温度控制器的例子来介绍如何手工实现一个ViewModel。给温度控制器设定一个温度，如果该温度与之前的温度不同，才可以把它设置到温度控制器中。

我们把温度控制器的名字取为temperature，它有：

* 一个double类型的value属性，代表要设置的温度。
* 还有一个apply的命令，负责让设置的温度生效。

**现在我们来定义一个temperature_t类。** 

在C语言中我们用结构体来模拟类，结构体的第一个成员是基类。下面的例子表示，temperature\_t类继承了view\_model\_t类。

```
typedef struct _temperature_t {
  view_model_t view_model;
  double value;

  double old_value;
} temperature_t;
```

> 这里"old_value"成员变量，用来保存之前的旧值，通过比较新旧的值是否相同，来决定"apply"命令是否可用。

**实现get_prop函数。** 

当属性名为"value"时，我们把成员变量"value"的值，放到参数"v"里返回给调用者。

```
static ret_t temperature_get_prop(object_t* obj, const char* name, value_t* v) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("value", name)) {
    double value = vm->value;
    value_set_double(v, value);
  } else {
    return RET_NOT_FOUND;
  }

  return RET_OK;
}
```

**实现set_prop函数。** 

当属性名为"value"时，我们把参数"v"的值赋给成员变量"value"。

```
static ret_t temperature_set_prop(object_t* obj, const char* name, const value_t* v) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("value", name)) {
    vm->value = value_double(v);
  } else {
    return RET_NOT_FOUND;
  }
  
  return RET_OK;
}
```

**实现can_exec函数。** 

当命令名称为"apply"时，调用函数temperature\_can\_exec\_apply去检查"apply"命令是否可用。在函数temperature\_can\_exec\_apply中，当新的值和旧的值不同时，返回TRUE表示"apply"命令可以执行。

```
static bool_t temperature_can_exec_apply(temperature_t* vm, const char* args) {
  return vm->old_value != vm->value;;
}

static bool_t temperature_can_exec(object_t* obj, const char* name, const char* args) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("apply", name)) {
    return temperature_can_exec_apply(vm, args);
  } else {
    return FALSE;
  }
}
```

**实现exec函数。** 

当命令名称为"apply"时，调用函数temperature\_apply。在函数temperature\_apply中去执行实际的命令。

```
static bool_t temperature_apply(temperature_t* vm, const char* args) {
  /*TODO: really apply the value*/
  vm->old_value = vm->value;;
  return RET_OK;
}

static ret_t temperature_exec(object_t* obj, const char* name, const char* args) {
  temperature_t* vm = (temperature_t*)(obj);

  if (tk_str_eq("apply", name)) {
    return temperature_apply(vm, args);
  } else {
    return RET_NOT_FOUND;
  }
}
```

**ViewModel构造函数。** 

现在我们用前面实现的函数来初始化object\_vtable\_t结构，并实现ViewModel的构造函数temperature\_create。

```
static const object_vtable_t s_temperature_vtable = {
  .type = "temperature",
  .desc = "temperature controller",
  .size = sizeof(temperature_t),
  .exec = temperature_exec,
  .can_exec = temperature_can_exec,
  .get_prop = temperature_get_prop,
  .set_prop = temperature_set_prop,
  .on_destroy = temperature_on_destroy
};

view_model_t* temperature_create(navigator_request_t* req) {
  return view_model_init(VIEW_MODEL(object_create(&s_temperature_vtable)));
}
```

**将ViewModel构造函数注册到工厂。** 

最后，还需要把ViewModel构造函数注册到工厂中，这样在XML文件中就可以直接指定ViewModel的名称了。

```
view_model_factory_register("temperature", temperature_create);
```


### 2.通过接口描述来生成ViewModel框架代码

前面我们手工实现了temperature这个ViewModel，这个过程不难，但是有些单调。写程序应该是件快乐的事情，我们用

### 3.手工编写ViewModelArray

### 4.通过接口描述来生成ViewModelArray框架代码
