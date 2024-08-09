# 配置文件模型

AWTK-MVVM 内置了一些配置文件模型，用于读写各种配置文件，比如，JSON/XML/INI/CSV/UBJSON 等。

> 文件名中可以引用下面的变量。

* ${app_dir} 应用程序目录
* ${user_dir} 用户目录
* ${temp_dir} 临时目录

```
json(url=${app_dir}/demos/demo_conf/demo.json)
```

> 支持 http/https 协议。支持 HTTP 协议时，要启用 socket。支持 HTTPS 协议时，要启用 ssl。

示例

```
json(url=http://t.weather.sojson.com/api/weather/city/101030100)
```

> 支持从资源加载数据。

## 1. json 配置

示例

```json
{
    "network" : {
        "device" : "eth0",
        "ip" : "192.168.1.1"
    },
    "serial" : {
        "device" : "COM1",
        "baudrate" : "115200"
    }
}
```

功能：用于读写 JSON 文件。

### 1.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

| 属性名 |属性的值 |
| :-----| ---- |
| network.device | eth0 |
| network.ip | 192.168.1.1 |
| serial.device | COM1 |
| serial.baudrate | 115200 |

### 1.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 1.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径（必须） |

示例

```json
json(url=${app_dir}/demos/demo_conf/demo.json)
```

## 2. ubjson 配置

示例

```ubjson
{
    "network" : {
        "device" : "eth0",
        "ip" : "192.168.1.1"
    },
    "serial" : {
        "device" : "COM1",
        "baudrate" : "115200"
    }
}
```

功能：用于读写 UBJSON 文件。

### 2.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

| 属性名 |属性的值 |
| :-----| ---- |
| network.device | eth0 |
| network.ip | 192.168.1.1 |
| serial.device | COM1 |
| serial.baudrate | 115200 |

### 2.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 2.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径（必须） |

示例

```ubjson
ubjson(url=${app_dir}/demos/demo_conf/demo.ubjson)
```

## 3. xml 配置

示例

```xml
<root>
  <network device="eth0" ip="192.168.1.1" />
  <serial device="COM1" baudrate="115200" />
</root>
```

功能：用于读写 XML 文件。

### 3.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

| 属性名 |属性的值 |
| :-----| ---- |
| network.device | eth0 |
| network.ip | 192.168.1.1 |
| serial.device | COM1 |
| serial.baudrate | 115200 |

### 3.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 3.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径（必须） |

示例

```xml
xml(url=${app_dir}/demos/demo_conf/demo.xml)
```

## 4. ini 配置

示例

```ini
[network]
  device = eth0
  ip = 192.168.1.1
[serial]
  device = COM1
  baudrate = 115200
```

功能：用于读写 INI 文件。

### 4.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

| 属性名 |属性的值 |
| :-----| ---- |
| network.device | eth0 |
| network.ip | 192.168.1.1 |
| serial.device | COM1 |
| serial.baudrate | 115200 |

### 4.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 4.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径（必须） |

示例

```ini
ini(url=${app_dir}/demos/demo_conf/demo.ini)
```

## 5. csv 配置

示例

```csv
eth0,192.168.1.1
eth1,192.168.1.2
eth2,192.168.1.3
```

功能：用于读写 CSV 文件。

### 5.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

> 用于列表项绑定

| 属性名 |属性的值 |
| :-----| ---- |
| item.device | 访问第一列 |
| item.ip | 访问第二列 |
| item.0 | 访问第一列 |
| item.1 | 访问第二列 |
| item.[0] | 访问第一列 |
| item.[1] | 访问第二列 |

### 5.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 5.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径（必须） |
| col_names | string | 列名，用分隔符分隔 |
| sep | char | 分隔符，默认为逗号 |
| name | string | 名称（可选，用于决定add/edit/detail子界面的文件名） |

示例

```csv
csv(url=${app_dir}/demos/demo_conf/demo.csv, col_names='device,ip', sep=',')
```

## 6. json_array 配置

示例

```json
{
  "networks": [
    {
      "device": "eth0",
      "ip": "192.168.1.1"
    },
    {
      "device": "eth1",
      "ip": "192.168.1.2"
    },
    {
      "device": "eth2",
      "ip": "192.168.1.3"
    }
  ]
}
```

功能：用于读写 JSON 文件（方便绑定到列表视图）。

### 6.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

> 用于列表项绑定

| 属性名 |属性的值 |
| :-----| ---- |
| item.device | device |
| item.ip | ip |

### 6.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 6.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径 （必须）|
| prefix | string | 路径前缀（可选） |
| name | string | 名称（可选，用于决定add/edit/detail子界面的文件名） |

示例

```json
json_array(url=${app_dir}/demos/demo_conf/demo_array.json, prefix=networks)
```

## 7. xml_array 配置

示例

```xml
<networks>
  <network device="eth0" ip="192.168.1.1" />
  <network device="eth1" ip="192.168.1.2" />
  <network device="eth2" ip="192.168.1.3" />
</networks>
```

功能：用于读写 XML 文件（方便绑定到列表视图）。

### 7.1 内置属性

可以通过路径引用属性值。比如上面的例子，可以用下面的方式访问：

> 用于列表项绑定

| 属性名 |属性的值 |
| :-----| ---- |
| item.device | device |
| item.ip | ip |

### 7.2 内置命令

| 命令 | 参数 | 说明 |
| :-----| ---- | :---- |
| save | 无 | 保存 |
| reload | 无 | 重新加载|

### 7.3 创建参数

| 参数 | 类型 | 说明 |
| :-----| ---- | :---- |
| url | string | 文件路径 （必须）|
| prefix | string | 路径前缀（可选） |
| name | string | 名称（可选，用于决定add/edit/detail子界面的文件名） |

示例

```xml
xml_array(url=${app_dir}/demos/demo_conf/demo_array.xml, prefix=networks)
```

## 8. 对于数组数据，绑定到列表时，可以添加、编辑、查看详细信息。

### 8.1 conf_add 用于添加数据

> 不需要指定 URL，但可以指定初始化数据。

添加数据的界面 UI 的文件名为: 

* [\<name\>_]\<prefix\>_add.xml。

name 和 prefix 在数组的模型中指定。比如，在下面的例子中，添加界面的文件名为：foo_networks_add.xml。

```xml
<window v-model="json_array(url=${app_dir}/demos/demo_conf/demo_array.json, prefix=networks, name=foo)">
```

示例

foo_networks_add.xml

```xml
<window v-model="conf_add(device='ethx',ip='192.168.1.100',mask='255.255.255.0',gateway='192.168.1.1')">
  <view w="100%" h="100%" children_layout="default(c=2,h=40,s=10,m=5)">
    <label text="Device" />
    <edit text="default" v-data:value="{device}"/>
    <label text="IP" />
    <edit v-data:value="{ip}" input_type="ipv4"/>
    <label text="Mask" />
    <edit v-data:value="{mask}" input_type="ipv4"/>
    <label text="GateWay" />
    <edit v-data:value="{gateway}" input_type="ipv4"/>
  </view>
  <button text="OK" floating="true" self_layout="default(x=20, y=b:80, w=128, h=30)" v-on:click="{save, CloseWindow=true}"/>
  <button text="Cancel" floating="true" self_layout="default(x=r:20, y=b:80, w=128, h=30)" v-on:click="{reload, CloseWindow=true}"/>
</window>
```

### 8.2 conf_edit 用于编辑数据

> 不需要指定 URL 和其它参数。


编辑数据的界面 UI 的文件名为: 

* [\<name\>_]\<prefix\>_edit.xml。

name 和 prefix 在数组的模型中指定。比如，在下面的例子中，编辑界面的文件名为：foo_networks_edit.xml。

```xml
<window v-model="json_array(url=${app_dir}/demos/demo_conf/demo_array.json, prefix=networks, name=foo)">
```

示例

```xml
<window v-model="conf_edit()">
  <view w="100%" h="100%" children_layout="default(c=2,h=40,s=10,m=5)">
    <label text="Device" />
    <edit v-data:value="{device}"/>
    <label text="IP" />
    <edit v-data:value="{ip}" input_type="ipv4"/>
    <label text="Mask" />
    <edit v-data:value="{mask}" input_type="ipv4"/>
    <label text="GateWay" />
    <edit v-data:value="{gateway}" input_type="ipv4"/>
  </view>
  <button text="OK" floating="true" self_layout="default(x=20, y=b:80, w=128, h=30)" v-on:click="{save, CloseWindow=true}"/>
  <button text="Cancel" floating="true" self_layout="default(x=r:20, y=b:80, w=128, h=30)" v-on:click="{reload, CloseWindow=true}"/>
</window>
```


### 8.3 conf_detail 用于查看数据

> 不需要指定 URL 和其它参数。

查看数据的界面 UI 的文件名为: 

* [\<name\>_]\<prefix\>_detail.xml。

name 和 prefix 在数组的模型中指定。比如，在下面的例子中，查看界面的文件名为：foo_networks_detail.xml。

```xml
<window v-model="json_array(url=${app_dir}/demos/demo_conf/demo_array.json, prefix=networks, name=foo)">
```

示例

```xml
<window v-model="conf_detail()">
  <view w="100%" h="100%" children_layout="default(c=2,h=40,s=10,m=5)">
    <label text="Device" />
    <edit v-data:value="{device}" readonly="true"/>
    <label text="IP" />
    <edit v-data:value="{ip}" input_type="ipv4" readonly="true"/>
    <label text="Mask" />
    <edit v-data:value="{mask}" input_type="ipv4" readonly="true"/>
    <label text="GateWay" />
    <edit v-data:value="{gateway}" input_type="ipv4" readonly="true"/>
  </view>
  <button text="OK" floating="true" self_layout="default(x=c, y=b:80, w=128, h=30)" v-on:click="{nothing, CloseWindow=true}"/>
</window>
```

完整示例请参考 demo_conf。

